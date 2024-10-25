/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bus.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:19:31 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/25 20:54:43 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

t_byte	bus_read(t_emu *emu, t_word addr)
{
	if (addr < 0x8000)
		return (emu->cart.data[addr]);
	if (addr >= 0x8000 && addr < 0xA000)
		return (emu->vram[addr - 0x8000]);
	if (addr >= 0xC000 && addr < 0xE000)
		return (emu->wram[addr - 0xC000]);
	if (addr >= 0xFF80 && addr < 0xFFFF)
		return (emu->hram[addr - 0xFF80]);
	//workaround
	// if (addr == 0xFF44)
		// return (0x88);
		// return (printf("addr:%04X actual data:%02X\n", addr, emu->wram[addr-0xC000]), emu->wram[addr - 0xC000]);
	printf("Unknown read addr for bus (%4X)\n", addr);
	return (0xFF);
}

void	bus_write(t_emu *emu, t_word addr, t_byte data)
{
	if (addr < 0x8000)
		return ;
	if (addr >= 0x8000 && addr < 0xA000)
		emu->vram[addr - 0x8000] = data;
	if (addr >= 0xC000 && addr < 0xE000)
		emu->wram[addr - 0xC000] = data;
	if(addr >= 0xFF80 && addr < 0xFFFF)
		emu->hram[addr - 0xFF80] = data;
			// {emu->wram[addr - 0xC000] = data;printf("addr:%04X data:%02X actual data:%02X\n", addr, data , emu->wram[addr-0xC000]);}
	if (addr >= 0xE000 && addr < 0xFF80)
		printf("Unknow write addr for bus (%4X) data:%2X\n", addr, data);
}
