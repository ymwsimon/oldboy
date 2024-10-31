/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:44:44 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/31 12:11:29 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	init_timer(t_emu *emu)
{
	emu->timer.div = 0;
	emu->timer.tima = 0;
	emu->timer.tma = 0;
	emu->timer.tac = 0;
}

t_byte	timer_read(t_emu *emu, t_word addr)
{
	if (addr == 0xFF04)
		return (emu->timer.div >> 8);
	if (addr == 0xFF05)
		return (emu->timer.tima);
	if (addr == 0xFF06)
		return (emu->timer.tma);
	if (addr == 0xFF07)
		return (emu->timer.tac);
	return (0xFF);
}

void	timer_write(t_emu *emu, t_word addr, t_byte data)
{
	if (addr == 0xFF04)
		emu->timer.div = 0;
	else if (addr == 0xFF05)
		emu->timer.tma = data;
	else if (addr == 0xFF06)
		emu->timer.tima = data;
	else if (addr == 0xFF07)
		emu->timer.tac = data;
}

void	timer_tick(t_emu *emu)
{
	static t_word	n_cycle[4] = {256 * 4, 4 * 4, 16 * 4, 64 * 4};

	++(emu->timer.div);
	if (emu->timer.tac & 0x4)
	{
		if (!(emu->timer.div % n_cycle[emu->timer.tac & 0x3]))
			++(emu->timer.tima);
		if (!emu->timer.tima)
		{
			emu->timer.tima = emu->timer.tma;
			emu->interrupt_flag |= 0x4;
		}
	}
}
