/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interrupt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:16:41 by mayeung           #+#    #+#             */
/*   Updated: 2024/12/10 14:18:40 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	process_interrupt(t_emu *emu)
{
	static t_word	iaddr[8] = {0x40, 0x48, 0x50, 0x58, 0x60, 0, 0, 0};
	t_byte			idx;
	t_word			pc;

	di(emu, 0);
	emu_tick(emu, 8);
	pc = emu->cpu.pc;
	--(emu->cpu.sp);
	bus_write(emu, emu->cpu.sp, pc >> 8 & 0xFF);
	emu_tick(emu, 4);
	idx = 0;
	while (!(emu->interrupt_enable & emu->interrupt_flag & (1 << idx)))
		++idx;
	emu->cpu.pc = iaddr[idx];
	emu->interrupt_flag &= ~(1 << idx);
	--(emu->cpu.sp);
	bus_write(emu, emu->cpu.sp, pc & 0xFF);
	emu_tick(emu, 4);
	emu_tick(emu, 4);
}
