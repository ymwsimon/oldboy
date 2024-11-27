/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:44:44 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/27 12:14:01 by mayeung          ###   ########.fr       */
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

int	need_to_add_tima(t_emu *emu)//, t_word p_div, t_word div)
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
	// if (emu->timer.interrupt_countdown)
	// {
	// 	--(emu->timer.interrupt_countdown);
	// 	if (!emu->timer.interrupt_countdown)
	// 		emu->interrupt_flag |= 4;
	// }
	if (is_timer_enabled(emu))
	{
		if (need_to_add_tima(emu))
		{
			++(emu->timer.tima);
			if (emu->timer.tima == 0)
			{
				emu->timer.tima = emu->timer.tma;
				// emu->timer.interrupt_countdown = 4;
				emu->interrupt_flag |= 4;
			}
		}
	}
}

	// t_word			p_div;
	// t_word			div;
	// t_byte			clock_setting;
	// static t_byte	n_bit_shift[4] = {9, 3, 5, 7};
	// int				need_update_tima;

	// p_div = emu->timer.div;
	// ++(emu->timer.div);
	// div = emu->timer.div;
	// if (is_timer_enabled(emu))
	// {
	// 	clock_setting = emu->timer.tac & 3;
	// 	need_update_tima = (p_div & (1 << n_bit_shift[clock_setting]))
	// 		&& (!(div & (1 << n_bit_shift[clock_setting])));
	// 	if (need_update_tima)
	// 	{
	// 		if (emu->timer.tima == 0xFF)
	// 		{
	// 			emu->interrupt_flag |= 4;
	// 			emu->timer.tima = emu->timer.tma;
	// 		}
	// 		else
	// 			++(emu->timer.tima);
	// 	}
	// }

	// static t_word	freq_divider[] = {1024, 16, 64, 256};
	// static t_byte	tima_overflow = FALSE;
	// t_word			div;
	// t_word			freq;
	// t_byte			increase_tima;

	// div = emu->timer.div;
	// ++(emu->timer.div);
	// if (tima_overflow)
	// {
	// 	tima_overflow = FALSE;
	// 	emu->interrupt_flag |= 4;
	// 	emu->timer.tima = emu->timer.tma;
	// }
	// freq = freq_divider[emu->timer.tac & 3] / 4;
	// increase_tima = ((div + 1) / freq) - (div / freq);
	// if (increase_tima && (emu->timer.tac & 4))
	// {
	// 	if (emu->timer.tima == 0xFF)
	// 	{
	// 		emu->timer.tima = 0;
	// 		tima_overflow = TRUE;
	// 	}
	// 	else
	// 		++(emu->timer.tima);
	// }
// void timer_ticks(u8 ticks)
// {
//     u16 div = g_timer.div;
//     u8 tac = read_timer(TIMER_TAC);

//     // update DIV's 16bit value
//     g_timer.div += ticks;

//     // delayed IE
//     if (g_cpu.ime_scheduled) {
//         interrupt_set_ime(true);
//         g_cpu.ime_scheduled = false;
//     }

//     // TIMA overflowed during the last cycle
//     if (g_tima_overflow) {
//         g_tima_overflow = false;
//         interrupt_request(IV_TIMA);
//         g_timer.tima = g_timer.tma;
//     }

//     // We only update the timer's value at certain frequencies (freq_divider)
//     // Here we compute the number of 'freq' between the old div and the new div
//     // (in clocks, no cycles ! Hence we divide by 4)
//     u16 freq = g_freq_divider[tac & 0x03] / 4;
//     u8 increase_tima = ((div + ticks) / freq) - (div / freq);

//     // If bit 2 of TAC is set to 0 then the timer is disabled
//     if (increase_tima && tac & 0x4) {
//         u8 tima = read_timer(TIMER_TIMA);

//         if (tima == 0xFF) { // overflow
//             // Timer interrupt is delayed 1 cycle (4 clocks) from the TIMA
//             // overflow. The TMA reload to TIMA is also delayed. For one cycle,
//             // after overflowing TIMA, the value in TIMA is 00h, not TMA.
//             write_timer(TIMER_TIMA, 0x00);
//             g_tima_overflow = true;
//         } else {
//             write_timer(TIMER_TIMA, tima + increase_tima);
//         }
//     }
// }