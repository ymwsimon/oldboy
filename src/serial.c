/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serial.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 23:02:06 by mayeung           #+#    #+#             */
/*   Updated: 2024/12/13 21:12:36 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	init_serial(t_emu *emu)
{
	emu->serial.idx_out_buf = 0;
	emu->serial.sb = 0;
	emu->serial.sc = 0x7E;
	bzero(&emu->serial.serial_out_buf, 1000);
}

void	serial_write_byte_file(t_emu *emu)
{
	int	fd;
	int	flag;

	flag = O_WRONLY | O_CREAT | O_APPEND;
	if (emu->serial.idx_out_buf == 1)
		flag = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open("out_buf", flag, 0755);
	if (fd >= 0)
	{
		write(fd, emu->serial.serial_out_buf + emu->serial.idx_out_buf - 1, 1);
		close(fd);
	}
}

void	serial_write(t_emu *emu, t_word addr, t_byte data)
{
	if (addr == 0xFF01)
	{
		emu->serial.sb = data;
		emu->serial.serial_out_buf[emu->serial.idx_out_buf] = data;
		++(emu->serial.idx_out_buf);
		if (!emu->serial.idx_out_buf)
			memmove(emu->serial.serial_out_buf,
				emu->serial.serial_out_buf + 1, 0xFFFF);
		serial_write_byte_file(emu);
	}
	if (addr == 0xFF02)
		emu->serial.sc = data;
	if ((emu->serial.sc & 0x81) == 0x81)
	{
		emu->serial.io_byte = emu->serial.sb;
		emu->serial.io_byte_bit_idx = 0;
		emu->serial.transferring = TRUE;
	}
}

t_byte	serial_read(t_emu *emu, t_word addr)
{
	if (addr == 0xFF01)
		return (emu->serial.sb);
	if (addr == 0xFF02)
		return (emu->serial.sc);
	return (0xFF);
}

void	serial_tick(t_emu *emu)
{
	if (emu->serial.transferring)
	{
		emu->serial.io_byte <<= 1;
		emu->serial.io_byte |= 1;
		// emu->serial.io_byte |= (1 << emu->serial.io_byte_bit_idx);
		++(emu->serial.io_byte_bit_idx);
		if (emu->serial.io_byte_bit_idx == 8)
		{
			emu->serial.transferring = FALSE;
			emu->interrupt_flag |= 0x8;
			emu->serial.sb = emu->serial.io_byte;
			emu->serial.io_byte_bit_idx = 0;
		}
	}
}
