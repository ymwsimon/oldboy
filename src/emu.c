/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:45:49 by mayeung           #+#    #+#             */
/*   Updated: 2024/12/09 17:23:09 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

int	update_frame(t_emu *emu, double delta_time)
{
	t_ull	num_clock_cycle;
	t_ull	i;

	num_clock_cycle = FREQUENCY / 4 * delta_time * emu->clock_scale;
	i = 0;
	while (i < num_clock_cycle)
	{
		// if (!(emu->clock_cycle % 4))
		cpu_step(emu);
		// ++(emu->clock_cycle);
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
		ppu_tick(emu);
		timer_tick(emu);
		serial_tick(emu);
		input_tick_state(emu);
		++(emu->clock_cycle);
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
	emu->cart.rom_bank0_ptr = emu->cart.data;
	emu->cart.rom_bankx_ptr = emu->cart.data + 0x4000;
	emu->cart.ram_bank_ptr = emu->cart.ram;
	emu->cart.ram_enbaled = FALSE;
	emu->cart.banking_mode = 0;
	emu->print_log = PRINT_CPU_LOG;
	init_cpu(&emu->cpu);
	init_ppu(emu);
	init_timer(emu);
	init_serial(emu);
	init_input(emu);
	bzero(&emu->vram, 0x2000);
	bzero(&emu->wram, 0x2000);
	bzero(&emu->hram, 0x7F);
	bzero(&emu->cart.ram, 0x20000);
	return (OK);
}
