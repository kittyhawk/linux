/*
 * File:         arch/blackfin/mach-common/dpmc.S
 * Based on:
 * Author:       LG Soft India
 *
 * Created:      ?
 * Description:  Watchdog Timer APIs
 *
 * Modified:
 *               Copyright 2004-2006 Analog Devices Inc.
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <linux/linkage.h>
#include <asm/blackfin.h>
#include <asm/mach/irq.h>

.text

ENTRY(_unmask_wdog_wakeup_evt)
	[--SP] = ( R7:0, P5:0 );
#if defined(CONFIG_BF561)
	P0.H = hi(SICA_IWR1);
	P0.L = lo(SICA_IWR1);
#else
	P0.h = (SIC_IWR >> 16);
	P0.l = (SIC_IWR & 0xFFFF);
#endif
	R7 = [P0];
#if defined(CONFIG_BF561)
	BITSET(R7, 27);
#else
	BITSET(R7,(IRQ_WATCH - IVG7));
#endif
	[P0] = R7;
	SSYNC;

	( R7:0, P5:0 ) = [SP++];
	RTS;

.LWRITE_TO_STAT:
	/* When watch dog timer is enabled, a write to STAT will load the
	 * contents of CNT to STAT
	 */
	R7 = 0x0000(z);
#if defined(CONFIG_BF561)
	P0.h = (WDOGA_STAT >> 16);
	P0.l = (WDOGA_STAT & 0xFFFF);
#else
	P0.h = (WDOG_STAT >> 16);
	P0.l = (WDOG_STAT & 0xFFFF);
#endif
	[P0] = R7;
	SSYNC;
	JUMP .LSKIP_WRITE_TO_STAT;

ENTRY(_program_wdog_timer)
	[--SP] = ( R7:0, P5:0 );
#if defined(CONFIG_BF561)
	P0.h = (WDOGA_CNT >> 16);
	P0.l = (WDOGA_CNT & 0xFFFF);
#else
	P0.h = (WDOG_CNT >> 16);
	P0.l = (WDOG_CNT & 0xFFFF);
#endif
	[P0] = R0;
	SSYNC;

#if defined(CONFIG_BF561)
	P0.h = (WDOGA_CTL >> 16);
	P0.l = (WDOGA_CTL & 0xFFFF);
#else
	P0.h = (WDOG_CTL >> 16);
	P0.l = (WDOG_CTL & 0xFFFF);
#endif
	R7 = W[P0](Z);
	CC = BITTST(R7,1);
	if !CC JUMP .LWRITE_TO_STAT;
	CC = BITTST(R7,2);
	if !CC JUMP .LWRITE_TO_STAT;

.LSKIP_WRITE_TO_STAT:
#if defined(CONFIG_BF561)
	P0.h = (WDOGA_CTL >> 16);
	    P0.l = (WDOGA_CTL & 0xFFFF);
#else
	P0.h = (WDOG_CTL >> 16);
	    P0.l = (WDOG_CTL & 0xFFFF);
#endif
	R7 = W[P0](Z);
	BITCLR(R7,1);   /* Enable GP event */
	BITSET(R7,2);
	W[P0] = R7.L;
	SSYNC;
	NOP;

	R7 = W[P0](Z);
	BITCLR(R7,4);   /* Enable the wdog counter */
	W[P0] = R7.L;
	SSYNC;

	( R7:0, P5:0 ) = [SP++];
	RTS;

ENTRY(_clear_wdog_wakeup_evt)
	[--SP] = ( R7:0, P5:0 );

#if defined(CONFIG_BF561)
	P0.h = (WDOGA_CTL >> 16);
	P0.l = (WDOGA_CTL & 0xFFFF);
#else
	P0.h = (WDOG_CTL >> 16);
	P0.l = (WDOG_CTL & 0xFFFF);
#endif
	R7 = 0x0AD6(Z);
	W[P0] = R7.L;
	SSYNC;

	R7 = W[P0](Z);
	BITSET(R7,15);
	W[P0] = R7.L;
	SSYNC;

	R7 = W[P0](Z);
	BITSET(R7,1);
	BITSET(R7,2);
	W[P0] = R7.L;
	SSYNC;

	( R7:0, P5:0 ) = [SP++];
	RTS;

ENTRY(_disable_wdog_timer)
	[--SP] = ( R7:0, P5:0 );
#if defined(CONFIG_BF561)
	P0.h = (WDOGA_CTL >> 16);
	P0.l = (WDOGA_CTL & 0xFFFF);
#else
	P0.h = (WDOG_CTL >> 16);
	P0.l = (WDOG_CTL & 0xFFFF);
#endif
	R7 = 0xAD6(Z);
	W[P0] = R7.L;
	SSYNC;
	( R7:0, P5:0 ) = [SP++];
	RTS;

#if !defined(CONFIG_BF561)

.section .l1.text

ENTRY(_sleep_mode)
	[--SP] = ( R7:0, P5:0 );
	[--SP] =  RETS;

	call _set_sic_iwr;

	R0 = 0xFFFF (Z);
	call _set_rtc_istat

	P0.H = hi(PLL_CTL);
	P0.L = lo(PLL_CTL);
	R1 = W[P0](z);
	BITSET (R1, 3);
	W[P0] = R1.L;

	CLI R2;
	SSYNC;
	IDLE;
	STI R2;

	call _test_pll_locked;

	R0 = IWR_ENABLE(0);
	call _set_sic_iwr;

	P0.H = hi(PLL_CTL);
	P0.L = lo(PLL_CTL);
	R7 = w[p0](z);
	BITCLR (R7, 3);
	BITCLR (R7, 5);
	w[p0] = R7.L;
	IDLE;
	call _test_pll_locked;

	RETS = [SP++];
	( R7:0, P5:0 ) = [SP++];
	RTS;

ENTRY(_hibernate_mode)
	[--SP] = ( R7:0, P5:0 );
	[--SP] =  RETS;

	call _set_sic_iwr;

	R0 = 0xFFFF (Z);
	call _set_rtc_istat

	P0.H = hi(VR_CTL);
	P0.L = lo(VR_CTL);
	R1 = W[P0](z);
	BITSET (R1, 8);
	BITCLR (R1, 0);
	BITCLR (R1, 1);
	W[P0] = R1.L;
	SSYNC;

	CLI R2;
	IDLE;

	/* Actually, adding anything may not be necessary...SDRAM contents
	 * are lost
	 */

ENTRY(_deep_sleep)
	[--SP] = ( R7:0, P5:0 );
	[--SP] =  RETS;

	CLI R4;

	call _set_sic_iwr;

	call _set_sdram_srfs;

	/* Clear all the interrupts,bits sticky */
	R0 = 0xFFFF (Z);
	call _set_rtc_istat

	P0.H = hi(PLL_CTL);
	P0.L = lo(PLL_CTL);
	R0 = W[P0](z);
	BITSET (R0, 5);
	W[P0] = R0.L;

	call _test_pll_locked;

	SSYNC;
	IDLE;

	call _unset_sdram_srfs;

	call _test_pll_locked;

	R0 = IWR_ENABLE(0);
	call _set_sic_iwr;

	P0.H = hi(PLL_CTL);
	P0.L = lo(PLL_CTL);
	R0 = w[p0](z);
	BITCLR (R0, 3);
	BITCLR (R0, 5);
	BITCLR (R0, 8);
	w[p0] = R0;
	IDLE;
	call _test_pll_locked;

	STI R4;

	RETS = [SP++];
	( R7:0, P5:0 ) = [SP++];
	RTS;

ENTRY(_sleep_deeper)
	[--SP] = ( R7:0, P5:0 );
	[--SP] =  RETS;

	CLI R4;

	P3 = R0;
	R0 = IWR_ENABLE(0);
	call _set_sic_iwr;
	call _set_sdram_srfs;

	/* Clear all the interrupts,bits sticky */
	R0 = 0xFFFF (Z);
	call _set_rtc_istat

	P0.H = hi(PLL_DIV);
	P0.L = lo(PLL_DIV);
	R6 = W[P0](z);
	R0.L = 0xF;
	W[P0] = R0.l;

	P0.H = hi(PLL_CTL);
	P0.L = lo(PLL_CTL);
	R5 = W[P0](z);
	R0.L = (MIN_VC/CONFIG_CLKIN_HZ) << 9;
	W[P0] = R0.l;

	SSYNC;
	IDLE;

	call _test_pll_locked;

	P0.H = hi(VR_CTL);
	P0.L = lo(VR_CTL);
	R7 = W[P0](z);
	R1 = 0x6;
	R1 <<= 16;
	R2 = 0x0404(Z);
	R1 = R1|R2;

	R2 = DEPOSIT(R7, R1);
	W[P0] = R2;

	SSYNC;
	IDLE;

	call _test_pll_locked;

	P0.H = hi(PLL_CTL);
	P0.L = lo(PLL_CTL);
	R0 = W[P0](z);
	BITSET (R0, 3);
	W[P0] = R0.L;

	R0 = P3;
	call _set_sic_iwr;

	SSYNC;
	IDLE;

	call _test_pll_locked;

	R0 = IWR_ENABLE(0);
	call _set_sic_iwr;

	P0.H = hi(VR_CTL);
	P0.L = lo(VR_CTL);
	W[P0]= R7;

	SSYNC;
	IDLE;

	call _test_pll_locked;

	P0.H = hi(PLL_DIV);
	P0.L = lo(PLL_DIV);
	W[P0]= R6;

	P0.H = hi(PLL_CTL);
	P0.L = lo(PLL_CTL);
	w[p0] = R5;
	IDLE;
	call _test_pll_locked;

	call _unset_sdram_srfs;

	STI R4;

	RETS = [SP++];
	( R7:0, P5:0 ) = [SP++];
	RTS;

ENTRY(_set_sdram_srfs)
	/*  set the sdram to self refresh mode */
	P0.H = hi(EBIU_SDGCTL);
	P0.L = lo(EBIU_SDGCTL);
	R2 = [P0];
	R3.H = hi(SRFS);
	R3.L = lo(SRFS);
	R2 = R2|R3;
	[P0] = R2;
	ssync;
	RTS;

ENTRY(_unset_sdram_srfs)
	/*  set the sdram out of self refresh mode */
	P0.H = hi(EBIU_SDGCTL);
	P0.L = lo(EBIU_SDGCTL);
	R2 = [P0];
	R3.H = hi(SRFS);
	R3.L = lo(SRFS);
	R3 = ~R3;
	R2 = R2&R3;
	[P0] = R2;
	ssync;
	RTS;

ENTRY(_set_sic_iwr)
	P0.H = hi(SIC_IWR);
	P0.L = lo(SIC_IWR);
	[P0] = R0;
	SSYNC;
	RTS;

ENTRY(_set_rtc_istat)
	P0.H = hi(RTC_ISTAT);
	P0.L = lo(RTC_ISTAT);
	w[P0] = R0.L;
	SSYNC;
	RTS;

ENTRY(_test_pll_locked)
	P0.H = hi(PLL_STAT);
	P0.L = lo(PLL_STAT);
1:
	R0 = W[P0] (Z);
	CC = BITTST(R0,5);
	IF !CC JUMP 1b;
	RTS;
#endif
