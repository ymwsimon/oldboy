/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bus.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:19:31 by mayeung           #+#    #+#             */
/*   Updated: 2024/12/09 21:29:11 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

t_byte	bus_read(t_emu *emu, t_word addr)
{
	// if (emu->ppu.dma_write_counter && !(addr >= 0xFF80 && addr <= 0xFFFE))
		// return (0xFF);
	if (addr <= 0x7FFF || (addr >= 0xA000 && addr <= 0xBFFF))
		return (cart_read(emu, addr));
	if (addr >= 0x8000 && addr <= 0x9FFF)
		return (emu->vram[addr - 0x8000]);
	if (addr >= 0xC000 && addr <= 0xDFFF)
		return (emu->wram[addr - 0xC000]);
	if (addr >= 0xE000 && addr <= 0xFDFF)
		return (emu->wram[addr & 0x1FFF]);
	if (addr >= 0xFF80 && addr <= 0xFFFE)
		return (emu->hram[addr - 0xFF80]);
	if (addr == 0xFFFF)
		return (emu->interrupt_enable);
	if (addr == 0xFF0F)
		return (emu->interrupt_flag);
	if (addr == 0xFF01 || addr == 0xFF02)
		return (serial_read(emu, addr));
	if (addr >= 0xFF04 && addr <= 0xFF07)
		return (timer_read(emu, addr));
	if ((addr >= 0xFE00 && addr <= 0xFE9F)
		|| (addr >= 0xFF40 && addr <= 0xFF4B))
		return (ppu_read(emu, addr));
	if (addr == 0xFF00)
		return (input_read(emu));
	if (addr >= 0xFF10 && addr <= 0xFF3F)
		return (apu_read(emu, addr));
	// printf("Unknown read addr for bus (%4X)\n", addr);
	return (0xFF);
}

void	bus_write(t_emu *emu, t_word addr, t_byte data)
{
	// if (emu->ppu.dma_write_counter && !(addr >= 0xFF80 && addr <= 0xFFFE))
		// return ;
	if (addr <= 0x7FFF || (addr >= 0xA000 && addr <= 0xBFFF))
		cart_write(emu, addr, data);
	else if (addr >= 0x8000 && addr <= 0x9FFF)
		emu->vram[addr - 0x8000] = data;
	else if (addr >= 0xC000 && addr <= 0xDFFF)
		emu->wram[addr - 0xC000] = data;
	else if (addr >= 0xE000 && addr <= 0xFDFF)
		emu->wram[addr & 0x1FFF] = data;
	else if (addr >= 0xFF80 && addr <= 0xFFFE)
		emu->hram[addr - 0xFF80] = data;
	else if (addr == 0xFFFF)
		emu->interrupt_enable = data | 0xE0;
	else if (addr == 0xFF0F)
		emu->interrupt_flag = data | 0xE0;
	else if (addr == 0xFF01 || addr == 0xFF02)
		serial_write(emu, addr, data);
	else if (addr >= 0xFF04 && addr <= 0xFF07)
		timer_write(emu, addr, data);
	else if ((addr >= 0xFE00 && addr <= 0xFE9F)
		|| (addr >= 0xFF40 && addr <= 0xFF4B))
		ppu_write(emu, addr, data);
	else if (addr == 0xFF00)
		input_write(emu, data);
	else if (addr >= 0xFF10 && addr <= 0xFF3F)
		apu_write(emu, addr, data);
	// else if (addr >= 0xE000 && addr < 0xFF80)
		// printf("Unknow write addr for bus (%4X) data:%2X\n", addr, data);
}
