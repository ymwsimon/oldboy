/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instruction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:54:16 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/22 14:25:50 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

t_setw	*g_setw_fptr[256] = {
	NULL, set_bc, NULL, set_bc, set_b, set_b, set_b, NULL,
	NULL, NULL, set_a, set_bc, set_c, set_c, set_c, NULL,
	NULL, set_de, NULL, set_de, set_d, set_d, set_d, NULL,
	NULL, NULL, set_a, set_de, set_e, set_e, set_e, NULL,
	NULL, set_hl, NULL, set_hl, set_h, set_h, set_h, NULL,
	NULL, NULL, set_a, set_hl, set_l, set_l, set_l, NULL,
	NULL, set_sp, NULL, set_sp, NULL, NULL, NULL, NULL,
	NULL, NULL, set_a, set_sp, set_a, set_a, set_a, NULL,
	set_b, set_b, set_b, set_b, set_b, set_b, set_b, set_b,
	set_c, set_c, set_c, set_c, set_c, set_c, set_c, set_c,
	set_d, set_d, set_d, set_d, set_d, set_d, set_d, set_d,
	set_e, set_e, set_e, set_e, set_e, set_e, set_e, set_e,
	set_h, set_h, set_h, set_h, set_h, set_h, set_h, set_h,
	set_l, set_l, set_l, set_l, set_l, set_l, set_l, set_l,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	set_a, set_a, set_a, set_a, set_a, set_a, set_a, set_a,
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
	NULL, NULL, a_of, bc_of, b_of, b_of, NULL, NULL,
	NULL, NULL, bc_of, bc_of, c_of, c_of, NULL, NULL,
	NULL, NULL, a_of, de_of, d_of, d_of, NULL, NULL,
	NULL, NULL, de_of, de_of, e_of, e_of, NULL, NULL,
	NULL, NULL, a_of, hl_of, h_of, h_of, NULL, NULL,
	NULL, NULL, hl_of, hl_of, l_of, l_of, NULL, NULL,
	NULL, NULL, a_of, sp_of, hl_of, hl_of, NULL, NULL,
	NULL, NULL, hl_of, sp_of, a_of, a_of, NULL, NULL,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, NULL, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of,
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

t_word	read_pc_byte_tick(t_emu *emu);

void	nop(t_emu *emu, t_byte op_code)
{
	emu_tick(emu, 4);
	(void)op_code;
}

void	stop(t_emu *emu, t_byte op_code)
{
	emu_tick(emu, 4);
	read_pc_byte_tick(emu);
	emu->paused = TRUE;
	(void)op_code;
}

void	halt(t_emu *emu, t_byte op_code)
{
	emu_tick(emu, 4);
	emu->cpu.halted = TRUE;
	(void)op_code;
}

void	ld_rr_d16(t_emu *emu, t_byte op_code)
{
	t_word	data;
	t_setw	*setw;

	emu_tick(emu, 4);
	setw = g_setw_fptr[op_code];
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

	emu_tick(emu, 4);
	setw = g_setw_fptr[op_code];
	addr = bus_read(emu, emu->cpu.pc) << 8;
	++(emu->cpu.pc);
	emu_tick(emu, 4);
	addr += bus_read(emu, emu->cpu.pc);
	++(emu->cpu.pc);
	setw(&emu->cpu, addr);
	emu_tick(emu, 4);
}

void	inc_dec_rr(t_emu *emu, t_byte op_code, int value)
{
	t_getw	*getw;
	t_setw	*setw;

	emu_tick(emu, 4);
	getw = g_getw_fptr[op_code];
	setw = g_setw_fptr[op_code];
	setw(&emu->cpu, getw(emu->cpu) + value);
	emu_tick(emu, 4);
}

void	inc_rr(t_emu *emu, t_byte op_code)
{
	inc_dec_rr(emu, op_code, 1);
}

void	dec_rr(t_emu *emu, t_byte op_code)
{
	inc_dec_rr(emu, op_code, -1);
}

t_word	read_mrr_idhl_tick(t_emu *emu, t_byte op_code)
{
	t_word	res;
	t_getw	*getw;

	getw = g_getw_fptr[op_code];
	res = getw(emu->cpu);
	res = bus_read(emu, res);
	if (op_code == 0x2A)
		set_hl(&emu->cpu, hl_of(emu->cpu) + 1);
	if (op_code == 0x3A)
		set_hl(&emu->cpu, hl_of(emu->cpu) - 1);
	emu_tick(emu, 4);
	return (res);
}

void	write_mrr_idhl_tick(t_emu *emu, t_byte op_code, t_byte data)
{
	t_word	addr;

	if (op_code == 0x02)
		addr = bc_of(emu->cpu);
	else if (op_code == 0x12)
		addr = de_of(emu->cpu);
	else
		addr = hl_of(emu->cpu);
	bus_write(emu, addr, data);
	if (op_code == 0x22)
		set_hl(&emu->cpu, hl_of(emu->cpu) + 1);
	if (op_code == 0x32)
		set_hl(&emu->cpu, hl_of(emu->cpu) - 1);
	emu_tick(emu, 4);
}

t_word	read_pc_byte_tick(t_emu *emu)
{
	t_word	res;

	res = bus_read(emu, emu->cpu.pc);
	++(emu->cpu.pc);
	emu_tick(emu, 4);
	return (res);
}

t_word	read_pc_word_tick(t_emu *emu)
{
	t_word	res;

	res = read_pc_byte_tick(emu);
	res <<= 8;
	res += read_pc_byte_tick(emu);
	return (res);
}

void	inc_dec_r(t_emu *emu, t_byte op_code, int value)
{
	t_getw	*getw;
	t_setw	*setw;
	t_word	result;

	emu_tick(emu, 4);
	getw = g_getw_fptr[op_code];
	setw = g_setw_fptr[op_code];
	if (op_code == 0x34 || op_code == 0x35)
		result = read_mrr_idhl_tick(emu, op_code);
	else
		result = getw(emu->cpu);
	result += value;
	set_flag_z(&emu->cpu, !(result & 0xFF));
	if (value == 1)
		set_flag_n(&emu->cpu, 0);
	else
		set_flag_n(&emu->cpu, 1);
	set_flag_h(&emu->cpu, !(result & 0x0F));
	if (op_code == 0x34 || op_code == 0x35)
		write_mrr_idhl_tick(emu, op_code, result);
	else
		setw(&emu->cpu, result);
}

void	inc_r(t_emu *emu, t_byte op_code)
{
	inc_dec_r(emu, op_code, 1);
}

void	dec_r(t_emu *emu, t_byte op_code)
{
	inc_dec_r(emu, op_code, -1);
}

void	ld_r_r(t_emu *emu, t_byte op_code)
{
	t_getw	*getw;
	t_setw	*setw;
	t_word	res;

	emu_tick(emu, 4);
	getw = g_getw_fptr[op_code];
	setw = g_setw_fptr[op_code];
	if ((op_code >= 0x40
			&& ((op_code & 0x0F) == 0x06 || (op_code & 0x0F) == 0x0E))
		|| (op_code < 0x40 && (op_code & 0x0F) == 0x0A))
		res = read_mrr_idhl_tick(emu, op_code);
	else if ((op_code & 0x0F) == 0x06 || (op_code & 0x0F) == 0x0E)
		res = read_pc_byte_tick(emu);
	else
		res = getw(emu->cpu);
	if (op_code >= 0x70 && op_code <= 0x77)
		write_mrr_idhl_tick(emu, op_code, res);
	else
		setw(&emu->cpu, res);
}

t_inst	*g_op_map[256] = {
	nop, ld_rr_d16, NULL, inc_rr, inc_r, dec_r, NULL, NULL,
	NULL, NULL, NULL, dec_rr, inc_r, dec_r, NULL, NULL,
	stop, ld_rr_d16, NULL, inc_rr, inc_r, dec_r, NULL, NULL,
	NULL, NULL, NULL, dec_rr, inc_r, dec_r, NULL, NULL,
	NULL, ld_rr_d16, NULL, inc_rr, inc_r, dec_r, NULL, NULL,
	NULL, NULL, NULL, dec_rr, inc_r, dec_r, NULL, NULL,
	NULL, ld_rr_d16, NULL, inc_rr, inc_r, dec_r, NULL, NULL,
	NULL, NULL, NULL, dec_rr, inc_r, dec_r, NULL, NULL,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, halt, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
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
