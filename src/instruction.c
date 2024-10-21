/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instruction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:54:16 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/21 23:48:21 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

t_setw	*g_setw_fptr[256] = {
	NULL, set_bc, NULL, set_bc, set_b, set_b, NULL, NULL,
	NULL, NULL, NULL, NULL, set_c, set_c, NULL, NULL,
	NULL, set_de, NULL, set_de, set_d, set_d, NULL, NULL,
	NULL, NULL, NULL, NULL, set_e, set_e, NULL, NULL,
	NULL, set_hl, NULL, set_hl, set_h, set_h, NULL, NULL,
	NULL, NULL, NULL, NULL, set_l, set_l, NULL, NULL,
	NULL, set_sp, NULL, set_sp, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, set_a, set_a, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, set_pc, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

t_getw	*g_getw_fptr[256] = {
	NULL, NULL, NULL, bc_of, b_of, b_of, NULL, NULL,
	NULL, NULL, NULL, NULL, c_of, c_of, NULL, NULL,
	NULL, NULL, NULL, de_of, d_of, d_of, NULL, NULL,
	NULL, NULL, NULL, NULL, e_of, e_of, NULL, NULL,
	NULL, NULL, NULL, hl_of, h_of, h_of, NULL, NULL,
	NULL, NULL, NULL, NULL, l_of, l_of, NULL, NULL,
	NULL, NULL, NULL, sp_of, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, a_of, a_of, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

void	test_op(t_emu *emu, t_byte op_code)
{
	t_setw	*setw;

	setw = g_setw_fptr[op_code];
	printf("tf:%p setw:%p\n", g_setw_fptr[op_code], set_bc);
	printf("before:");
	print_cpu_register(&emu->cpu);
	setw(&emu->cpu, bus_read(emu, emu->cpu.pc));
	printf("after:");
	print_cpu_register(&emu->cpu);
}

void	nop(t_emu *emu, t_byte op_code)
{
	(void)op_code;
	emu_tick(emu, 4);
}

void	ld_rr_d16(t_emu *emu, t_byte op_code)
{
	t_word	data;
	t_setw	*setw;

	setw = g_setw_fptr[op_code];
	emu_tick(emu, 4);
	data = bus_read(emu, emu->cpu.pc) << 8;
	++(emu->cpu.pc);
	emu_tick(emu, 4);
	data += bus_read(emu, emu->cpu.pc);
	++(emu->cpu.pc);
	setw(&emu->cpu, data);
	emu_tick(emu, 4);
}

void	jp_d16(t_emu *emu, t_byte op_code)
{
	t_word	addr;
	t_setw	*setw;

	setw = g_setw_fptr[op_code];
	addr = bus_read(emu, emu->cpu.pc) << 8;
	++(emu->cpu.pc);
	emu_tick(emu, 4);
	addr += bus_read(emu, emu->cpu.pc);
	++(emu->cpu.pc);
	emu_tick(emu, 4);
	setw(&emu->cpu, addr);
	emu_tick(emu, 4);
}

void	inc_rr(t_emu *emu, t_byte op_code)
{
	t_getw	*getw;
	t_setw	*setw;

	getw = g_getw_fptr[op_code];
	setw = g_setw_fptr[op_code];
	emu_tick(emu, 4);
	setw(&emu->cpu, getw(emu->cpu) + 1);
	emu_tick(emu, 4);
}

void	dec_rr(t_emu *emu, t_byte op_code)
{
	t_getw	*getw;
	t_setw	*setw;

	getw = g_getw_fptr[op_code];
	setw = g_setw_fptr[op_code];
	emu_tick(emu, 4);
	setw(&emu->cpu, getw(emu->cpu) - 1);
	emu_tick(emu, 4);
}

t_word	read_mhl_tick(t_emu *emu)
{
	t_word	res;

	res = bus_read(emu, hl_of(emu->cpu));
	emu_tick(emu, 4);
	return (res);
}

void	write_mhl_tick(t_emu *emu, t_byte data)
{
	bus_write(emu, hl_of(emu->cpu), data);
	emu_tick(emu, 4);
}

void	inc_dec_r(t_emu *emu, t_byte op_code, int value)
{
	t_getw	*getw;
	t_setw	*setw;
	t_word	result;

	getw = g_getw_fptr[op_code];
	setw = g_setw_fptr[op_code];
	if (op_code == 0x34)
		result = read_mhl_tick(emu);
	else
		result = getw(emu->cpu);
	result += 1;
	set_flag_z(&emu->cpu, !(result & 0xFF));
	if (value == 1)
		set_flag_n(&emu->cpu, 0);
	else
		set_flag_n(&emu->cpu, 1);
	set_flag_h(&emu->cpu, result & 0x08);
	if (op_code == 0x34)
		write_mhl_tick(emu, result);
	else
		setw(&emu->cpu, result);
	emu_tick(emu, 4);
}

void	inc_r(t_emu *emu, t_byte op_code)
{
	inc_dec_r(emu, op_code, 1);
}

void	dec_r(t_emu *emu, t_byte op_code)
{
	inc_dec_r(emu, op_code, -1);
}

t_inst	*g_op_map[256] = {
	nop, ld_rr_d16, NULL, inc_rr, inc_r, dec_r, NULL, NULL,
	NULL, NULL, NULL, dec_rr, inc_r, dec_r, NULL, NULL,
	NULL, ld_rr_d16, NULL, inc_rr, inc_r, dec_r, NULL, NULL,
	NULL, NULL, NULL, dec_rr, inc_r, dec_r, NULL, NULL,
	NULL, ld_rr_d16, NULL, inc_rr, inc_r, dec_r, NULL, NULL,
	NULL, NULL, NULL, dec_rr, inc_r, dec_r, NULL, NULL,
	NULL, ld_rr_d16, NULL, inc_rr, inc_r, dec_r, NULL, NULL,
	NULL, NULL, NULL, dec_rr, inc_r, dec_r, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, jp_d16, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};
