/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interrupt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:16:41 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/24 13:52:51 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	process_interrupt(t_emu *emu)
{
	static t_word	iaddr[5] = {0x40, 0x48, 0x50, 0x58, 0x60};
	t_byte			idx;

	idx = 0;
	while (!(emu->interrupt_enable & emu->interrupt_flag & (1 << idx)))
		++idx;
	di(emu, 0);
	emu->interrupt_flag &= ~(1 << idx);
	emu_tick(emu, 8);
	push_word(emu, emu->cpu.pc);
	emu->cpu.pc = iaddr[idx];
	emu_tick(emu, 4);
}
