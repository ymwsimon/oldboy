/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bus.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:19:31 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/24 14:48:22 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

t_byte	bus_read(t_emu *emu, t_word addr)
{
	if (addr < 0x8000)
		return (emu->cart.data[addr]);
	if (addr < 0xA000)
		return (emu->vram[addr - 0x8000]);
	if (addr < 0xE000)
		return (emu->wram[addr - 0xA000]);
	printf("Unknown read addr for bus (%4X)\n", addr);
	return (0xFF);
}

void	bus_write(t_emu *emu, t_word addr, t_byte data)
{
	if (addr < 0x8000)
		return ;
	if (addr < 0xA000)
		emu->vram[addr - 0x8000] = data;
	if (addr < 0xE000)
		emu->wram[addr - 0xC000] = data;
	if (addr >= 0xE000)
		printf("Unknow write addr for bus (%4X) data:%2X\n", addr, data);
}
