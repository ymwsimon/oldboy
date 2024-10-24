/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instruction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:54:16 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/24 14:13:19 by mayeung          ###   ########.fr       */
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
	set_a, set_a, set_a, set_a, set_a, set_a, set_a, set_a,
	set_a, set_a, set_a, set_a, set_a, set_a, set_a, set_a,
	set_a, set_a, set_a, set_a, set_a, set_a, set_a, set_a,
	set_a, set_a, set_a, set_a, set_a, set_a, set_a, set_a,
	set_a, set_a, set_a, set_a, set_a, set_a, set_a, set_a,
	set_a, set_a, set_a, set_a, set_a, set_a, set_a, set_a,
	set_a, set_a, set_a, set_a, set_a, set_a, set_a, set_a,
	set_id, set_id, set_id, set_id, set_id, set_id, set_id, set_id,
	NULL, NULL, NULL, set_pc, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, set_a, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, set_a, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, set_a, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, set_a, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, set_a, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, set_a, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, set_id, NULL
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
	res += read_pc_byte_tick(emu) << 8;
	return (res);
}

void	nop(t_emu *emu, t_byte op_code)
{
	emu_tick(emu, 4);
	(void)op_code;
}

void	stop(t_emu *emu, t_byte op_code)
{
	emu_tick(emu, 4);
	++(emu->cpu.pc);
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
	data = read_pc_word_tick(emu);
	setw(&emu->cpu, data);
}

void	jp_d16(t_emu *emu, t_byte op_code)
{
	t_word	addr;
	t_setw	*setw;

	emu_tick(emu, 4);
	setw = g_setw_fptr[op_code];
	addr = read_pc_word_tick(emu);
	setw(&emu->cpu, addr);
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
	if ((result & 0xF0) != ((result - value) & 0xF0))
		set_flag_h(&emu->cpu, 1);
	else
		set_flag_h(&emu->cpu, 0);
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
	if ((op_code >= 0x70 && op_code <= 0x77)
		|| (op_code < 0x40 && (op_code & 0x0F) == 0x02) || op_code == 0x36)
		write_mrr_idhl_tick(emu, op_code, res);
	else
		setw(&emu->cpu, res);
}

t_byte	add_addc(t_cpu *cpu, t_byte op_code, t_byte v)
{
	t_byte	res;
	t_byte	half_byte;
	t_byte	carry;

	carry = 0;
	if (op_code > 0x87 && op_code != 0xC6)
		carry = get_flag_c(*cpu);
	res = a_of(*cpu) + v + carry;
	half_byte = res & 0x0F;
	set_flag_z(cpu, !(res & 0xFF));
	set_flag_n(cpu, 0);
	if (half_byte < (v & 0xF) || half_byte < (a_of(*cpu) & 0xF)
		|| half_byte < carry)
		set_flag_h(cpu, 1);
	else
		set_flag_h(cpu, 0);
	if (res < v || res < a_of(*cpu) || res < carry)
		set_flag_c(cpu, 1);
	else
		set_flag_c(cpu, 0);
	return (res);
}

t_byte	sub_subc_cp(t_cpu *cpu, t_byte op_code, t_byte v)
{
	t_byte	res;
	t_byte	carry;

	carry = 0;
	if (op_code > 0x97 && op_code <= 0x9F)
		carry = get_flag_c(*cpu);
	res = a_of(*cpu) - v - carry;
	set_flag_z(cpu, !(res & 0xFF));
	set_flag_n(cpu, 1);
	if ((a_of(*cpu) & 0xF) < (v & 0xF)
		|| (a_of(*cpu) & 0xF) < carry
		|| ((a_of(*cpu) - carry) & 0xF) < (v & 0xF))
		set_flag_h(cpu, 1);
	else
		set_flag_h(cpu, 0);
	if (a_of(*cpu) < v || a_of(*cpu) < carry
		|| a_of(*cpu) - carry < v)
		set_flag_c(cpu, 1);
	else
		set_flag_c(cpu, 0);
	return (res);
}

t_byte and_xor_or(t_cpu *cpu, t_byte op_code, t_byte v)
{
	t_byte	res;

	if (op_code <= 0xA7)
		res = a_of(*cpu) & v;
	else if (op_code <= 0xAF)
		res = a_of(*cpu) ^ v;
	else
		res = a_of(*cpu) | v;
	set_flag_z(cpu, !(res & 0xFF));
	set_flag_n(cpu, 0);
	set_flag_h(cpu, 0);
	if (op_code <= 0xA7)
		set_flag_h(cpu, 1);
	set_flag_c(cpu, 0);
	return (res);
}

t_byte (*g_op_fptr[8])(t_cpu *, t_byte, t_byte) = {
add_addc, add_addc, sub_subc_cp, sub_subc_cp,
and_xor_or, and_xor_or, and_xor_or, sub_subc_cp};

void	bit_op(t_emu *emu, t_byte op_code)
{
	t_getw	*getw;
	t_setw	*setw;
	t_word	res;
	t_byte	idx;

	emu_tick(emu, 4);
	getw = g_getw_fptr[op_code];
	setw = g_setw_fptr[op_code];
	idx = (((op_code - 0x80) / 8) % 8);
	if (op_code <= 0xBF && ((op_code & 0xF) == 0x6 || (op_code & 0xF) == 0xE))
		res = read_mrr_idhl_tick(emu, op_code);
	else if ((op_code & 0xF) == 0x6 || (op_code & 0xF) == 0xE)
		res = read_pc_byte_tick(emu);
	else
		res = getw(emu->cpu);
	res = g_op_fptr[idx](&emu->cpu, op_code, res);
	setw(&emu->cpu, res);
}

void	jp(t_emu *emu, t_byte op_code)
{
	t_word			addr;

	emu_tick(emu, 4);
	if ((op_code & 0xF) == 0x0 || (op_code & 0xF) == 0x8)
	{
		addr = read_pc_byte_tick(emu);
		addr = emu->cpu.pc + (char)addr;
	}
	else if (op_code == 0xE9)
		emu->cpu.pc = hl_of(emu->cpu);
	else
		addr = read_pc_word_tick(emu);
	if (op_code == 0x18 || op_code == 0xC3
		|| (op_code == 0x20 && get_flag_nz(emu->cpu))
		|| (op_code == 0x30 && get_flag_nc(emu->cpu))
		|| (op_code == 0x28 && get_flag_z(emu->cpu))
		|| (op_code == 0x38 && get_flag_c(emu->cpu))
		|| (op_code == 0xC2 && get_flag_nz(emu->cpu))
		|| (op_code == 0xD2 && get_flag_nc(emu->cpu))
		|| (op_code == 0xCA && get_flag_z(emu->cpu))
		|| (op_code == 0xDA && get_flag_c(emu->cpu)))
	{
		emu->cpu.pc = addr;
		emu_tick(emu, 4);
	}
}

void	di(t_emu *emu, t_byte op_code)
{
	emu_tick(emu, 4);
	(void)op_code;
	emu->cpu.ime = FALSE;
}
void	ei(t_emu *emu, t_byte op_code)
{
	emu_tick(emu, 4);
	(void)op_code;
	emu->cpu.ime = TRUE;
}

void	ld_m(t_emu *emu, t_byte op_code)
{
	t_word	data;

	emu_tick(emu, 4);
	if ((op_code & 0xF0) == 0xE0)
		data = a_of(emu->cpu);
	else if (op_code == 0xF0)
		data = bus_read(emu, read_pc_byte_tick(emu) + 0xFF00);
	else if (op_code == 0xF2)
		data = bus_read(emu, c_of(emu->cpu) + 0xFF00);
	else if (op_code == 0xFA)
		data = bus_read(emu, read_pc_word_tick(emu));
	if ((op_code & 0xF0) == 0xF0)
		set_a(&emu->cpu, data);
	else if (op_code == 0xE0)
		bus_write(emu, read_pc_byte_tick(emu) + 0xFF00, data);
	else if (op_code == 0xE2)
		bus_write(emu, c_of(emu->cpu) + 0xFF00, data);
	else if (op_code == 0xEA)
		bus_write(emu, read_pc_word_tick(emu), data);
}

t_inst	*g_op_map[256] = {
	nop, ld_rr_d16, ld_r_r, inc_rr, inc_r, dec_r, ld_r_r, NULL,
	NULL, NULL, ld_r_r, dec_rr, inc_r, dec_r, ld_r_r, NULL,
	stop, ld_rr_d16, ld_r_r, inc_rr, inc_r, dec_r, ld_r_r, NULL,
	jp, NULL, ld_r_r, dec_rr, inc_r, dec_r, ld_r_r, NULL,
	jp, ld_rr_d16, ld_r_r, inc_rr, inc_r, dec_r, ld_r_r, NULL,
	jp, NULL, ld_r_r, dec_rr, inc_r, dec_r, ld_r_r, NULL,
	jp, ld_rr_d16, ld_r_r, inc_rr, inc_r, dec_r, ld_r_r, NULL,
	jp, NULL, ld_r_r, dec_rr, inc_r, dec_r, ld_r_r, NULL,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, halt, ld_r_r,
	ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r, ld_r_r,
	bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op,
	bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op,
	bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op,
	bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op,
	bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op,
	bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op,
	bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op,
	bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op, bit_op,
	NULL, NULL, jp, jp, NULL, NULL, bit_op, NULL,
	NULL, NULL, jp, NULL, NULL, NULL, bit_op, NULL,
	NULL, NULL, jp, NULL, NULL, NULL, bit_op, NULL,
	NULL, NULL, jp, NULL, NULL, NULL, bit_op, NULL,
	ld_m, NULL, ld_m, NULL, NULL, NULL, bit_op, NULL,
	NULL, jp, ld_m, NULL, NULL, NULL, bit_op, NULL,
	ld_m, NULL, ld_m, di, NULL, NULL, bit_op, NULL,
	NULL, NULL, ld_m, ei, NULL, NULL, bit_op, NULL
};
