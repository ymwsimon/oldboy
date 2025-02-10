/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:45:49 by mayeung           #+#    #+#             */
/*   Updated: 2025/02/07 21:57:21 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

int	update_frame(t_emu *emu, double delta_time)
{
	static t_ull	n_instr = 0;
	t_ull			num_clock_cycle;

	// time_diff = calculate_time_diff(emu->genesis_tick) - ((emu->clock_cycle / (double)(FREQUENCY - 1)) + emu->sec_elapsed);
	num_clock_cycle = ((FREQUENCY / 4) * delta_time);
	// if ((1 - calculate_time_diff(emu->zero_tick) < 1.0 / FPS && calculate_time_diff(emu->zero_tick) < 1.0))
	// 	// || (1 - calculate_time_diff(emu->zero_tick) < 2.0 / FPS && calculate_time_diff(emu->zero_tick) < 1.0))// && (emu->clock_cycle % 2)))
	// if (emu->sec_elapsed + 1 - calculate_time_diff(emu->genesis_tick) < 1.0 / FPS)
		// num_clock_cycle = ceil((FREQUENCY - emu->clock_cycle) / 4.0);
	if (SDL_GetAudioStreamQueued(emu->audio_stream) < 8192 / 2)
		num_clock_cycle *= 1.2;
	if (SDL_GetAudioStreamQueued(emu->audio_stream) > 8192 * 4)
		num_clock_cycle *= 0.5;
	// {
	// 	num_clock_cycle = ((FREQUENCY - emu->clock_cycle) / 4.0);
	// 	// num_clock_cycle += (FREQUENCY / 4) * emu->last_render_time;
	// 	emu->last_render_time = 0;
	// }
	n_instr = 0;
	while (n_instr < num_clock_cycle)
		n_instr += cpu_step(emu);
	// n_instr = n_instr - num_clock_cycle;
	// printf("n_instr:%llu %llu\n", n_instr, n_instr - num_clock_cycle);
	// printf("n cycle:%llu n_instr:%llu\n", num_clock_cycle, n_instr);
	return (OK);
}

void	emu_tick(t_emu *emu, t_ull clock_cycle)
{
	t_ull	i;

	i = 0;
	while (i < clock_cycle)
	{
		timer_tick(emu);
		apu_tick(emu);
		serial_tick(emu);
		input_tick(emu);
		rtc_tick(emu);
		ppu_tick(emu);
		++(emu->clock_cycle);
		if (emu->clock_cycle + 1 == FREQUENCY)
		{
			++(emu->sec_elapsed);
			printf("%f %llu\n", calculate_time_diff(emu->genesis_tick), emu->sec_elapsed);
			emu->clock_cycle = 0;
		}
		++i;
	}
}

int	init_emu(t_emu *emu)
{
	emu->clock_cycle = 0;
	emu->clock_scale = 1;
	gettimeofday(&emu->last_tick, NULL);
	emu->paused = FALSE;
	emu->quit = FALSE;
	emu->interrupt_enable = 0;
	emu->interrupt_flag = 0xE1;
	emu->last_render_time = 0;
	emu->print_log = PRINT_CPU_LOG;
	emu->sec_elapsed = 0;
	init_cpu(&emu->cpu);
	init_ppu(emu);
	init_timer(emu);
	init_serial(emu);
	init_input(emu);
	init_cart(emu);
	init_apu(emu);
	bzero(&emu->vram, 0x2000);
	bzero(&emu->wram, 0x2000);
	bzero(&emu->hram, 0x7F);
	return (OK);
}
