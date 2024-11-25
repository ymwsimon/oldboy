/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:42:59 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/25 13:45:12 by mayeung          ###   ########.fr       */
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
	emu->ppu.lcdc = 0x91;
	emu->ppu.lyc = 0;
	emu->ppu.dma_write_counter = 0;
	emu->ppu.dma = 0;
	emu->ppu.bgp = 0xFC;
	emu->ppu.obp0 = 0xFF;
	emu->ppu.obp1 = 0xFF;
	emu->ppu.win_xl = 0;
	emu->ppu.win_yl = 0;
	bzero(&emu->ppu.oam, sizeof(t_byte) * 0xA0);
}

void	ppu_write(t_emu *emu, t_word addr, t_byte data)
{
	if (addr == 0xFF40 && (emu->ppu.ppu_mode == VBLANK || (data & 0x80)))
		emu->ppu.lcdc = data;
	if (addr == 0xFF40 && emu->ppu.ppu_mode != VBLANK && !(data & 0x80))
	{
		emu->ppu.lcdc = 0x80 | (data & ~0x80);
		emu->ppu.ly = 0;
	}
	if (addr == 0xFF41)
		emu->ppu.stat = (data & ~7) | (emu->ppu.stat & 7);
	if (addr == 0xFF42)
		emu->ppu.scy = data;
	if (addr == 0xFF43)
		emu->ppu.scx = data;
	if (addr == 0xFF45)
		emu->ppu.lyc = data;
	if (addr == 0xFF46)
	{
		emu->ppu.dma = data;
		emu->ppu.dma_write_counter = 644;
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
	if (addr >= 0xFE00 && addr <= 0xFE9F
		&& (emu->ppu.ppu_mode == HBLANK || emu->ppu.ppu_mode == VBLANK))
		emu->ppu.oam[addr - 0xFE00] = data;
}

t_byte	ppu_read(t_emu *emu, t_word addr)
{
	if (addr == 0xFF40)
		return (emu->ppu.lcdc);
	if (addr == 0xFF41 && is_ppu_enabled(emu))
		return (0x80 | emu->ppu.stat);
	if (addr == 0xFF41 && !is_ppu_enabled(emu))
		return (0x80 | (emu->ppu.stat & ~3));
	if (addr == 0xFF42)
		return (emu->ppu.scy);
	if (addr == 0xFF43)
		return (emu->ppu.scx);
	if (addr == 0xFF44 && is_ppu_enabled(emu))
		return (emu->ppu.ly);
	if (addr == 0xFF44 && !is_ppu_enabled(emu))
		return (0x00);
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
	if (addr >= 0xFE00 && addr <= 0xFE9F
		&& (emu->ppu.ppu_mode == HBLANK || emu->ppu.ppu_mode == VBLANK))
		return (emu->ppu.oam[addr - 0xFE00]);
	return (0xFF);
}

t_byte	get_obj_size(t_emu *emu)
{
	if (emu->ppu.lcdc & 4)
		return (16);
	return (8);
}

void	swap_elem(t_emu *emu, t_byte i, t_byte j)
{
	t_byte	temp;

	temp = emu->ppu.object_queue[i];
	emu->ppu.object_queue[i] = emu->ppu.object_queue[j];
	emu->ppu.object_queue[j] = temp;
}

void 	sort_obj_queue(t_emu *emu)
{
	t_byte	i;
	t_byte	j;

	i = 0;
	while (i < emu->ppu.num_obj_scanline - 1)
	{
		j = i + 1;
		while (j < emu->ppu.num_obj_scanline)
		{
			if (emu->ppu.oam[emu->ppu.object_queue[j] * 4 + 1]
				< emu->ppu.oam[emu->ppu.object_queue[i] * 4 + 1]
				|| (emu->ppu.oam[emu->ppu.object_queue[j] * 4 + 1]
					== emu->ppu.oam[emu->ppu.object_queue[i] * 4 + 1]
					&& emu->ppu.object_queue[j] < emu->ppu.object_queue[i]))
				swap_elem(emu, i, j);
			++j;
		}
		++i;
	}
	if (emu->ppu.num_obj_scanline > 10)
		emu->ppu.num_obj_scanline = 10;
}

void	scan_obj(t_emu *emu)
{
	t_byte	idx;
	t_byte	obj_size;

	emu->ppu.num_obj_scanline = 0;
	idx = 0;
	obj_size = get_obj_size(emu);
	while (idx < 40 && (emu->ppu.lcdc & 2))
	{
		if ((emu->ppu.ly + 16 >= emu->ppu.oam[idx * 4]
				&& emu->ppu.ly + 16 < emu->ppu.oam[idx * 4] + obj_size))
		{
			emu->ppu.object_queue[emu->ppu.num_obj_scanline] = idx;
			++emu->ppu.num_obj_scanline;
		}
		++idx;
	}
	sort_obj_queue(emu);
}

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
	int		offset;
	t_byte	obj_size;

	offset = emu->ppu.oam[oid + 2] * 16;
	obj_size = get_obj_size(emu);
	if (obj_size == 16)
		offset = (emu->ppu.oam[oid + 2] & ~1) * 16;
	if (need_yflip(emu, oid))
		offset += (obj_size - 1
				- ((emu->ppu.ly - (emu->ppu.oam[oid] - 16))) % obj_size) * 2;
	else
		offset += ((emu->ppu.ly - (emu->ppu.oam[oid] - 16)) % obj_size) * 2;
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

int	lx_is_within_object(t_emu *emu, t_byte oid)
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

t_byte	get_obj_palette(t_byte attr)
{
	if (!(attr & 16))
		return (OBJ0_TILE);
	return (OBJ1_TILE);
}

unsigned int	get_colour_from_palette(t_emu *emu,
	t_byte cid, t_byte tile_type)
{
	static unsigned int	colour_map[4] = {WHITE, LIGHT_GREEN, DARK_GREEN, BLACK};
	t_word				palette_reg;

	if (tile_type == BG_WIN_TILE)
		palette_reg = emu->ppu.bgp;
	else if (tile_type == OBJ0_TILE)
		palette_reg = emu->ppu.obp0;
	else
		palette_reg = emu->ppu.obp1;
	return (colour_map[(palette_reg >> (2 * cid)) & 3]);
}

t_byte	get_cid_priority(t_byte bgwin_cid, t_byte cid, t_byte attr)
{
	if ((attr & 128) && bgwin_cid > 0)
		return (0);
	return (cid);
}

unsigned int	get_obj_over_bgwin_colour(t_emu *emu, t_byte bgwin_cid)
{
	t_byte	cid;
	t_byte	idx;
	t_byte	oid;
	int		offset;

	idx = 0;
	while (object_enabled(emu) && idx < emu->ppu.num_obj_scanline)
	{
		oid = emu->ppu.object_queue[idx] * 4;
		if (lx_is_within_object(emu, oid))
		{
			offset = get_offset_pj_from_objectid(emu, oid);
			cid = get_cid_from(emu, offset, get_pi_from_objectid(emu, oid));
			cid = get_cid_priority(bgwin_cid, cid, (emu->ppu.oam[oid + 3]));
			if (cid)
				break ;
		}
		++idx;
	}
	if (cid)
		return (get_colour_from_palette(emu, cid,
				get_obj_palette(emu->ppu.oam[oid + 3])));
	return (get_colour_from_palette(emu, bgwin_cid, BG_WIN_TILE));
}

t_word	get_tid_from_tile_map(t_emu *emu, t_word tid, t_byte is_bg)
{
	if (is_bg)
	{
		if (!(emu->ppu.lcdc & 8))
			tid = emu->vram[0x1800 + tid];
		else
			tid = emu->vram[0x1C00 + tid];
	}
	else
	{
		if (!(emu->ppu.lcdc & 64))
			tid = emu->vram[0x1800 + tid];
		else
			tid = emu->vram[0x1C00 + tid];
	}
	return (tid);
}

t_byte	get_bg_win_cid(t_emu *emu, t_word li, t_word lj, t_byte is_bg)
{
	t_word	tid;
	t_byte	cid;
	int		offset;

	tid = lj / 8 * SCREEN_NUM_TILE_PER_ROW + li / 8;
	tid = get_tid_from_tile_map(emu, tid, is_bg);
	offset = (char)tid * 16 + (lj % 8) * 2;
	if (emu->ppu.lcdc & 16)
		offset = tid * 16 + (lj % 8) * 2;
	if (!(emu->ppu.lcdc & 16))
		offset += 0x1000;
	cid = get_cid_from(emu, offset, 7 - li % 8);
	if (!(emu->ppu.lcdc & 1))
		cid = 0;
	return (cid);
}

t_byte	get_win_cid(t_emu *emu)
{
	t_word		li;
	t_word		lj;

	li = emu->ppu.win_xl;
	lj = emu->ppu.win_yl;
	++(emu->ppu.win_xl);
	if (emu->ppu.lx - 80 >= 159)
	{
		emu->ppu.win_xl = 0;
		++(emu->ppu.win_yl);
	}
	return (get_bg_win_cid(emu, li, lj, FALSE));
}

t_byte	get_bg_cid(t_emu *emu)
{
	t_word		li;
	t_word		lj;

	li = (emu->ppu.lx - 80 + emu->ppu.scx) % 256;
	lj = (emu->ppu.ly + emu->ppu.scy) % 256;
	return (get_bg_win_cid(emu, li, lj, TRUE));
}

void	ppu_draw_pix(t_emu *emu)
{
	unsigned int	colour;
	t_byte			cid;
	SDL_Surface		*s;

	cid = get_bg_cid(emu);
	if (is_window_covered(emu))
		cid = get_win_cid(emu);
	colour = get_obj_over_bgwin_colour(emu, cid);
	s = SDL_GetWindowSurface(emu->window);
	SDL_LockSurface(s);
	print_pixel(emu, s, colour, (t_tile_pix_info){(emu->ppu.lx - 80) / 8,
		emu->ppu.ly / 8, (emu->ppu.lx - 80) % 8, emu->ppu.ly % 8});
	SDL_UnlockSurface(s);
}

int	is_vblank(t_emu *emu)
{
	return (emu->ppu.ly >= 144 && emu->ppu.ly <= 153);
}

int	is_switching_to_next_line(t_emu *emu)
{
	return (emu->ppu.lx == 456);
}

void	non_vblank(t_emu *emu)
{
	if (!is_vblank(emu))
	{
		if (is_switching_to_next_line(emu))
		{
			emu->ppu.lx = 0;
			emu->ppu.ppu_mode = OAM_SCAN;
			++(emu->ppu.ly);
		}
		else if (emu->ppu.lx == 240)
			emu->ppu.ppu_mode = HBLANK;
		else if (emu->ppu.lx == 80)
			emu->ppu.ppu_mode = DRAWING;
	}
}

void	check_last_dot_of_line(t_emu *emu)
{
	if (is_switching_to_next_line(emu))
	{
		emu->ppu.lx = 0;
		++(emu->ppu.ly);
	}
}

int	is_switching_to_first_line(t_emu *emu)
{
	return (emu->ppu.ly == 154);
}

void	check_last_line_of_screen(t_emu *emu)
{
	if (is_switching_to_first_line(emu))
	{
		emu->ppu.ly = 0;
		emu->ppu.lx = 0;
		emu->ppu.ppu_mode = OAM_SCAN;
	}
}

int	is_switching_to_vblank(t_emu *emu)
{
	return (emu->ppu.ly == 144 && emu->ppu.lx == 0);
}

void	vblank(t_emu *emu)
{
	if (is_switching_to_vblank(emu))
	{
		emu->ppu.ppu_mode = VBLANK;
		emu->ppu.win_xl = 0;
		emu->ppu.win_yl = 0;
		SDL_UpdateWindowSurface(emu->window);
	}
}

void	dma_transfer(t_emu *emu)
{
	if (emu->ppu.dma_write_counter)
	{
		if (emu->ppu.dma_write_counter <= 640
			&& !(emu->ppu.dma_write_counter % 4))
		{
			emu->ppu.oam[160 - emu->ppu.dma_write_counter / 4]
				= bus_read(emu, (emu->ppu.dma << 8)
					+ (160 - emu->ppu.dma_write_counter / 4));
		}
		--(emu->ppu.dma_write_counter);
	}
}

int	is_ppu_enabled(t_emu *emu)
{
	return (emu->ppu.lcdc & 128);
}

void	update_ppu_stat_mode(t_emu *emu)
{
	emu->ppu.stat &= ~3;
	emu->ppu.stat |= emu->ppu.ppu_mode;
}

void	update_ly_eq_lyc(t_emu *emu)
{
	emu->ppu.stat &= ~4;
	emu->ppu.stat |= (emu->ppu.ly == emu->ppu.lyc) * 4;
}

void	check_ppu_stat_interrupt(t_emu *emu)
{
	if (is_ppu_enabled(emu) && ((emu->ppu.ly < 144 && ((emu->ppu.lx == 0 && (emu->ppu.stat & 32))
				|| (emu->ppu.lx == 240 && (emu->ppu.stat & 8))))
		|| (is_switching_to_vblank(emu) && (emu->ppu.stat & 16))
		|| ((emu->ppu.stat & 4) && (emu->ppu.stat & 64) && emu->ppu.lx == 0)))
		emu->interrupt_flag |= 2;
}

void	check_ppu_vblank_interrupt(t_emu *emu)
{
	if (is_switching_to_vblank(emu))
		emu->interrupt_flag |= 1;
}

void	ppu_tick(t_emu *emu)
{
	if (!is_ppu_enabled(emu))
		return ;
	++(emu->ppu.lx);
	non_vblank(emu);
	check_last_dot_of_line(emu);
	check_last_line_of_screen(emu);
	vblank(emu);
	update_ppu_stat_mode(emu);
	update_ly_eq_lyc(emu);
	check_ppu_stat_interrupt(emu);
	check_ppu_vblank_interrupt(emu);
	if (emu->ppu.ppu_mode == OAM_SCAN && emu->ppu.lx == 0)
		scan_obj(emu);
	if (emu->ppu.ppu_mode == DRAWING && is_ppu_enabled(emu))
		ppu_draw_pix(emu);
	dma_transfer(emu);
}
