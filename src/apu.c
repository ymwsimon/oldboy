/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:10:16 by mayeung           #+#    #+#             */
/*   Updated: 2025/02/10 12:13:05 by mayeung          ###   ########.fr       */
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
	emu->apu.apu_div = 0;
	emu->apu.ch1_value = 0;
	emu->apu.ch2_value = 0;
	emu->apu.play_ch1 = TRUE;
	emu->apu.play_ch2 = TRUE;
	emu->apu.play_ch3 = TRUE;
	emu->apu.play_ch4 = TRUE;
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
		if (data & 0x80)
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
		if (data & 0x80)
			apu_enable_ch2(emu);
	}
	if (addr == 0xFF1A)
	{
		emu->apu.nr30_c3_dac = 0x7F | (data & 0x80);
		if (data & 0x80)
			apu_enable_ch3(emu);
		else
			apu_disable_ch3(emu);
	}
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
	{
		emu->apu.nr44_c4_ctrl = 0x3F | (data & 0xC0);
		if (data & 0x80)
			apu_enable_ch4(emu);
	}
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

t_byte	is_apu_ch3_dac_on(t_emu *emu)
{
	return (emu->apu.nr30_c3_dac & 0x80);
}


t_byte	is_apu_ch1_length_timer_on(t_emu *emu)
{
	return (emu->apu.nr14_c1_per_high_ctrl & 0x40);
}

t_byte	is_apu_ch2_length_timer_on(t_emu *emu)
{
	return (emu->apu.nr24_c2_per_high_ctrl & 0x40);
}

t_byte	is_apu_ch3_length_timer_on(t_emu *emu)
{
	return (emu->apu.nr34_c3_per_high_ctrl & 0x40);
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

t_word	apu_get_ch3_period(t_emu *emu)
{
	return (((emu->apu.nr34_c3_per_high_ctrl & 7) << 8)
		| emu->apu.nr33_c3_per_low);
}

t_word	apu_get_ch4_period(t_emu *emu)
{
	float	divider;
	t_byte	shift;

	shift = emu->apu.nr43_c4_freq_ran >> 4;
	divider = emu->apu.nr43_c4_freq_ran & 7;
	if (!(emu->apu.nr43_c4_freq_ran & 7))
		divider = 0.5;
	return (divider * (2 << shift));
}

t_byte	apu_lfsr_is_short_mode(t_emu *emu)
{
	return (emu->apu.nr43_c4_freq_ran & 8);
}

t_word	apu_get_ch4_lfsr_range(t_emu *emu)
{
	if (emu->apu.nr43_c4_freq_ran & 8)
		return (0xFF);
	return (0xFFFF);
}

float	apu_get_ch3_vol(t_emu *emu)
{
	const float	vols[] = {0, 1, 0.5, 0.25};
	return (vols[(emu->apu.nr32_c3_out_lvl & 0x60) >> 5]);
}

void	apu_update_ch1_period(t_emu *emu, t_word value)
{
	emu->apu.nr13_c1_per_low = value & 0xFF;
	emu->apu.nr14_c1_per_high_ctrl = (value >> 8) & 7;
}

t_byte	apu_get_ch1_freq_sweep_pace(t_emu *emu)
{
	return ((emu->apu.nr10_c1_sweep >> 4) & 7);
}

t_byte	apu_get_ch1_freq_sweep_dir(t_emu *emu)
{
	return (emu->apu.nr10_c1_sweep & 8);
}

t_byte	apu_get_ch1_freq_sweep_is_add(t_emu *emu)
{
	return (!apu_get_ch1_freq_sweep_dir(emu));
}

t_byte	apu_get_ch1_freq_sweep_step(t_emu *emu)
{
	return (emu->apu.nr10_c1_sweep & 7);
}

t_byte	is_apu_ch1_freq_sweep_on(t_emu *emu)
{
	return (apu_get_ch1_freq_sweep_pace(emu)
		|| apu_get_ch1_freq_sweep_step(emu));
}

void	apu_ch1_freq_sweep_perform(t_emu *emu, t_byte update_reg)
{
	t_word	new_period;

	if (apu_get_ch1_freq_sweep_step(emu))
	{
		new_period = emu->apu.ch1_period_og >> apu_get_ch1_freq_sweep_step(emu);
		if (apu_get_ch1_freq_sweep_is_add(emu))
			new_period += emu->apu.ch1_period_og;
		else
			new_period = emu->apu.ch1_period_og - new_period;
		if (new_period > 0x7FF)
			apu_disable_ch1(emu);
		else
		{
			if (update_reg)
			{
				emu->apu.ch1_period_og = new_period;
				apu_update_ch1_period(emu, new_period);
			}
		}
	}
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
	emu->apu.ch1_vol_sweep_counter = 0;
	emu->apu.ch1_vol = emu->apu.nr12_c1_vol_env >> 4;
	emu->apu.ch1_freq_sweep_pace = apu_get_ch1_freq_sweep_pace(emu);
	emu->apu.ch1_period_og = apu_get_ch1_period(emu);
	emu->apu.ch1_freq_sweep_enabled = is_apu_ch1_freq_sweep_on(emu);
	if (apu_get_ch1_freq_sweep_step(emu))
		apu_ch1_freq_sweep_perform(emu, FALSE);
}

void	apu_enable_ch2(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl |= 2;
	emu->apu.ch2_length_timer = emu->apu.nr21_c2_timer_duty & 0x3F;
	emu->apu.ch2_period_counter = apu_get_ch2_period(emu);
	emu->apu.ch2_sample_idx = 0;
	emu->apu.ch2_value = 0;
	emu->apu.ch2_vol_sweep_counter = 0;
	emu->apu.ch2_vol = emu->apu.nr22_c2_vol_env >> 4;
}

void	apu_enable_ch3(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl |= 4;
	emu->apu.ch3_temp_data = emu->apu.wave_ram[0] >> 4;
	emu->apu.ch3_sample_idx = 0;
	emu->apu.ch3_value = emu->apu.ch3_temp_data * apu_get_ch3_vol(emu);
	emu->apu.ch3_length_timer = emu->apu.nr31_c3_timer;
	emu->apu.ch3_period_counter = apu_get_ch3_period(emu);
}

void	apu_enable_ch4(t_emu *emu)
{
	emu->apu.nr52_mas_ctrl |= 8;
	emu->apu.ch4_length_timer = emu->apu.nr41_c4_timer & 0x3F;
	emu->apu.ch4_period_counter = apu_get_ch4_period(emu);
	emu->apu.ch4_value = 0;
	emu->apu.ch4_vol_sweep_counter = 0;
	emu->apu.ch4_vol = emu->apu.nr42_c4_vol_env >> 4;
	emu->apu.lfsr = 0;
}

float	apu_dac(float range, float max, float min, t_byte sample)
{
	return ((float)sample / (range / (max - min)) - (max - min) / 2);
}

void	apu_step_ch1(t_emu *emu)
{
	++(emu->apu.ch1_period_counter);
	if (emu->apu.ch1_period_counter >= 2048)
	{
		emu->apu.ch1_sample_idx = (emu->apu.ch1_sample_idx + 1) % 8;
		emu->apu.ch1_period_counter = apu_get_ch1_period(emu);
	}
	emu->apu.ch1_value = apu_dac(1, 1, -1,
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
		emu->apu.ch2_sample_idx = (emu->apu.ch2_sample_idx + 1) % 8;
		emu->apu.ch2_period_counter = apu_get_ch2_period(emu);
	}
	emu->apu.ch2_value = apu_dac(1, 1, -1,
			apu_get_sq_wave_sample(emu->apu.ch2_sample_idx,
				emu->apu.nr21_c2_timer_duty >> 6));
	emu->apu.ch2_value *= ((float)emu->apu.ch2_vol / 15.0);
	if (emu->apu.ch2_length_timer == 64)
		apu_disable_ch2(emu);
}

void	apu_step_ch3(t_emu *emu)
{
	++(emu->apu.ch3_period_counter);
	if (emu->apu.ch3_period_counter >= 2048)
	{
		emu->apu.ch3_sample_idx = (emu->apu.ch3_sample_idx + 1) % 32;
		emu->apu.ch3_temp_data = emu->apu.wave_ram[emu->apu.ch3_sample_idx / 2];
		if (!(emu->apu.ch3_sample_idx % 2))
			emu->apu.ch3_temp_data >>= 4;
		emu->apu.ch3_temp_data &= 0xF;
		emu->apu.ch3_period_counter = apu_get_ch3_period(emu);
	}
	emu->apu.ch3_value = apu_dac(15, 1, -1, emu->apu.ch3_temp_data);
	emu->apu.ch3_value *= apu_get_ch3_vol(emu);
	if (emu->apu.ch3_length_timer == 256)
		apu_disable_ch3(emu);
}

void	apu_update_lfsr(t_emu *emu)
{
	t_word	new_lfsr;
	t_word	new_b15;

	new_lfsr = emu->apu.lfsr & 0x7FFF;
	new_b15 = (emu->apu.lfsr & 1) == ((emu->apu.lfsr >> 1) & 1);
	if (apu_lfsr_is_short_mode(emu))
	{
		new_lfsr = emu->apu.lfsr & 0x7F;
		new_lfsr |= new_b15 << 7;
	}
	new_lfsr |= new_b15 << 15;
	new_lfsr >>= 1;
	emu->apu.lfsr = new_lfsr;
}

void	apu_step_ch4(t_emu *emu)
{
	if (emu->apu.ch4_period_counter)
	{
		--(emu->apu.ch4_period_counter);
		if (emu->apu.ch4_period_counter == 0)
		{
			apu_update_lfsr(emu);
			emu->apu.ch4_period_counter = apu_get_ch4_period(emu);
			// printf("%d\n", emu->apu.ch4_period_counter);
		}
		// emu->apu.ch4_value = apu_dac(apu_get_ch4_lfsr_range(emu), 1, -1, );
		emu->apu.ch4_value = 0;
		if (emu->apu.lfsr & 1)
			emu->apu.ch4_value = ((float)emu->apu.ch4_vol / 15.0);
	}
	// emu->apu.ch4_value = apu_dac(1, 1, -1,
			// apu_get_sq_wave_sample(emu->apu.ch2_sample_idx,
				// emu->apu.nr21_c2_timer_duty >> 6));
	// emu->apu.ch2_value *= ((float)emu->apu.ch2_vol / 15.0);
	if (emu->apu.ch4_length_timer == 64)
		apu_disable_ch4(emu);
}

void	apu_tick(t_emu *emu)
{
	float			data[2];
	t_byte			n_ch_on;

	n_ch_on = 0;
	if (!(emu->clock_cycle % 64))
	{
		data[0] = 0;
		if (is_apu_ch1_on(emu) && emu->apu.play_ch1)
		{
			data[0] += emu->apu.ch1_value;
			++n_ch_on;
		}
		if (is_apu_ch2_on(emu) && emu->apu.play_ch2)
		{
			data[0] += emu->apu.ch2_value;
			++n_ch_on;
		}
		if (is_apu_ch3_on(emu) && emu->apu.play_ch3)
		{
			data[0] += emu->apu.ch3_value;
			++n_ch_on;
		}
		if (is_apu_ch4_on(emu) && emu->apu.play_ch4)
		{
			data[0] += emu->apu.ch4_value;
			++n_ch_on;
		}
		if (n_ch_on)
			data[0] /= n_ch_on;
		data[1] = data[0];
		SDL_PutAudioStreamData(emu->audio_stream,
			&data, sizeof(float) * 2);
	}
	if (!is_apu_enabled(emu))
		return ;
	if (!is_apu_ch1_dac_on(emu))
		apu_disable_ch1(emu);
	if (!is_apu_ch2_dac_on(emu))
		apu_disable_ch2(emu);
	if (!is_apu_ch3_dac_on(emu))
		apu_disable_ch3(emu);
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
				if (is_apu_ch3_on(emu) && is_apu_ch3_length_timer_on(emu))
					++(emu->apu.ch3_length_timer);
			}
			if (!(emu->apu.apu_div % 8))
			{
				if (is_apu_ch1_on(emu) && is_apu_ch1_dac_on(emu))
				{
					++(emu->apu.ch1_vol_sweep_counter);
					if ((emu->apu.nr12_c1_vol_env & 7) && (emu->apu.ch1_vol_sweep_counter >= (emu->apu.nr12_c1_vol_env & 7)))
					{
						// printf("ch1 sweep pace:%d counter:%d\n", emu->apu.nr12_c1_vol_env & 7, emu->apu.ch1_vol_sweep_counter);
						if ((emu->apu.nr12_c1_vol_env & 8) && (emu->apu.ch1_vol < 0xF))
							++(emu->apu.ch1_vol);
						if (!(emu->apu.nr12_c1_vol_env & 8) && (emu->apu.ch1_vol > 0))
							--(emu->apu.ch1_vol);
						emu->apu.ch1_vol_sweep_counter = 0;
					}
				}
				if (is_apu_ch2_on(emu) && is_apu_ch2_dac_on(emu))
				{
					++(emu->apu.ch2_vol_sweep_counter);
					if ((emu->apu.nr22_c2_vol_env & 7) && (emu->apu.ch2_vol_sweep_counter >= (emu->apu.nr22_c2_vol_env & 7)))
					{
						if ((emu->apu.nr22_c2_vol_env & 8) && (emu->apu.ch2_vol < 0xF))
							++(emu->apu.ch2_vol);
						if (!(emu->apu.nr22_c2_vol_env & 8) && (emu->apu.ch2_vol > 0))
							--(emu->apu.ch2_vol);
						emu->apu.ch2_vol_sweep_counter = 0;
					}
				}
				if (is_apu_ch4_on(emu))
				{
					++(emu->apu.ch4_vol_sweep_counter);
					if ((emu->apu.nr42_c4_vol_env & 7) && (emu->apu.ch4_vol_sweep_counter >= (emu->apu.nr42_c4_vol_env & 7)))
					{
						if ((emu->apu.nr42_c4_vol_env & 8) && (emu->apu.ch4_vol < 0xF))
							++(emu->apu.ch4_vol);
						if (!(emu->apu.nr42_c4_vol_env & 8) && (emu->apu.ch4_vol > 0))
							--(emu->apu.ch4_vol);
						emu->apu.ch4_vol_sweep_counter = 0;
					}
				}
			}
			if (!(emu->apu.apu_div % 4))
			{
				if (is_apu_ch1_on(emu) && emu->apu.ch1_freq_sweep_enabled
					&& emu->apu.ch1_freq_sweep_pace)
					--(emu->apu.ch1_freq_sweep_pace);
				if (is_apu_ch1_on(emu) && emu->apu.ch1_freq_sweep_enabled
					&& apu_get_ch1_freq_sweep_pace(emu)
					&& !emu->apu.ch1_freq_sweep_pace)
				{
					apu_ch1_freq_sweep_perform(emu, TRUE);
					emu->apu.ch1_freq_sweep_pace = apu_get_ch1_freq_sweep_pace(emu);
				}
			}
		}
		if (is_apu_ch1_on(emu))
			apu_step_ch1(emu);
		if (is_apu_ch2_on(emu))
			apu_step_ch2(emu);
		// if (is_apu_ch4_on(emu))
			// apu_step_ch4(emu);
	}
	if (!(emu->clock_cycle % 2))
	{
		if (is_apu_ch3_on(emu))
			apu_step_ch3(emu);
	}
	if (!(emu->clock_cycle % 8))
	{
		if (is_apu_ch4_on(emu))
			apu_step_ch4(emu);
	}
	// if (is_apu_ch4_on(emu))
		// apu_step_ch4(emu);
}
