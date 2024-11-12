/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:14:26 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/12 23:25:14 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	init_input(t_emu *emu)
{
	bzero(&emu->joypad, sizeof(t_joypad));
	emu->joypad.joyp = 0xFF;
}

void	input_write(t_emu *emu, t_byte data)
{
	// printf("data%x joypad%x\n", data, emu->joypad);
	data = (data & 0xF0) | (emu->joypad.joyp & 0xF);
	emu->joypad.joyp = data;
}

t_byte	input_read(t_emu *emu)
{
	if ((emu->joypad.joyp & 0xF0) == 0x30)
		return (0xF);
	return (emu->joypad.joyp & 0xF);
}

void	input_tick_state(t_emu *emu)
{
	t_byte	interrupt;

	interrupt = 0;
	emu->joypad.joyp |= 0xF;
	if (emu->joypad.a && (emu->joypad.joyp & 0xF0) == 0x10)
	{
		emu->joypad.joyp = (emu->joypad.joyp & 0xF0) | 0b1110;
		interrupt = 1;
	}
	if (emu->joypad.b && (emu->joypad.joyp & 0xF0) == 0x10)
	{
		emu->joypad.joyp = (emu->joypad.joyp & 0xF0) | 0b1101;
		interrupt = 1;
	}
	if (emu->joypad.select && (emu->joypad.joyp & 0xF0) == 0x10)
	{
		emu->joypad.joyp = (emu->joypad.joyp & 0xF0) | 0b1011;
		interrupt = 1;
	}
	if (emu->joypad.start && (emu->joypad.joyp & 0xF0) == 0x10)
	{
		emu->joypad.joyp = (emu->joypad.joyp & 0xF0) | 0b0111;
		interrupt = 1;
	}
	if (emu->joypad.right && (emu->joypad.joyp & 0xF0) == 0x20)
	{
		emu->joypad.joyp = (emu->joypad.joyp & 0xF0) | 0b1110;
		interrupt = 1;
	}
	if (emu->joypad.left && (emu->joypad.joyp & 0xF0) == 0x20)
	{
		emu->joypad.joyp = (emu->joypad.joyp & 0xF0) | 0b1101;
		interrupt = 1;
	}
	if (emu->joypad.up && (emu->joypad.joyp & 0xF0) == 0x20)
	{
		emu->joypad.joyp = (emu->joypad.joyp & 0xF0) | 0b1011;
		interrupt = 1;
	}
	if (emu->joypad.down && (emu->joypad.joyp & 0xF0) == 0x20)
	{
		emu->joypad.joyp = (emu->joypad.joyp & 0xF0) | 0b0111;
		interrupt = 1;
	}
	if (interrupt)
		emu->interrupt_flag |= 16;
}

void	handle_input_down(t_emu *emu, SDL_Event event)
{
	if (event.key.key == SDLK_D)
		emu->joypad.right = 1;
	if (event.key.key == SDLK_A)
		emu->joypad.left = 1;
	if (event.key.key == SDLK_W)
		emu->joypad.up = 1;
	if (event.key.key == SDLK_S)
		emu->joypad.down = 1;
	if (event.key.key == SDLK_K)
		emu->joypad.a = 1;
	if (event.key.key == SDLK_J)
		emu->joypad.b = 1;
	if (event.key.key == SDLK_Z)
		emu->joypad.select = 1;
	if (event.key.key == SDLK_X)
		emu->joypad.start = 1;
}

void	handle_input_up(t_emu *emu, SDL_Event event)
{
	if (event.key.key == SDLK_D)
		emu->joypad.right = 0;
	if (event.key.key == SDLK_A)
		emu->joypad.left = 0;
	if (event.key.key == SDLK_W)
		emu->joypad.up = 0;
	if (event.key.key == SDLK_S)
		emu->joypad.down = 0;
	if (event.key.key == SDLK_K)
		emu->joypad.a = 0;
	if (event.key.key == SDLK_J)
		emu->joypad.b = 0;
	if (event.key.key == SDLK_Z)
		emu->joypad.select = 0;
	if (event.key.key == SDLK_X)
		emu->joypad.start = 0;
}
