/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:10:16 by mayeung           #+#    #+#             */
/*   Updated: 2025/01/17 16:23:10 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	init_apu(t_emu *emu)
{
	emu->apu.nr10_c1_sweep = 0x80;
	emu->apu.nr11_c1_timer_duty = 0;
	emu->apu.nr12_c1_vol_env = 0;
	emu->apu.nr13_c1_per_low = 0;
	emu->apu.nr14_c1_per_high_ctrl = 0x38;
	emu->apu.nr21_c2_timer_duty = 0;
	emu->apu.nr22_c2_vol_env = 0;
	emu->apu.nr23_c2_per_low = 0;
	emu->apu.nr24_c2_per_high_ctrl = 0x38;
	emu->apu.nr30_c3_dac = 0x7F;
	emu->apu.nr31_c3_timer = 0;
	emu->apu.nr32_c3_out_lvl = 0x9F;
	emu->apu.nr33_c3_per_low = 0;
	emu->apu.nr34_c3_per_high_ctrl = 0x38;
	emu->apu.nr41_c4_timer = 0xC0;
	emu->apu.nr42_c4_vol_env = 0;
	emu->apu.nr43_c4_freq_ran = 0;
	emu->apu.nr44_c4_ctrl = 0x3F;
	emu->apu.nr50_mas_vol_vin_pan = 0;
	emu->apu.nr51_sound_pan = 0xFF;
	emu->apu.nr52_mas_ctrl = 0x70;
	emu->apu.audio_buff_idx = 0;
	emu->apu.apu_div = 0;
	emu->apu.ch1_value = 0;
	emu->apu.ch2_value = 0;
	emu->apu.audio_buff_idx = 0;
	emu->apu.audio_buff_len = 0;
	bzero(&emu->apu.wave_ram, 0xF);
}

void	apu_write(t_emu *emu, t_word addr, t_byte data)
{
	if (addr == 0xFF10)
		emu->apu.nr10_c1_sweep = 0x80 | (data & 0x7F);
	if (addr == 0xFF11)
		emu->apu.nr11_c1_timer_duty = data;
	if (addr == 0xFF12)
		emu->apu.nr12_c1_vol_env = data;
	if (addr == 0xFF13)
		emu->apu.nr13_c1_per_low = data;
	if (addr == 0xFF14)
	{
		emu->apu.nr14_c1_per_high_ctrl = 0x38 | (data & 0xC7);
		if (data & 128)
			apu_enable_ch1(emu);
	}
	if (addr == 0xFF16)
		emu->apu.nr21_c2_timer_duty = data;
	if (addr == 0xFF17)
		emu->apu.nr22_c2_vol_env = data;
	if (addr == 0xFF18)
		emu->apu.nr23_c2_per_low = data;
	if (addr == 0xFF19)
	{
		emu->apu.nr24_c2_per_high_ctrl = 0x38 | (data & 0xC7);
		if (data & 128)
			apu_enable_ch2(emu);
	}
	if (addr == 0xFF1A)
		emu->apu.nr30_c3_dac = 0x7F | (data & 0x80);
	if (addr == 0xFF1B)
		emu->apu.nr31_c3_timer = data;
	if (addr == 0xFF1C)
		emu->apu.nr32_c3_out_lvl = 0x9F | (data & 0x60);
	if (addr == 0xFF1D)
		emu->apu.nr33_c3_per_low = data;
	if (addr == 0xFF1E)
		emu->apu.nr34_c3_per_high_ctrl = 0x38 | (data & 0xC7);
	if (addr == 0xFF20)
		emu->apu.nr41_c4_timer = 0xC0 | (data & 0x3F);
	if (addr == 0xFF21)
		emu->apu.nr42_c4_vol_env = data;
	if (addr == 0xFF22)
		emu->apu.nr43_c4_freq_ran = data;
	if (addr == 0xFF23)
		emu->apu.nr44_c4_ctrl = 0x3F | (data & 0xC0);
	if (addr == 0xFF24)
		emu->apu.nr50_mas_vol_vin_pan = data;
	if (addr == 0xFF25)
		emu->apu.nr51_sound_pan = data;
	if (addr == 0xFF26)
		emu->apu.nr52_mas_ctrl = (emu->apu.nr52_mas_ctrl & 0x7F) | (data & 0x80);
	if (addr >= 0xFF30 && addr <= 0xFF3F)
		emu->apu.wave_ram[addr - 0xFF30] = data;
}

t_byte	apu_read(t_emu *emu, t_word addr)
{
	if (addr == 0xFF10)
		return (emu->apu.nr10_c1_sweep);
	if (addr == 0xFF11)
		return (emu->apu.nr11_c1_timer_duty);
	if (addr == 0xFF12)
		return (emu->apu.nr12_c1_vol_env);
	if (addr == 0xFF13)
		return (emu->apu.nr13_c1_per_low);
	if (addr == 0xFF14)
		return (emu->apu.nr14_c1_per_high_ctrl);
	if (addr == 0xFF16)
		return (emu->apu.nr21_c2_timer_duty);
	if (addr == 0xFF17)
		return (emu->apu.nr22_c2_vol_env);
	if (addr == 0xFF18)
		return (emu->apu.nr23_c2_per_low);
	if (addr == 0xFF19)
		return (emu->apu.nr24_c2_per_high_ctrl);
	if (addr == 0xFF1A)
		return (emu->apu.nr30_c3_dac);
	if (addr == 0xFF1B)
		return (emu->apu.nr31_c3_timer);
	if (addr == 0xFF1C)
		return (emu->apu.nr32_c3_out_lvl);
	if (addr == 0xFF1D)
		return (emu->apu.nr33_c3_per_low);
	if (addr == 0xFF1E)
		return (emu->apu.nr34_c3_per_high_ctrl);
	if (addr == 0xFF20)
		return (emu->apu.nr41_c4_timer);
	if (addr == 0xFF21)
		return (emu->apu.nr42_c4_vol_env);
	if (addr == 0xFF22)
		return (emu->apu.nr43_c4_freq_ran);
	if (addr == 0xFF23)
		return (emu->apu.nr44_c4_ctrl);
	if (addr == 0xFF24)
		return (emu->apu.nr50_mas_vol_vin_pan);
	if (addr == 0xFF25)
		return (emu->apu.nr51_sound_pan);
	if (addr == 0xFF26)
		return (emu->apu.nr52_mas_ctrl);
	if (addr >= 0xFF30 && addr <= 0xFF3F)
		return (emu->apu.wave_ram[addr - 0xFF30]);
	return (0xFF);
}

t_byte	is_apu_enabled(t_emu *emu)
{
	return (emu->apu.nr52_mas_ctrl & 0x80);
}

t_word	need_add_apu_div(t_emu *emu)
{
	static t_word	p_div = 0;
	t_word			div;
	t_word			res;

	res = FALSE;
	div = emu->timer.div / 256;
	if ((p_div & 16) && !(div & 16))
		res = TRUE;
	p_div = div;
	return (res);
	// (void)p_div;
	// return (((div ^ p_div) & ((1 << 13))) && (div & (1 << 13)));
}

t_byte	is_apu_ch1_on(t_emu *emu)
{
	return (emu->apu.nr52_mas_ctrl & 1);
}

t_byte	is_apu_ch2_on(t_emu *emu)
{
	return (emu->apu.nr52_mas_ctrl & 2);
}

t_byte	is_apu_ch3_on(t_emu *emu)
{
	return (emu->apu.nr52_mas_ctrl & 4);
}

t_byte	is_apu_ch4_on(t_emu *emu)
{
	return (emu->apu.nr52_mas_ctrl & 8);
}

t_byte	is_apu_ch1_dac_on(t_emu *emu)
{
	return (emu->apu.nr12_c1_vol_env & 0xF8);
}

t_byte	is_apu_ch2_dac_on(t_emu *emu)
{
	return (emu->apu.nr22_c2_vol_env & 0xF8);
}

t_byte	is_apu_ch1_length_timer_on(t_emu *emu)
{
	return (emu->apu.nr14_c1_per_high_ctrl & 0x40);
}

t_byte	is_apu_ch2_length_timer_on(t_emu *emu)
{
	return (emu->apu.nr24_c2_per_high_ctrl & 0x40);
}

t_byte	apu_get_sq_wave_sample(t_byte idx, t_byte form_type)
{
	const t_byte	waveforms[4][8] = {
	{1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 1}};

	return (waveforms[form_type][idx]);
}

t_word	apu_get_ch1_period(t_emu *emu)
{
	return (((emu->apu.nr14_c1_per_high_ctrl & 7) << 8)
		| emu->apu.nr13_c1_per_low);
}

t_word	apu_get_ch2_period(t_emu *emu)
{
	return (((emu->apu.nr24_c2_per_high_ctrl & 7) << 8)
		| emu->apu.nr23_c2_per_low);
}

void	apu_disable_ch1(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl &= ~1;
	emu->apu.ch1_length_timer = 0;
}

void	apu_disable_ch2(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl &= ~2;
	emu->apu.ch2_length_timer = 0;
}

void	apu_disable_ch3(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl &= ~4;
}

void	apu_disable_ch4(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl &= ~8;
}

void	apu_enable_ch1(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl |= 1;
	emu->apu.ch1_length_timer = emu->apu.nr11_c1_timer_duty & 0x3F;
	emu->apu.ch1_period_counter = apu_get_ch1_period(emu);
	emu->apu.ch1_sample_idx = 0;
	emu->apu.ch1_value = 0;
	emu->apu.ch1_sweep_counter = 0;
	emu->apu.ch1_vol = emu->apu.nr12_c1_vol_env >> 4;
	emu->apu.audio_buff_idx = 0;
}

void	apu_enable_ch2(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl |= 2;
	emu->apu.ch2_length_timer = emu->apu.nr21_c2_timer_duty & 0x3F;
	emu->apu.ch2_period_counter = apu_get_ch2_period(emu);
	emu->apu.ch2_sample_idx = 0;
	emu->apu.ch2_value = 0;
	emu->apu.ch2_sweep_counter = 0;
	emu->apu.ch2_vol = emu->apu.nr22_c2_vol_env >> 4;
}

void	apu_enable_ch3(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl |= 4;
}

void	apu_enable_ch4(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl |= 8;
}

float	apu_dac(float max, float min, t_byte sample)
{
	if (sample)
		return (max);
	return (min);
}

void	apu_step_ch1(t_emu *emu)
{
	++(emu->apu.ch1_period_counter);
	if (emu->apu.ch1_period_counter >= 2048)
	{
		emu->apu.ch1_sample_idx = (emu->apu.ch1_sample_idx + 1) % 8;
		emu->apu.ch1_period_counter = apu_get_ch1_period(emu);
	}
	emu->apu.ch1_value = apu_dac(1, -1,
			apu_get_sq_wave_sample(emu->apu.ch1_sample_idx,
				emu->apu.nr11_c1_timer_duty >> 6));
	emu->apu.ch1_value *= ((float)emu->apu.ch1_vol / 15.0);
	// emu->apu.audio_buff[emu->apu.audio_buff_idx++] = emu->apu.ch1_value;
	if (emu->apu.ch1_length_timer == 64)
		apu_disable_ch1(emu);
}

void	apu_step_ch2(t_emu *emu)
{
	++(emu->apu.ch2_period_counter);
	if (emu->apu.ch2_period_counter >= 2048)
	{
		emu->apu.ch2_sample_idx = (emu->apu.ch2_sample_idx + 1) % 8;
		emu->apu.ch2_period_counter = apu_get_ch2_period(emu);
	}
	emu->apu.ch2_value = apu_dac(1, -1,
			apu_get_sq_wave_sample(emu->apu.ch2_sample_idx,
				emu->apu.nr21_c2_timer_duty >> 6));
	emu->apu.ch2_value *= ((float)emu->apu.ch2_vol / 15.0);
	emu->apu.audio_buff[(emu->apu.audio_buff_idx + emu->apu.audio_buff_len)
		% AUDIO_BUFFER_SIZE] = emu->apu.ch2_value;
	++emu->apu.audio_buff_len;
	if (emu->apu.ch2_length_timer == 64)
		apu_disable_ch2(emu);
}

float	apu_buf_avg(t_emu *emu)
{
	float	res;
	t_word	idx;

	res = 0;
	idx = 0;
	while (idx < emu->apu.audio_buff_len)
	{
		res += emu->apu.audio_buff[(emu->apu.audio_buff_idx + idx)
			% AUDIO_BUFFER_SIZE];
		++idx;
	}
	if (idx)
		res /= idx;
	return (res);
}

void apu_callback_old(void *userdata, SDL_AudioStream *stream,
	int additional_amount, int total_amount)
{
	t_ull	i;
	t_emu	*emu;
	float	data[2];
	t_word	buf_idx;
	t_ull	clock_cycle;
	t_ull	clock_idx;
	t_word	n_sample;
	t_word	used_sample;

	i = 0;
	emu = (t_emu *)userdata;
	buf_idx = emu->apu.audio_buff_idx;
	clock_cycle = emu->clock_cycle - additional_amount / sizeof(float) / 2 * 95;
	if (clock_cycle >= FREQUENCY)
	{
		// clock_cycle %= FREQUENCY;
		// printf("b4%llu\n", clock_cycle);
		clock_cycle = ((t_ull)-1) - clock_cycle;
		clock_cycle = FREQUENCY - clock_cycle;
		// printf("af%llu\n", clock_cycle);
	}
	clock_idx = 0;
	used_sample = 0;
	(void)total_amount;
	(void)additional_amount;
	(void)stream;
	(void)i;
	// SDL_LockAudioStream(stream);
	while (i < (additional_amount / sizeof(float) / 2))
	{
		data[0] = 0;
		n_sample = 0;
		while ((((clock_cycle + clock_idx) % AUDIO_BUFFER_SIZE) % 95) && clock_idx / 4 < emu->apu.audio_buff_len)
		{
			if (is_apu_ch2_on(emu) && !(clock_idx % 4))
			{
				data[0] += emu->apu.audio_buff[(emu->apu.audio_buff_idx + clock_idx / 4) % AUDIO_BUFFER_SIZE];
				++n_sample;
			}
			++clock_idx;
		}
		if (n_sample)
			data[0] /= (n_sample);
		used_sample += n_sample;
		data[1] = data[0];
		++clock_idx;
		SDL_PutAudioStreamData(stream, &data, sizeof(float) * 2);
		++i;
	}
	(void)buf_idx;
	// if (clock_idx / 4 < buf_idx)
	{
		// memmove(&emu->apu.audio_buff, &emu->apu.audio_buff[clock_idx / 4], emu->apu.audio_buff_idx - clock_idx / 4);
		emu->apu.audio_buff_idx = (emu->apu.audio_buff_idx + used_sample) % AUDIO_BUFFER_SIZE;
		emu->apu.audio_buff_len -= used_sample;
	}
	// else
	// 	emu->apu.audio_buff_idx = 0;
	// SDL_UnlockAudioStream(stream);
}

void apu_callback(void *userdata, SDL_AudioStream *stream,
	int additional_amount, int total_amount)
{
	t_ull	i;
	t_emu	*emu;
	float	*data;
	// t_word	buf_idx;
	t_word	n_sample;
	t_word	used_sample;

	i = 0;
	emu = (t_emu *)userdata;
	n_sample = 0;
	used_sample = 0;
	(void)n_sample;
	(void)total_amount;
	data = malloc(additional_amount);
	if (!data)
		return ;
	// SDL_LockAudioStream(stream);
	// printf("b4:%llu add:%d\n", emu->apu.audio_buff_len, additional_amount);
	// while (i < additional_amount / sizeof(float))
	// {
	// 	data[i] = 0.0f;
	// 	if (i / 2 < emu->apu.audio_buff_len)
	// 	{
	// 		data[i] = emu->apu.audio_buff[(emu->apu.audio_buff_idx + i / 2) % AUDIO_BUFFER_SIZE];
	// 		++used_sample;
	// 	}
	// 	data[i + 1] = data[i];
	// 	i += 2;
	// }
	// SDL_PutAudioStreamData(stream, data, additional_amount);
	// emu->apu.audio_buff_idx = (emu->apu.audio_buff_idx + i / 2) % AUDIO_BUFFER_SIZE;
	// if (emu->apu.audio_buff_len > i / 2)
	// 	emu->apu.audio_buff_len -= i / 2;
	// else
	// 	emu->apu.audio_buff_len = 0;
	while (n_sample < additional_amount / sizeof(float) / 2 && used_sample < emu->apu.audio_buff_len)
	{
		i = 0;
		data[n_sample * 2] = 0;
		while (i < 25 && i + used_sample < emu->apu.audio_buff_len)
		{
			data[n_sample * 2] += emu->apu.audio_buff[(emu->apu.audio_buff_idx + used_sample + i) % AUDIO_BUFFER_SIZE];
			++i;
		}
		if (i)
			data[n_sample * 2] /= i;
		// printf("%f\n", data[n_sample * 2]);
		used_sample += i;
		data[n_sample * 2 + 1] = data[n_sample * 2];
		++n_sample;
	}
	// printf("%d %d\n", n_sample, total_amount);
	// SDL_PutAudioStreamData(stream, data, total_amount);
	SDL_PutAudioStreamData(stream, data, n_sample * sizeof(float) * 2);
	emu->apu.audio_buff_idx = (emu->apu.audio_buff_idx + used_sample) % AUDIO_BUFFER_SIZE;
	if (emu->apu.audio_buff_len > used_sample)
		emu->apu.audio_buff_len -= used_sample;
	else
		emu->apu.audio_buff_len = 0;
	// printf("af:%llu\n", emu->apu.audio_buff_len);
	free(data);
	// SDL_UnlockAudioStream(stream);
}

void	write_data_to_stream(t_emu *emu)
{
	float	data[32];
	t_ull	idx;
	t_word	n_sample;

	idx = 0;
	while (idx < 32)
		data[idx++] = 0.0f;
		// bzero(&data, sizeof(data));
	idx = 0;
	n_sample = 0;
	if (!(emu->clock_cycle % 64))
	{
		// printf("%llu\n", emu->apu.audio_buff_len);
		while (idx < 32)
		{
			if (idx / 2 < emu->apu.audio_buff_len)
			{
				data[idx] = emu->apu.audio_buff[(emu->apu.audio_buff_idx + idx) % AUDIO_BUFFER_SIZE];
				data[idx + 1] = data[idx];
				++n_sample;
			}
			idx += 2;
		}
		emu->apu.audio_buff_idx = (emu->apu.audio_buff_idx + n_sample) % AUDIO_BUFFER_SIZE;
		emu->apu.audio_buff_len -= n_sample;
		SDL_PutAudioStreamData(emu->audio_stream,
			&data, sizeof(float) * 32);
	}
}

void	apu_tick(t_emu *emu)
{
	static t_ull	idx = 0;
	float			data[2];
	
	
	if (!is_apu_enabled(emu))
		return ;
	if (!is_apu_ch1_dac_on(emu))
		apu_disable_ch1(emu);
	if (!is_apu_ch2_dac_on(emu))
		apu_disable_ch2(emu);
	if (!(emu->clock_cycle % 4))
	{
		if (need_add_apu_div(emu))
		{
			++(emu->apu.apu_div);
			if (!(emu->apu.apu_div % 2))
			{
				if (is_apu_ch1_on(emu) && is_apu_ch1_length_timer_on(emu))
					++(emu->apu.ch1_length_timer);
				if (is_apu_ch2_on(emu) && is_apu_ch2_length_timer_on(emu))
					++(emu->apu.ch2_length_timer);
			}
			if (!(emu->apu.apu_div % 8))
			{
				if (is_apu_ch1_on(emu) && is_apu_ch1_dac_on(emu))
				{
					++(emu->apu.ch1_sweep_counter);
					if ((emu->apu.nr12_c1_vol_env & 7) && (emu->apu.ch1_sweep_counter >= (emu->apu.nr12_c1_vol_env & 7)))
					{
						// printf("ch1 sweep pace:%d counter:%d\n", emu->apu.nr12_c1_vol_env & 7, emu->apu.ch1_sweep_counter);
						if ((emu->apu.nr12_c1_vol_env & 8) && (emu->apu.ch1_vol < 0xF))
							++(emu->apu.ch1_vol);
						if (!(emu->apu.nr12_c1_vol_env & 8) && (emu->apu.ch1_vol > 0))
							--(emu->apu.ch1_vol);
						emu->apu.ch1_sweep_counter = 0;
					}
				}
				if (is_apu_ch2_on(emu) && is_apu_ch2_dac_on(emu))
				{
					++(emu->apu.ch2_sweep_counter);
					if ((emu->apu.nr22_c2_vol_env & 7) && (emu->apu.ch2_sweep_counter >= (emu->apu.nr22_c2_vol_env & 7)))
					{
						if ((emu->apu.nr22_c2_vol_env & 8) && (emu->apu.ch2_vol < 0xF))
							++(emu->apu.ch2_vol);
						if (!(emu->apu.nr22_c2_vol_env & 8) && (emu->apu.ch2_vol > 0))
							--(emu->apu.ch2_vol);
						emu->apu.ch2_sweep_counter = 0;
					}
				}
			}
		}
		if (is_apu_ch1_on(emu))
			apu_step_ch1(emu);
		if (is_apu_ch2_on(emu))
			apu_step_ch2(emu);
	}
	// if ((emu->clock_cycle >= 95.1089 * idx))
	// if (!(emu->clock_cycle % 95))
	// if (emu->clock_cycle / 95.108934240362811 > idx)
	// write_data_to_stream(emu);
	if (!(emu->clock_cycle % 4))
	{
		// printf("%llu %llu\n", emu->clock_cycle, idx);
		data[0] = 0;
		// if (is_apu_ch2_on(emu))
		if (is_apu_ch1_on(emu))
		{
			// printf("%f\n", apu_buf_avg(emu));
			// data += apu_buf_avg(emu);
			// printf("idx:%d\n", emu->apu.audio_buff_idx);
			data[0] += emu->apu.ch1_value;
			// emu->apu.audio_buff_idx = 0;
		}
		if (is_apu_ch2_on(emu))
		{
			data[0] += emu->apu.ch2_value;
			// data += apu_buf_avg(emu);
			// emu->apu.audio_buff_idx = (emu->apu.audio_buff_idx
			// 		+ emu->apu.audio_buff_len) % AUDIO_BUFFER_SIZE;
			// emu->apu.audio_buff_len = 0;
		}
		if (is_apu_ch1_on(emu) && is_apu_ch2_on(emu))
			data[0] /= 2.0;
		// printf("data%f\n", data);
		// if (is_apu_ch1_on(emu) || is_apu_ch2_dac_on(emu))
		data[1] = data[0];
		// SDL_PutAudioStreamData(emu->audio_stream,
		// 	&data, sizeof(float) * 2);
		// data = 0;
		// SDL_PutAudioStreamData(emu->audio_stream,
		// 	&data, 4);
		++idx;
		if (idx >= SAMPLING_RATE || emu->clock_cycle >= FREQUENCY)
		{
			// printf("1s %f\n", emu->last_render_time);
			idx = 0;
		}
	}
}
