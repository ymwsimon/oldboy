/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bus.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:19:31 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/21 23:49:05 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

t_byte	bus_read(t_emu *emu, t_word addr)
{
	(void)addr;
	(void)emu;
	if (addr < 0x8000)
		return (emu->cart.data[addr]);
	if (addr < 0xA000)
		return (emu->vram[addr - 0x8000]);
	if (addr < 0xE000)
		return (emu->wram[addr - 0xA000]);
	return (0xFF);
}

void	bus_write(t_emu *emu, t_word addr, t_byte data)
{
	(void)addr;
	(void)data;
	(void)emu;
}
