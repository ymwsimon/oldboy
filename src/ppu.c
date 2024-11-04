/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:42:59 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/04 17:03:49 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	init_ppu(t_emu *emu)
{
	emu->ppu.ly = 0;
}

void	ppu_tick(t_emu *emu)
{
	++(emu->ppu.ly);
	if (emu->ppu.ly > 152)
		emu->ppu.ly = 0;
}
