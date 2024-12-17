/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:14:26 by mayeung           #+#    #+#             */
/*   Updated: 2024/12/17 21:52:17 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	init_input(t_emu *emu)
{
	bzero(&emu->joypad, sizeof(t_joypad));
	emu->joypad.joyp = 0xCF;
}

t_byte	get_key_state(t_emu *emu)
{
	t_byte	pad_status;

	pad_status = 0xFF;
	if (!(emu->joypad.joyp & 0x10))
	{
		if (emu->joypad.right)
			pad_status &= ~1;
		if (emu->joypad.left)
			pad_status &= ~2;
		if (emu->joypad.up)
			pad_status &= ~4;
		if (emu->joypad.down)
			pad_status &= ~8;
		pad_status &= ~0x10;
	}
	if (!(emu->joypad.joyp & 0x20))
	{
		if (emu->joypad.a)
			pad_status &= ~1;
		if (emu->joypad.b)
			pad_status &= ~2;
		if (emu->joypad.select)
			pad_status &= ~4;
		if (emu->joypad.start)
			pad_status &= ~8;
		pad_status &= ~0x20;
	}
	return (pad_status);
}

void	input_tick(t_emu *emu)
{
	t_byte	pad_status;

	pad_status = get_key_state(emu);
	if ((!(pad_status & 1) && (emu->joypad.joyp & 1))
		|| (!(pad_status & 2) && (emu->joypad.joyp & 2))
		|| (!(pad_status & 4) && (emu->joypad.joyp & 4))
		|| (!(pad_status & 8) && (emu->joypad.joyp & 8)))
		emu->interrupt_flag |= 16;
	emu->joypad.joyp = pad_status;
}

void	input_write(t_emu *emu, t_byte data)
{
	emu->joypad.joyp = (0xCF & emu->joypad.joyp) | (data & 0x30);
}

t_byte	input_read(t_emu *emu)
{
	return (emu->joypad.joyp);
}

void	handle_input_down(t_emu *emu, SDL_Event event)
{
	if (event.key.key == SDLK_D)
		emu->joypad.right = TRUE;
	if (event.key.key == SDLK_A)
		emu->joypad.left = TRUE;
	if (event.key.key == SDLK_W)
		emu->joypad.up = TRUE;
	if (event.key.key == SDLK_S)
		emu->joypad.down = TRUE;
	if (event.key.key == SDLK_K)
		emu->joypad.a = TRUE;
	if (event.key.key == SDLK_J)
		emu->joypad.b = TRUE;
	if (event.key.key == SDLK_Z)
		emu->joypad.select = TRUE;
	if (event.key.key == SDLK_X)
		emu->joypad.start = TRUE;
}

void	handle_input_up(t_emu *emu, SDL_Event event)
{
	if (event.key.key == SDLK_D)
		emu->joypad.right = FALSE;
	if (event.key.key == SDLK_A)
		emu->joypad.left = FALSE;
	if (event.key.key == SDLK_W)
		emu->joypad.up = FALSE;
	if (event.key.key == SDLK_S)
		emu->joypad.down = FALSE;
	if (event.key.key == SDLK_K)
		emu->joypad.a = FALSE;
	if (event.key.key == SDLK_J)
		emu->joypad.b = FALSE;
	if (event.key.key == SDLK_Z)
		emu->joypad.select = FALSE;
	if (event.key.key == SDLK_X)
		emu->joypad.start = FALSE;
}
