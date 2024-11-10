/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:45:49 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/09 21:58:22 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

int	update_frame(t_emu *emu, double delta_time)
{
	t_ull	num_clock_cycle;
	t_ull	i;

	num_clock_cycle = FREQUENCY * delta_time * emu->clock_scale;
	i = 0;
	while (i < num_clock_cycle)
	{
		if (!(emu->clock_cycle % 4))
			cpu_step(emu);
		++(emu->clock_cycle);
		++i;
	}
	return (OK);
}

void	emu_tick(t_emu *emu, t_ull clock_cycle)
{
	t_ull	i;

	i = 0;
	while (i < clock_cycle)
	{
		// ++(emu->clock_cycle);
		timer_tick(emu);
		serial_tick(emu);
		ppu_tick(emu);
		++i;
	}
}

int	init_emu(t_emu *emu)
{
	emu->clock_cycle = 0;
	emu->clock_scale = 1;
	gettimeofday(&emu->last_tick, NULL);
	emu->paused = FALSE;
	emu->interrupt_enable = 0;
	emu->interrupt_flag = 0;
	emu->last_render_time = 0;
	init_cpu(&emu->cpu);
	init_timer(emu);
	init_serial(emu);
	bzero(&emu->vram, 0x2000);
	bzero(&emu->wram, 0x2000);
	bzero(&emu->hram, 0x7F);
	return (OK);
}
