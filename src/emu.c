/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:45:49 by mayeung           #+#    #+#             */
/*   Updated: 2025/01/18 23:19:55 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

int	update_frame(t_emu *emu, double delta_time)
{
	t_ull	n_instr;
	t_ull	num_clock_cycle;

	num_clock_cycle = ceil((FREQUENCY / 4) * delta_time * emu->clock_scale);
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
		apu_tick(emu);
		timer_tick(emu);
		serial_tick(emu);
		input_tick(emu);
		rtc_tick(emu);
		ppu_tick(emu);
		++(emu->clock_cycle);
		if (emu->clock_cycle >= FREQUENCY)
			emu->clock_cycle = 0;
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
	// SDL_SetAudioStreamGetCallback(emu->audio_stream, apu_callback, emu);
	return (OK);
}
