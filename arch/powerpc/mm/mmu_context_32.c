/*
 * This file contains the routines for handling the MMU on those
 * PowerPC implementations where the MMU substantially follows the
 * architecture specification.  This includes the 6xx, 7xx, 7xxx,
 * 8260, and POWER3 implementations but excludes the 8xx and 4xx.
 *  -- paulus
 *
 *  Derived from arch/ppc/mm/init.c:
 *    Copyright (C) 1995-1996 Gary Thomas (gdt@linuxppc.org)
 *
 *  Modifications by Paul Mackerras (PowerMac) (paulus@cs.anu.edu.au)
 *  and Cort Dougan (PReP) (cort@cs.nmt.edu)
 *    Copyright (C) 1996 Paul Mackerras
 *  Amiga/APUS changes by Jesper Skov (jskov@cygnus.co.uk).
 *
 *  Derived from "arch/i386/mm/init.c"
 *    Copyright (C) 1991, 1992, 1993, 1994  Linus Torvalds
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
 */

#include <linux/mm.h>
#include <linux/init.h>

#include <asm/mmu_context.h>
#include <asm/tlbflush.h>

unsigned long next_mmu_context;
unsigned long context_map[LAST_CONTEXT / BITS_PER_LONG + 1];
#ifdef FEW_CONTEXTS
spinlock_t context_lock;
atomic_t nr_free_contexts;
struct mm_struct *context_mm[LAST_CONTEXT+1];
DEFINE_PER_CPU(unsigned long, current_context);
void steal_context(void);
#endif /* FEW_CONTEXTS */

/*
 * Initialize the context management stuff.
 */
void __init
mmu_context_init(void)
{
	/*
	 * Some processors have too few contexts to reserve one for
	 * init_mm, and require using context 0 for a normal task.
	 * Other processors reserve the use of context zero for the kernel.
	 * This code assumes FIRST_CONTEXT < 32.
	 */
	context_map[0] = (1 << FIRST_CONTEXT) - 1;
	next_mmu_context = FIRST_CONTEXT;
#ifdef FEW_CONTEXTS
	atomic_set(&nr_free_contexts, LAST_CONTEXT - FIRST_CONTEXT + 1);
#endif /* FEW_CONTEXTS */
}

#ifdef FEW_CONTEXTS
/*
 * Steal a context from a task that has one at the moment.
 * We assume that the system has less processors than MMU contexts so
 * that we are guaranteed to find an unused context.  In the worst
 * case we need online-CPUs - 1 tests; not scaling but ok for small
 * numbers of CPUs.
 * We also assume that this is only used on systems that don't
 * use an MMU hash table - this is true for 8xx and 4xx.
 * This isn't an LRU system, it just frees up each context in
 * turn (sort-of pseudo-random replacement :).  This would be the
 * place to implement an LRU scheme if anyone was motivated to do it.
 *  -- paulus
 */
void
steal_context(void)
{
	struct mm_struct *mm;
	int found, cpu;

	/* free up context `next_mmu_context' */
	/* if we shouldn't free context 0, don't... */
	do {
		if (next_mmu_context < FIRST_CONTEXT)
			next_mmu_context = FIRST_CONTEXT;

		found = 1;

		for_each_online_cpu(cpu)
			if (per_cpu(current_context, cpu) == next_mmu_context) {
				next_mmu_context++;
				next_mmu_context &= LAST_CONTEXT;
				found = 0;
				break;
			}
	} while (!found);

	mm = context_mm[next_mmu_context];
	flush_tlb_mm(mm);
	destroy_context(mm);

	/* some book-keeping to reduce remote flushes */
	cpus_clear(mm->cpu_vm_mask);
}
#endif /* FEW_CONTEXTS */
