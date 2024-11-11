/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:42:59 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/11 13:28:51 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	init_ppu(t_emu *emu)
{
	emu->ppu.ly = 0;
	emu->ppu.lx = 0;
	emu->ppu.ppu_mode = OAM_SCAN;
	emu->ppu.scx = 0;
	emu->ppu.scy = 0;
	emu->ppu.stat = 0;
}

void	ppu_write(t_emu *emu, t_word addr, t_byte data)
{
	if (addr == 0xFF40)
		emu->ppu.lcdc = data;
	if (addr == 0xFF41)
		emu->ppu.stat = data;
	if (addr == 0xFF42)
		emu->ppu.scy = data;
	if (addr == 0xFF43)
		emu->ppu.scx = data;
	if (addr == 0xFF47)
		emu->ppu.bgp = data;
	if (addr == 0xFF48)
		emu->ppu.obp0 = data;
	if (addr == 0xFF49)
		emu->ppu.obp1 = data;
	if (addr == 0xFF4A)
		emu->ppu.wy = data;
	if (addr == 0xFF4B)
		emu->ppu.wx = data;
}

t_byte	ppu_read(t_emu *emu, t_word addr)
{
	if (addr == 0xFF40)
		return (emu->ppu.lcdc);
	if (addr == 0xFF41)
		return (emu->ppu.stat);
	if (addr == 0xFF42)
		return (emu->ppu.scy);
	if (addr == 0xFF43)
		return (emu->ppu.scx);
	if (addr == 0xFF44)
		return (emu->ppu.ly);
	if (addr == 0xFF45)
		return (emu->ppu.lyc);
	if (addr == 0xFF47)
		return (emu->ppu.bgp);
	if (addr == 0xFF48)
		return (emu->ppu.obp0);
	if (addr == 0xFF49)
		return (emu->ppu.obp1);
	if (addr == 0xFF4A)
		return (emu->ppu.wy);
	if (addr == 0xFF4B)
		return (emu->ppu.wx);
	return (0xFF);
}

// void	ppu_draw_pix_n_time(t_emu *emu, t_byte n)
// {

// }

void	ppu_draw_pix(t_emu *emu)
{
	t_byte		cid;
	t_byte		tid;
	t_byte		pi;
	t_word		offset;
	SDL_Surface	*s;

	tid = emu->ppu.ly / 8 * SCREEN_NUM_TILE_PER_ROW + emu->ppu.lx / 8;
	offset = tid * 16 + emu->ppu.ly % 8 * 2;
			// 	cid = (app->emu.vram[offset]
			// 		& (1 << (7 - pi))) >> (7 - pi);
			// cid += ((app->emu.vram[offset + 1]
			// 			& (1 << (7 - pi))) >> (7 - pi)) << 1;
	pi = emu->ppu.lx % 8;
	cid = (emu->vram[offset] & (1 << (7 - pi))) >> (7 - pi);
	cid += ((emu->vram[offset + 1] & (1 << (7 - pi))) >> (7 - pi)) << 1;
	s = SDL_GetWindowSurface(emu->window);
	SDL_LockSurface(s);
	print_pixel(s, cid, emu->ppu.lx / 8, emu->ppu.ly / 8, emu->ppu.lx % 8, emu->ppu.ly % 8);
	SDL_UnlockSurface(s);
}

void	ppu_tick(t_emu *emu)
{
	++(emu->ppu.lx);
	if (emu->ppu.ly < 144)
	{
		if (emu->ppu.lx == 456)
		{
			emu->ppu.lx = 0;
			emu->ppu.ppu_mode = OAM_SCAN;
			++(emu->ppu.ly);
		}
		else if (emu->ppu.lx == 252)
		{
			emu->ppu.ppu_mode = HBLANK;
			emu->ppu.stat = 1;
		}
		else if (emu->ppu.lx == 80)
			emu->ppu.ppu_mode = DRAWING;
	}
	if (emu->ppu.lx == 456)
	{
		emu->ppu.lx = 0;
		++(emu->ppu.ly);
	}
	if (emu->ppu.ly == 154)
	{
		emu->ppu.ly = 0;
		emu->ppu.ppu_mode = OAM_SCAN;
	}
	else if (emu->ppu.ly == 144)
		emu->ppu.ppu_mode = VBLANK;
	emu->ppu.stat &= ~3;
	emu->ppu.stat |= emu->ppu.ppu_mode;
	emu->ppu.stat &= ~4;
	emu->ppu.stat |= (emu->ppu.ly == emu->ppu.lyc) * 4;
	if ((emu->ppu.ly < 144 && ((emu->ppu.lx == 0 && (emu->ppu.stat & 32))
				|| (emu->ppu.lx == 252 && (emu->ppu.stat & 8))))
		|| (emu->ppu.ly == 144 && emu->ppu.lx == 0 && (emu->ppu.stat & 16))
		|| ((emu->ppu.stat & 4) && (emu->ppu.stat & 64)))
		emu->interrupt_flag |= 2;
	if (emu->ppu.ly == 144 && emu->ppu.lx == 0)
		emu->interrupt_flag |= 1;
	if (emu->ppu.ppu_mode == DRAWING)
		ppu_draw_pix(emu);
}
