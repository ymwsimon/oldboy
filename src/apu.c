/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:10:16 by mayeung           #+#    #+#             */
/*   Updated: 2024/12/17 23:41:54 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	init_apu(t_emu *emu)
{
	(void)emu;
}

void	apu_write(t_emu *emu, t_word addr, t_byte data)
{
	(void)emu;
	(void)addr;
	(void)data;
}

t_byte	apu_read(t_emu *emu, t_word addr)
{
	(void)emu;
	(void)addr;
	return (0xFF);
}
