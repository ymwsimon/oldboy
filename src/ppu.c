/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:42:59 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/10 14:03:09 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	init_ppu(t_emu *emu)
{
	emu->ppu.ly = 0;
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

void	ppu_tick(t_emu *emu)
{
	++(emu->ppu.ly);
	if (emu->ppu.ly > 152)
		emu->ppu.ly = 0;
}
