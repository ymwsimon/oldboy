/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:10:16 by mayeung           #+#    #+#             */
/*   Updated: 2025/01/08 18:40:13 by mayeung          ###   ########.fr       */
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
		else
			apu_disable_ch1(emu);
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
		else
			apu_disable_ch2(emu);
	}
	if (addr == 0xFF1A)
		emu->apu.nr30_c3_dac = 0x7F | (data & 0x80);
	if (addr == 0xFF1B)
		emu->apu.nr31_c3_timer = data;
	if (addr == 0xFF1C)
		emu->apu.nr32_c3_out_lvl = 0x60 | (data & 0x9F);
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
		emu->apu.nr52_mas_ctrl = (emu->apu.nr52_mas_ctrl & 0b01111111) | (data & 0b10000000);
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
	t_word			p_div;
	t_word			div;

	div = emu->timer.div;// / 256;
	p_div = div - 1;
	return ((div ^ p_div) & (1 << 4) && (div & (1 << 4)));
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
	return (emu->apu.nr14_c1_per_high_ctrl & 64);
}

t_byte	is_apu_ch2_length_timer_on(t_emu *emu)
{
	return (emu->apu.nr24_c2_per_high_ctrl & 64);
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
}

void	apu_disable_ch2(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl &= ~2;
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
	emu->apu.ch1_length_timer = emu->apu.nr11_c1_timer_duty & 31;
	emu->apu.ch1_period_counter = apu_get_ch1_period(emu);
	emu->apu.ch1_sample_idx = 0;
	emu->apu.ch1_value = 0;
	emu->apu.ch1_sweep_counter = 0;
	emu->apu.ch1_vol = emu->apu.nr12_c1_vol_env >> 4;
}

void	apu_enable_ch2(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl |= 2;
	emu->apu.ch2_length_timer = emu->apu.nr21_c2_timer_duty & 31;
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
		++(emu->apu.ch1_sample_idx);
		emu->apu.ch1_period_counter = apu_get_ch1_period(emu);
	}
	emu->apu.ch1_sample_idx %= 8;
	emu->apu.ch1_value = apu_dac(1, -1,
			apu_get_sq_wave_sample(emu->apu.ch1_sample_idx,
				emu->apu.nr11_c1_timer_duty >> 6));
	emu->apu.ch1_value *= ((float)emu->apu.ch1_vol / 15.0);
	if (emu->apu.ch1_length_timer == 64)
		apu_disable_ch1(emu);
}

void	apu_step_ch2(t_emu *emu)
{
	++(emu->apu.ch2_period_counter);
	if (emu->apu.ch2_period_counter >= 2048)
	{
		++(emu->apu.ch2_sample_idx);
		emu->apu.ch2_period_counter = apu_get_ch2_period(emu);
	}
	emu->apu.ch2_sample_idx %= 8;
	emu->apu.ch2_value = apu_dac(1, -1,
			apu_get_sq_wave_sample(emu->apu.ch2_sample_idx,
				emu->apu.nr21_c2_timer_duty >> 6));
	emu->apu.ch2_value *= ((float)emu->apu.ch2_vol / 15.0);
	if (emu->apu.ch2_length_timer == 64)
		apu_disable_ch2(emu);
}

void	apu_tick(t_emu *emu)
{
	static	t_ull	idx = 0;
	float	data;

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
	if (!((emu->clock_cycle * 10) % (950)))
	{
		data = 0;
		if (is_apu_ch1_on(emu))
			data += emu->apu.ch1_value;
		if (is_apu_ch2_on(emu))
			data += emu->apu.ch2_value;
		if (is_apu_ch1_on(emu) && is_apu_ch2_on(emu))
			data /= 2.0;
		// if (is_apu_ch1_on(emu) || is_apu_ch2_dac_on(emu))
		SDL_PutAudioStreamData(emu->audio_stream,
			&data, sizeof(float));
		// SDL_PutAudioStreamData(emu->audio_stream,
			// &data, sizeof(float));
		++idx;
		if (idx == SAMPLING_RATE)
		{
			// printf("1s %f\n", emu->last_render_time);
			idx = 0;
		}
		// SDL_PutAudioStreamData(emu->audio_stream,
		// 	&data, sizeof(float));
		// SDL_PutAudioStreamData(emu->audio_stream,
			// &emu->apu.ch1_value, sizeof(float));
		// int	s;
		// SDL_AudioSpec	as;
		// SDL_GetAudioDeviceFormat(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &as, &s);
		// printf("channel%i, freq%i, sr%i\n", as.channels, as.freq, s);
		// printf("writing sound in cycle%llu\n", emu->clock_cycle);
	}
}
