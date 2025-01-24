/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:45:49 by mayeung           #+#    #+#             */
/*   Updated: 2025/01/24 15:21:46 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	apu_callback(void *userdata, SDL_AudioStream *stream,
	int additional_amount, int total_amount)
{
	t_emu	*emu;
	int		i;
	t_word	n_sample;
	t_word	used_sample;
	// double	time;
	float	*data;

	(void)total_amount;
	emu = (t_emu *)userdata;
	i = 0;
	// printf("%d %f\n", additional_amount, additional_amount / sizeof(float) / 2 * 64 / (FREQUENCY * 1.0));
	update_frame(emu, additional_amount / sizeof(float) / 2 * 64 / (FREQUENCY * 1.0));
	data = malloc(additional_amount);
	n_sample = 0;
	used_sample = 0;
	while (n_sample < additional_amount / sizeof(float) / 2)
	{
		i = 0;
		data[n_sample * 2] = 0;
		while (i < 16)
		{
			data[n_sample * 2] += emu->apu.audio_buff[(emu->apu.audio_buff_idx + used_sample + i) % AUDIO_BUFFER_SIZE];
			++i;
		}
		used_sample += i;
		data[n_sample * 2] /= i;
		data[n_sample * 2 + 1] = data[n_sample * 2];
		++n_sample;
	}
	emu->apu.audio_buff_idx = (emu->apu.audio_buff_idx + used_sample) % AUDIO_BUFFER_SIZE;
	emu->apu.audio_buff_len -= used_sample;
	SDL_PutAudioStreamData(stream, data, additional_amount);
	free(data);
}

void	get_data_callback(void *userdata, SDL_AudioStream *stream,
	int additional_amount, int total_amount)
{
	t_emu	*emu;

	emu = (t_emu *)userdata;
	(void)userdata;
	(void)additional_amount;
	(void)stream;
	(void)total_amount;
	(void)emu;
	SDL_LockMutex(emu->mod);
	// emu->byte_read += additional_amount;
	if (emu->byte_available > total_amount)
		emu->byte_available -= total_amount;
	else
		emu->byte_available = 0;
	SDL_UnlockMutex(emu->mod);
	// printf("getting data %f add:%d total:%d\n", calculate_time_diff(emu->genesis_tick), additional_amount, total_amount);
}

int	update_frame(t_emu *emu, double delta_time)
{
	static t_ull	n_instr = 0;
	t_ull			num_clock_cycle;
	double			time_diff;

	(void)time_diff;
	// if (SDL_GetAudioStreamAvailable(emu->audio_stream) > 4000)
	// if (SDL_GetAudioStreamQueued(emu->audio_stream) > 3000)
	time_diff = calculate_time_diff(emu->genesis_tick) - ((emu->clock_cycle / (double)(FREQUENCY - 1)) + emu->sec_elapsed);
	// SDL_LockMutex(emu->mod);
	// emu->byte_read += additional_amount;
	// if (emu->byte_available > 14000)
	// 	delta_time *= 0;
	// SDL_UnlockMutex(emu->mod);
	// if ((((double)emu->byte_written) / ((double)emu->byte_read)) > 1.5)
	// {
	// 	// printf("too much %llu %llu %lld %f\n", emu->byte_written, emu->byte_read, (long)emu->byte_written - emu->byte_read, (((double)emu->byte_written) / ((double)emu->byte_read)));
	// 	delta_time *= 0.9;
	// 	// return (0);
	// }
	// else
	// 	delta_time *= 1.05;
	// if (time_diff > 0.005)
	// 	delta_time += time_diff;
	// if (time_diff < -0.005)
	// 	delta_time += time_diff;
	// if (SDL_GetAudioStreamAvailable(emu->audio_stream) > 4000)
	// if (SDL_GetAudioStreamQueued(emu->audio_stream) > 30000)
		// delta_time = 0;
	num_clock_cycle = ((FREQUENCY / 4) * delta_time);
	// if ((1 - calculate_time_diff(emu->zero_tick) < 1.0 / FPS && calculate_time_diff(emu->zero_tick) < 1.0))
	// 	// || (1 - calculate_time_diff(emu->zero_tick) < 2.0 / FPS && calculate_time_diff(emu->zero_tick) < 1.0))// && (emu->clock_cycle % 2)))
	if (emu->sec_elapsed + 1 - calculate_time_diff(emu->genesis_tick) < 1.0 / FPS)
		num_clock_cycle = ceil((FREQUENCY - emu->clock_cycle) / 4.0);
	// {
	// 	num_clock_cycle = ((FREQUENCY - emu->clock_cycle) / 4.0);
	// 	// num_clock_cycle += (FREQUENCY / 4) * emu->last_render_time;
	// 	emu->last_render_time = 0;
	// }
	n_instr = 0;
	while (n_instr < num_clock_cycle)
	{
		n_instr += cpu_step(emu);

	}
	// n_instr = n_instr - num_clock_cycle;
	// printf("n_instr:%llu %llu\n", n_instr, n_instr - num_clock_cycle);
	// printf("n cycle:%llu n_instr:%llu\n", num_clock_cycle, n_instr);
	return (OK);
}

void	emu_tick(t_emu *emu, t_ull clock_cycle)
{
	t_ull	i;
	double	td;

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
		(void)td;
		// if (emu->clock_cycle == FREQUENCY)
		// // if (!(emu->clock_cycle % 2048) && emu->clock_cycle)
		// {
		// 	// td = calculate_time_diff(emu->zero_tick);
		// 	// SDL_SetAudioStreamFrequencyRatio(emu->audio_stream, ((float)emu->clock_cycle / FREQUENCY) / td);
		// 	// printf("td:%f ratio:%f\n", td, ((float)emu->clock_cycle / FREQUENCY) / td);
		// }
		if (emu->clock_cycle + 1 == FREQUENCY)
		{
			++(emu->sec_elapsed);
			td = calculate_time_diff(emu->zero_tick);
			gettimeofday(&emu->zero_tick, NULL);
			emu->last_render_time = 0;
			if (td > 1.0)
				emu->last_render_time = td - 1.0;
			// printf("%f %f %llu %f wr:%llu read:%llu diff:%lld ratio:%f \n", td, calculate_time_diff(emu->genesis_tick), emu->sec_elapsed, (double)SDL_GetPerformanceCounter()/SDL_GetPerformanceFrequency(), emu->byte_written, emu->byte_read, (long)emu->byte_written - emu->byte_read, (double)emu->byte_written / emu->byte_read);
			printf("%f %f %llu %f\n", td, calculate_time_diff(emu->genesis_tick), emu->sec_elapsed, (double)SDL_GetPerformanceCounter()/SDL_GetPerformanceFrequency());
			// printf("%f %f %llu %f wr:%llu read:%llu diff:%lld ratio:%f available:%llu\n", td, calculate_time_diff(emu->genesis_tick), emu->sec_elapsed, (double)SDL_GetPerformanceCounter()/SDL_GetPerformanceFrequency(), emu->byte_written, emu->byte_read, (long)emu->byte_written - emu->byte_read, (double)emu->byte_written / emu->byte_read, emu->byte_available);
			// SDL_SetAudioStreamFrequencyRatio(emu->audio_stream, ((float)emu->clock_cycle / FREQUENCY) / td);
			// SDL_SetAudioStreamFrequencyRatio(emu->audio_stream, 1);
			emu->clock_cycle = 0;

		}
		// while (SDL_GetAudioStreamQueued(emu->audio_stream) > 40000)
			// ;
		++i;
	}
}

int	init_emu(t_emu *emu)
{
	emu->clock_cycle = 0;
	emu->clock_scale = 1;
	gettimeofday(&emu->last_tick, NULL);
	gettimeofday(&emu->zero_tick, NULL);
	emu->paused = FALSE;
	emu->quit = FALSE;
	emu->interrupt_enable = 0;
	emu->interrupt_flag = 0xE1;
	emu->last_render_time = 0;
	emu->print_log = PRINT_CPU_LOG;
	emu->genesis_tick.tv_sec = 0;
	emu->genesis_tick.tv_usec = 0;
	emu->sec_elapsed = 0;
	emu->byte_written = 0;
	emu->byte_read = 0;
	emu->byte_available = 0;
	emu->mod = SDL_CreateMutex();
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
	// SDL_SetAudioStreamGetCallback(emu->audio_stream, get_data_callback, emu);
	return (OK);
}
