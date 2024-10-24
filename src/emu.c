/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:45:49 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/24 14:48:16 by mayeung          ###   ########.fr       */
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
		cpu_step(emu);
		// emu->clock_cycle++;
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
		++(emu->clock_cycle);
		++i;
	}
}

int	init_emu(t_emu *emu)
{
	emu->clock_cycle = 0;
	emu->clock_scale = CLOCK_SCALE;
	gettimeofday(&emu->last_tick, NULL);
	emu->paused = FALSE;
	init_cpu(&emu->cpu);
	bzero(&emu->vram, 0x2000);
	bzero(&emu->wram, 0x2000);
	bzero(&emu->hram, 0x80);
	return (OK);
}
