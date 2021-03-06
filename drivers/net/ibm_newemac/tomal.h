/*
 * drivers/net/ibm_newemac/mal.h
 *
 * Memory Access Layer (MAL) support
 *
 * Copyright (c) 2004, 2005 Zultys Technologies.
 * Eugene Surovegin <eugene.surovegin@zultys.com> or <ebs@ebshome.net>
 *
 * Based on original work by
 *      Armin Kuster <akuster@mvista.com>
 *      Copyright 2002 MontaVista Softare Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#ifndef __IBM_NEWEMAC_TOMAL_H
#define __IBM_NEWEMAC_TOMAL_H

/* TOMAL CFG bits */
#define TOMAL_CFG			0
#define   TOMAL_CFG_RX_MAC(x)		(0x00800000 >> (x))
#define   TOMAL_CFG_TX_MAC(x)		(0x00200000 >> (x))
#define   TOMAL_CFG_PLB_FREQ_250	0x00000000
#define   TOMAL_CFG_PLB_FREQ_300	0x00040000
#define   TOMAL_CFG_PLB_FREQ_350	0x00080000
#define   TOMAL_CFG_PLB_FREQ_400	0x000c0000
#define   TOMAL_CFG_PLB_M_POWER		0x00000080
#define   TOMAL_CFG_SLEEP		0x00000002
#define   TOMAL_CFG_SOFT_RESET		0x00000001

#define TOMAL_CONSUMER_BASE_ADDR	0x200
#define TOMAL_PACKET_DATA_ENGINE	0x400
#define   TOMAL_PDE_RX_PREFETCH_1	0
#define   TOMAL_PDE_RX_PREFETCH_8	0x30
#define   TOMAL_PDE_TX_PREFETCH_1	0
#define   TOMAL_PDE_TX_PREFETCH_8	0x03

#define TOMAL_TX_BASE(x)		(0x600 + (x) * 0x100)
#define TOMAL_TX_NOTIFY			0x00 + TOMAL_TX_BASE(0)
#define   TOMAL_TX_NOTIFY_COUNTER(x)	(0x00000020 >> (x))
#define TOMAL_TX_MIN_TIMER(x)		0x10 + TOMAL_TX_BASE(x)
#define TOMAL_TX_MAX_TIMER(x)		0x20 + TOMAL_TX_BASE(x)
#define TOMAL_TX_FRAME_PER_SERV(x)	0x50 + TOMAL_TX_BASE(x)
#define TOMAL_TX_CURR_DESC_HIGH(x)	0x60 + TOMAL_TX_BASE(x)
#define TOMAL_TX_CURR_DESC_LOW(x)	0x70 + TOMAL_TX_BASE(x)
#define TOMAL_TX_PEND_FRAMES(x)		0x90 + TOMAL_TX_BASE(x)
#define TOMAL_TX_POSTED_FRAMES(x)	0xa0 + TOMAL_TX_BASE(x)
#define TOMAL_TX_TRANSMITTED_FRAMES(x)	0xb0 + TOMAL_TX_BASE(x)
#define TOMAL_TX_MAX_FRAME(x)		0xc0 + TOMAL_TX_BASE(x)
#define TOMAL_TX_MIN_FRAME(x)		0xd0 + TOMAL_TX_BASE(x)
#define TOMAL_TX_STATUS(x)		0xe0 + TOMAL_TX_BASE(x)
#define TOMAL_TX_MASK(x)		0xf0 + TOMAL_TX_BASE(x)

#define TOMAL_RX_BASE(x)		(0xf00 + (x) * 0x100)
#define TOMAL_RX_NOTIFY			0x00 + TOMAL_RX_BASE(0)
#define   TOMAL_RX_NOTIFY_COUNTER(x)	(0x00000080 >> (x))
#define TOMAL_RX_MIN_TIMER(x)		0x10 + TOMAL_RX_BASE(x)
#define TOMAL_RX_MAX_TIMER(x)		0x20 + TOMAL_RX_BASE(x)
#define TOMAL_RX_CURR_DESC_HIGH(x)	0x120 + TOMAL_RX_BASE(x)
#define TOMAL_RX_CURR_DESC_LOW(x)	0x130 + TOMAL_RX_BASE(x)
#define TOMAL_RX_ADD_FREE_BYTES(x)	0x140 + TOMAL_RX_BASE(x)
#define TOMAL_RX_TOTAL_BYTES(x)		0x150 + TOMAL_RX_BASE(x)
#define TOMAL_RX_RECEIVED_FRAMES(x)	0x160 + TOMAL_RX_BASE(x)
#define TOMAL_RX_DROPPED_FRAMES(x)	0x170 + TOMAL_RX_BASE(x)
#define TOMAL_RX_MAX_FRAME(x)		0x180 + TOMAL_RX_BASE(x)
#define TOMAL_RX_MIN_FRAME(x)		0x190 + TOMAL_RX_BASE(x)
#define TOMAL_RX_STATUS(x)		0x1a0 + TOMAL_RX_BASE(x)
#define TOMAL_RX_MASK(x)		0x1b0 + TOMAL_RX_BASE(x)
#define   TOMAL_RX_CODE_ERROR		0x00001000
#define   TOMAL_RX_PARITY_ERROR		0x00000400
#define   TOMAL_RX_OVERRUN		0x00000200
#define   TOMAL_RX_PAUSE_FRAME		0x00000100
#define   TOMAL_RX_BAD_FRAME		0x00000080
#define   TOMAL_RX_RUNT_FRAME		0x00000040
#define   TOMAL_RX_SHORT_EVENT		0x00000020
#define   TOMAL_RX_ALIGN_ERROR		0x00000010
#define   TOMAL_RX_BAD_FCS		0x00000008
#define   TOMAL_RX_FRAME_TOO_LONG	0x00000004
#define   TOMAL_RX_OUT_RANGE_ERROR	0x00000002
#define   TOMAL_RX_IN_RANGE_ERROR	0x00000001

#define TOMAL_SWERR_STATUS(c)		(0x1800 + (c) * 0x30)
#define   TOMAL_SWERR_TX_BAD_DESC	0x00000010
#define   TOMAL_SWERR_RX_TOO_SHORT	0x00000001
#define   TOMAL_SWERR_EVENTS		(TOMAL_SWERR_TX_BAD_DESC)
#define TOMAL_SWERR_ENABLE(c)		(0x1810 + (c) * 0x30)
#define TOMAL_SWERR_IRQMASK(c)		(0x1820 + (c) * 0x30)

#define TOMAL_RX_DATABUF_FREE		(0x1900)
#define TOMAL_TX_DATABUF_FREE(c)	(0x1910 + (c) * 0x10)

#define TOMAL_RX_MAC_STATUS(c)		(0x1b20 + (c) * 0x80)
#define TOMAL_RX_MAC_STATUS_ENABLE(c)	(0x1b30 + (c) * 0x80)
#define TOMAL_RX_MAC_STATUS_IRQMASK(c)	(0x1b40 + (c) * 0x80)
#define   TOMAL_RX_MAC_CODE_ERROR	0x00001000
#define   TOMAL_RX_MAC_PARITY_ERROR	0x00000400
#define   TOMAL_RX_MAC_OVERRUN		0x00000200
#define   TOMAL_RX_MAC_PAUSE_FRAME	0x00000100
#define   TOMAL_RX_MAC_BAD_FRAME	0x00000080
#define   TOMAL_RX_MAC_RUNT_FRAME	0x00000040
#define   TOMAL_RX_MAC_SHORT_EVENT	0x00000020
#define   TOMAL_RX_MAC_ALIGN_ERROR	0x00000010
#define   TOMAL_RX_MAC_BAD_FCS		0x00000008
#define   TOMAL_RX_MAC_FRAME_TOO_LONG	0x00000004
#define   TOMAL_RX_MAC_OUT_RANGE_ERROR	0x00000002
#define   TOMAL_RX_MAC_IN_RANGE_ERROR	0x00000001
#define   TOMAL_RX_MAC_MASK (TOMAL_RX_MAC_CODE_ERROR | \
	TOMAL_RX_MAC_PARITY_ERROR | TOMAL_RX_MAC_OVERRUN | \
	TOMAL_RX_MAC_PAUSE_FRAME | TOMAL_RX_MAC_BAD_FRAME | \
	TOMAL_RX_MAC_RUNT_FRAME | TOMAL_RX_MAC_BAD_FCS | \
	TOMAL_RX_MAC_FRAME_TOO_LONG | TOMAL_RX_MAC_OUT_RANGE_ERROR | \
	TOMAL_RX_MAC_IN_RANGE_ERROR)

#define TOMAL_TX_MAC_STATUS(c)		(0x1b50 + (c) * 0x80)
#define TOMAL_TX_MAC_STATUS_ENABLE(c)	(0x1b60 + (c) * 0x80)
#define TOMAL_TX_MAC_STATUS_IRQMASK(c)	(0x1b70 + (c) * 0x80)
#define   TOMAL_TX_MAC_LOCAL_FAULT	0x00001000
#define   TOMAL_TX_MAC_REMOTE_FAULT	0x00000800
#define   TOMAL_TX_MAC_BAD_FCS		0x00000200
#define   TOMAL_TX_MAC_PARITY_ERROR	0x00000100
#define   TOMAL_TX_MAC_LOST_CARRIER	0x00000080
#define   TOMAL_TX_MAC_EXC_DEFERRAL	0x00000040
#define   TOMAL_TX_MAC_EXC_COLLISION	0x00000020
#define   TOMAL_TX_MAC_LATE_COLLISION	0x00000010
#define   TOMAL_TX_MAC_UNDERRUN		0x00000002
#define   TOMAL_TX_MAC_SQE		0x00000001
#define   TOMAL_TX_MAC_MASK (TOMAL_TX_MAC_LOCAL_FAULT | \
	TOMAL_TX_MAC_REMOTE_FAULT | TOMAL_TX_MAC_PARITY_ERROR | \
	TOMAL_TX_MAC_UNDERRUN)

#define TOMAL_HWERR_STATUS		0x1e00
#define   TOMAL_HWERR_IRAPE		8
#define   TOMAL_HWERR_ORAPE		4
#define   TOMAL_HWERR_IDBPE		2
#define   TOMAL_HWERR_ODBPE		1
#define   TOMAL_HWERR_MASK (TOMAL_HWERR_IRAPE | TOMAL_HWERR_ORAPE |\
			    TOMAL_HWERR_IDBPE | TOMAL_HWERR_ODBPE)
#define TOMAL_HWERR_ENABLE		0x1e10
#define TOMAL_HWERR_IRQMASK		0x1e20

#define TOMAL_CRITERR_STATUS		0x1f00
#define TOMAL_CRITERR_ENABLE		0x1f10
#define  TOMAL_CRITERR_TXDESC		2
#define  TOMAL_CRITERR_RXDESC		1
#define  TOMAL_CRITERR_MASK (TOMAL_CRITERR_TXDESC | TOMAL_CRITERR_RXDESC)
#define TOMAL_CRITERR_IRQMASK		0x1f20
#define TOMAL_CRITERR_BAD_RXDESC	0x1f30
#define TOMAL_CRITERR_BAD_TXDESC	0x1f40

#define TOMAL_IRQ_STATUS		0x1f80
#define   TOMAL_IRQ_TX(c)		(0x80000000 >> (15 - c))
#define   TOMAL_IRQ_RX(c)		(0x80000000 >> (23 - c))
#define   TOMAL_IRQ_TX_MACERR(c)	(0x80000000 >> (25 - c))
#define   TOMAL_IRQ_RX_MACERR(c)	(0x80000000 >> (27 - c))
#define   TOMAL_IRQ_SW_ERR(c)		(0x80000000 >> (30 - c))
#define   TOMAL_IRQ_CRITICAL_ERR	0x1
#define TOMAL_IRQ_ROUTE			0x1f90

#define TOMAL_BAD_STATUS_COUNT		0x2060


#endif  /* __IBM_NEWEMAC_TOMAL_H */
