/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:44:44 by mayeung           #+#    #+#             */
/*   Updated: 2024/12/17 21:54:32 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	init_timer(t_emu *emu)
{
	emu->timer.div = 0xABCC;
	emu->timer.tima = 0;
	emu->timer.tma = 0;
	emu->timer.tac = 0;
	emu->timer.interrupt_countdown = 0;
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
		return (0xF8 | emu->timer.tac);
	return (0xFF);
}

void	timer_write(t_emu *emu, t_word addr, t_byte data)
{
	if (addr == 0xFF04)
	{
		emu->timer.p_div = emu->timer.div;
		emu->timer.div = 0;
	}
	else if (addr == 0xFF05)
		emu->timer.tima = data;
	else if (addr == 0xFF06)
		emu->timer.tma = data;
	else if (addr == 0xFF07)
		emu->timer.tac = 0xF8 | data;
}

int	is_timer_enabled(t_emu *emu)
{
	return (emu->timer.tac & 4);
}

int	need_to_add_tima(t_emu *emu)
{
	static t_word	bit_pos[4] = {10, 4, 6, 8};
	t_word			p_div;
	t_word			div;
	t_byte			clock_setting;

	p_div = emu->timer.div - 1;
	div = emu->timer.div;
	clock_setting = emu->timer.tac & 3;
	if (emu->timer.p_div)
	{
		p_div = emu->timer.p_div;
		emu->timer.p_div = 0;
		div = 0;
		return ((div ^ p_div)
			& (1 << (bit_pos[clock_setting] - 1)));
	}
	return ((div ^ p_div)
		& (1 << bit_pos[clock_setting]));
}

void	timer_tick(t_emu *emu)
{
	++(emu->timer.div);
	if (is_timer_enabled(emu))
	{
		if (need_to_add_tima(emu))
		{
			++(emu->timer.tima);
			if (emu->timer.tima == 0)
			{
				emu->timer.tima = emu->timer.tma;
				emu->interrupt_flag |= 4;
			}
		}
	}
}
