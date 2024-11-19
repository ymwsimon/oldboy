/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:42:59 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/19 19:00:09 by mayeung          ###   ########.fr       */
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
	emu->ppu.dma_write_counter = 0;
	emu->ppu.dma = 0;
	bzero(&emu->ppu.oam, sizeof(t_byte) * 0xA0);
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
	if (addr == 0xFF46)
	{
		emu->ppu.dma = data;
		emu->ppu.dma_write_counter = 640;
	}
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
	if (addr >= 0xFE00 && addr <= 0xFE9F)
		emu->ppu.oam[addr - 0xFE00] = data;
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
	if (addr == 0xFF46)
		return (emu->ppu.dma);
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

void	scan_obj(t_emu *emu)
{
	t_byte	idx;

	emu->ppu.num_obj_scanline = 0;
	idx = 0;
	while (idx < 40 && emu->ppu.num_obj_scanline < 10 && (emu->ppu.lcdc & 2))
	{
		if ((emu->ppu.ly + 16 >= emu->ppu.oam[idx * 4]
				&& emu->ppu.ly + 16 < emu->ppu.oam[idx * 4] + 8))
		{
			emu->ppu.object_queue[emu->ppu.num_obj_scanline] = idx;
			++emu->ppu.num_obj_scanline;
		}
		++idx;
	}
}

// void	ppu_draw_pix_n_time(SDL_Surface *s, t_emu *emu, t_word tid, t_byte cid, t_byte n)
// {
// 	t_byte				i;
// 	t_byte				j;
// 	unsigned int		colour;
// 	static unsigned int	colour_map[4] = {WHITE, LIGHT_GREEN, DARK_GREEN, BLACK};

// 	j = 0;
// 	colour = colour_map[cid];
// 	(void)tid;
// 	while (j < n)
// 	{
// 		i = 0;
// 		while (i < n)
// 		{
// 			SDL_WriteSurfacePixel(s,
// 				(emu->ppu.lx - 80) / 8 * 8 * n + (emu->ppu.lx - 80) % 8 * n + i,
// 				emu->ppu.ly / 8 * 8 * n + emu->ppu.ly % 8 * n + j,
// 				colour >> 16, (colour & 0xFF00) >> 8, colour & 0xFF, 255);
// 			++i;
// 		}
// 		++j;
// 	}

// }

t_byte	get_cid_from(t_emu *emu, int offset, t_word pi)
{
	t_byte	cid;

	cid = (emu->vram[offset] & (1 << pi)) >> pi;
	cid += ((emu->vram[offset + 1] & (1 << pi)) >> pi) << 1;
	return (cid);
}

int	need_yflip(t_emu *emu, t_byte oid)
{
	return (emu->ppu.oam[oid + 3] & 64);
}

int	need_xflip(t_emu *emu, t_byte oid)
{
	return (emu->ppu.oam[oid + 3] & 32);
}

int	get_offset_pj_from_objectid(t_emu *emu, t_byte oid)
{
	int	offset;

	offset = emu->ppu.oam[oid + 2] * 16;
	if (need_yflip(emu, oid))
		offset += (7 - ((emu->ppu.ly - (emu->ppu.oam[oid] - 16))) % 8) * 2;
	else
		offset += ((emu->ppu.ly - (emu->ppu.oam[oid] - 16)) % 8) * 2;
	return (offset);
}

t_word	get_pi_from_objectid(t_emu *emu, t_byte oid)
{
	t_word	pi;

	pi = (emu->ppu.lx - 80 - (emu->ppu.oam[oid + 1] - 8)) % 8;
	if (!need_xflip(emu, oid))
		pi = 7 - pi;
	return (pi);
}

int	ly_is_within_object(t_emu *emu, t_byte oid)
{
	return (emu->ppu.lx - 80 + 8 >= emu->ppu.oam[oid + 1]
		&& emu->ppu.lx - 80 + 8 < emu->ppu.oam[oid + 1] + 8);
}

int	object_enabled(t_emu *emu)
{
	return (emu->ppu.lcdc & 2);
}

int	window_enabled(t_emu *emu)
{
	return (emu->ppu.lcdc & 32);
}

int	is_window_covered(t_emu *emu)
{
	return (window_enabled(emu)
		&& emu->ppu.ly >= emu->ppu.wy
		&& (emu->ppu.lx - 80) + 7 >= emu->ppu.wx);
}

int	get_offset_window_tile(t_emu *emu)
{
	int		offset;
	t_word	tid;
	t_word	li;
	t_word	lj;

	li = emu->ppu.lx - 80 - emu->ppu.wx + 7;
	lj = emu->ppu.ly - emu->ppu.wy;
	tid = lj / 8 * SCREEN_NUM_TILE_PER_ROW + li / 8;
	if (!(emu->ppu.lcdc & 64))
		tid = emu->vram[0x1800 + tid];
	else
		tid = emu->vram[0x1C00 + tid];
	offset = (char)tid * 16 + (lj % 8) * 2;
	if (emu->ppu.lcdc & 16)
		offset = tid * 16 + (lj % 8) * 2;
	if (!(emu->ppu.lcdc & 16))
		offset += 0x1000;
	return (offset);
}

t_byte	get_final_cid(t_emu *emu, int offset)
{
	t_byte	cid;
	t_byte	new_cid;
	t_word	pi;
	t_byte	idx;
	t_byte	oid;

	pi = 7 - ((emu->ppu.lx - 80 + emu->ppu.scx) % 256) % 8;
	cid = get_cid_from(emu, offset, pi);
	idx = 0;
	while (object_enabled(emu) && idx < emu->ppu.num_obj_scanline)
	{
		oid = emu->ppu.object_queue[idx] * 4;
		if (ly_is_within_object(emu, oid))
		{
			pi = get_pi_from_objectid(emu, oid);
			offset = get_offset_pj_from_objectid(emu, oid);
			new_cid = get_cid_from(emu, offset, pi);
			if (new_cid)
				cid = new_cid;
		}
		++idx;
	}
	return (cid);
}

void	ppu_draw_pix(t_emu *emu)
{
	t_byte		cid;
	t_word		tid;
	t_word		li;
	t_word		lj;
	int			offset;
	SDL_Surface	*s;

	li = (emu->ppu.lx - 80 + emu->ppu.scx) % 256;
	lj = (emu->ppu.ly + emu->ppu.scy) % 256;
	tid = lj / 8 * SCREEN_NUM_TILE_PER_ROW + li / 8;
	if (!(emu->ppu.lcdc & 8))
		tid = emu->vram[0x1800 + tid];
	else
		tid = emu->vram[0x1C00 + tid];
	offset = (char)tid * 16 + (lj % 8) * 2;
	if (emu->ppu.lcdc & 16)
		offset = tid * 16 + (lj % 8) * 2;
	if (!(emu->ppu.lcdc & 16))
		offset += 0x1000;
	if (is_window_covered(emu))
		offset = get_offset_window_tile(emu);
	cid = get_final_cid(emu, offset);
	s = SDL_GetWindowSurface(emu->window);
	SDL_LockSurface(s);
	print_pixel(emu, s, cid, (t_tile_pix_info){(emu->ppu.lx - 80) / 8,
		emu->ppu.ly / 8, (emu->ppu.lx - 80) % 8, emu->ppu.ly % 8});
	SDL_UnlockSurface(s);
}

int	is_vblank(t_emu *emu)
{
	return (emu->ppu.ly >= 144 && emu->ppu.ly <= 153);
}

int	is_last_dot_of_line(t_emu *emu)
{
	return (emu->ppu.lx == 456);
}

void	non_vblank(t_emu *emu)
{
	if (!is_vblank(emu))
	{
		if (is_last_dot_of_line(emu))
		{
			emu->ppu.lx = 0;
			emu->ppu.ppu_mode = OAM_SCAN;
			++(emu->ppu.ly);
		}
		else if (emu->ppu.lx == 240)
		{
			emu->ppu.ppu_mode = HBLANK;
			emu->ppu.stat = 1;
		}
		else if (emu->ppu.lx == 80)
			emu->ppu.ppu_mode = DRAWING;
	}
}

void	last_dot_per_line(t_emu *emu)
{
	if (is_last_dot_of_line(emu))
	{
		emu->ppu.lx = 0;
		++(emu->ppu.ly);
	}
}

void	last_line_of_screen(t_emu *emu)
{
	if (emu->ppu.ly == 154)
	{
		emu->ppu.ly = 0;
		emu->ppu.lx = 0;
		emu->ppu.ppu_mode = OAM_SCAN;
	}
}

int	is_first_dot_of_vblank(t_emu *emu)
{
	return (emu->ppu.ly == 144 && emu->ppu.lx == 0);
}

void	vblank(t_emu *emu)
{
	if (is_first_dot_of_vblank(emu))
	{
		emu->ppu.ppu_mode = VBLANK;
		SDL_UpdateWindowSurface(emu->window);
	}
}

void	dma_transfer(t_emu *emu)
{
	if (emu->ppu.dma_write_counter)
	{
		if (!(emu->ppu.dma_write_counter % 4))
		{
			emu->ppu.oam[160 - emu->ppu.dma_write_counter / 4]
				= bus_read(emu, (emu->ppu.dma << 8)
					+ (160 - emu->ppu.dma_write_counter / 4));
		}
		--(emu->ppu.dma_write_counter);
	}
}

void	ppu_tick(t_emu *emu)
{
	++(emu->ppu.lx);
	non_vblank(emu);
	last_dot_per_line(emu);
	last_line_of_screen(emu);
	vblank(emu);
	emu->ppu.stat &= ~3;
	emu->ppu.stat |= emu->ppu.ppu_mode;
	emu->ppu.stat &= ~4;
	emu->ppu.stat |= (emu->ppu.ly == emu->ppu.lyc) * 4;
	if ((emu->ppu.ly < 144 && ((emu->ppu.lx == 0 && (emu->ppu.stat & 32))
				|| (emu->ppu.lx == 240 && (emu->ppu.stat & 8))))
		|| (emu->ppu.ly == 144 && emu->ppu.lx == 0 && (emu->ppu.stat & 16))
		|| ((emu->ppu.stat & 4) && (emu->ppu.stat & 64)))
		emu->interrupt_flag |= 2;
	if (emu->ppu.ly == 144 && emu->ppu.lx == 0)
		emu->interrupt_flag |= 1;
	if (emu->ppu.ppu_mode == OAM_SCAN && !emu->ppu.lx)
		scan_obj(emu);
	if (emu->ppu.ppu_mode == DRAWING)
		ppu_draw_pix(emu);
	dma_transfer(emu);
}
