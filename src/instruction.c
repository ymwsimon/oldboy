/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instruction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:54:16 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/29 11:50:36 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

t_setw	*g_setw_fptr[256] = {
	NULL, set_bc, NULL, set_bc, set_b, set_b, set_b, NULL,
	NULL, set_hl, set_a, set_bc, set_c, set_c, set_c, NULL,
	NULL, set_de, NULL, set_de, set_d, set_d, set_d, NULL,
	NULL, set_hl, set_a, set_de, set_e, set_e, set_e, NULL,
	NULL, set_hl, NULL, set_hl, set_h, set_h, set_h, NULL,
	NULL, set_hl, set_a, set_hl, set_l, set_l, set_l, NULL,
	NULL, set_sp, NULL, set_sp, NULL, NULL, NULL, NULL,
	NULL, set_hl, set_a, set_sp, set_a, set_a, set_a, NULL,
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
	set_pc, set_bc, NULL, set_pc, NULL, NULL, set_a, NULL,
	set_pc, set_pc, NULL, NULL, NULL, NULL, set_a, NULL,
	set_pc, set_de, NULL, NULL, NULL, NULL, set_a, NULL,
	set_pc, set_pc, NULL, NULL, NULL, NULL, set_a, NULL,
	NULL, set_hl, NULL, NULL, NULL, NULL, set_a, NULL,
	set_sp, NULL, NULL, NULL, NULL, NULL, set_a, NULL,
	NULL, set_af, NULL, NULL, NULL, NULL, set_a, NULL,
	set_hl, set_sp, NULL, NULL, NULL, NULL, set_id, NULL
};

t_getw	*g_getw_fptr[256] = {
	NULL, NULL, a_of, bc_of, b_of, b_of, NULL, NULL,
	sp_of, bc_of, bc_of, bc_of, c_of, c_of, NULL, NULL,
	NULL, NULL, a_of, de_of, d_of, d_of, NULL, NULL,
	NULL, de_of, de_of, de_of, e_of, e_of, NULL, NULL,
	NULL, NULL, a_of, hl_of, h_of, h_of, NULL, NULL,
	NULL, hl_of, hl_of, hl_of, l_of, l_of, NULL, NULL,
	NULL, NULL, a_of, sp_of, hl_of, hl_of, NULL, NULL,
	NULL, sp_of, hl_of, sp_of, a_of, a_of, NULL, NULL,
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
	NULL, NULL, NULL, NULL, NULL, bc_of, NULL, pc_of,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, pc_of,
	NULL, NULL, NULL, NULL, NULL, de_of, NULL, pc_of,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, pc_of,
	NULL, NULL, NULL, NULL, NULL, hl_of, NULL, pc_of,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, pc_of,
	NULL, NULL, NULL, NULL, NULL, af_of, NULL, pc_of,
	sp_of, hl_of, NULL, NULL, NULL, NULL, NULL, pc_of
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

void	write_word(t_emu *emu, t_word addr, t_word data)
{
	bus_write(emu, addr, data & 0xFF);
	emu_tick(emu, 4);
	bus_write(emu, addr + 1, data >> 8);
	emu_tick(emu, 4);
}

void	nop(t_emu *emu, t_byte op_code)
{
	(void)emu;
	(void)op_code;
}

void	stop(t_emu *emu, t_byte op_code)
{
	++(emu->cpu.pc);
	emu->paused = TRUE;
	(void)op_code;
}

void	halt(t_emu *emu, t_byte op_code)
{
	emu->cpu.halted = TRUE;
	(void)op_code;
}

void	set_flag_0xE8_0xF8(t_cpu *cpu, t_word sp, char offset)
{
	set_flag_z(cpu, 0);
	set_flag_n(cpu, 0);
	if (offset >= 0 && sp > (sp + offset))
		set_flag_c(cpu, 1);
	if (offset < 0 && (sp + offset) > sp)
		set_flag_c(cpu, 1);
	if ((sp & 0xF0) != ((sp + offset) & 0xF0))
		set_flag_h(cpu, 1);
}

void	ld_16(t_emu *emu, t_byte op_code)
{
	t_word	data;
	char	offset;

	if ((op_code & 0xF) == 0x1)
		data = read_pc_word_tick(emu);
	else if (op_code == 0xF8)
		offset = (char)read_pc_byte_tick(emu);
	else
		data = g_getw_fptr[op_code](emu->cpu);
	if (op_code == 0x8)
		write_word(emu, read_pc_word_tick(emu), data);
	else if (op_code == 0xF8)
		set_hl(&emu->cpu, sp_of(emu->cpu) + offset);
	else
		g_setw_fptr[op_code](&emu->cpu, data);
	if ((op_code & 0xF0) == 0xF0)
		emu_tick(emu, 4);
	if (op_code == 0xF8)
		set_flag_0xE8_0xF8(&emu->cpu, sp_of(emu->cpu), offset);
}

void	inc_dec_rr(t_emu *emu, t_byte op_code, int value)
{
	t_getw	*getw;
	t_setw	*setw;

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

	if (op_code <= 0xA7 || op_code == 0xE6)
		res = a_of(*cpu) & v;
	else if (op_code <= 0xAF || op_code == 0xEE)
		res = a_of(*cpu) ^ v;
	else
		res = a_of(*cpu) | v;
	set_flag_z(cpu, !(res & 0xFF));
	set_flag_n(cpu, 0);
	set_flag_h(cpu, 0);
	if (op_code <= 0xA7 || op_code == 0xE6)
		set_flag_h(cpu, 1);
	set_flag_c(cpu, 0);
	return (res);
}

void	bit_op(t_emu *emu, t_byte op_code)
{
	t_getw			*getw;
	t_setw			*setw;
	t_word			res;
	t_byte			idx;
	static t_byte	(*op_fptr[8])(t_cpu *, t_byte, t_byte) = {
		add_addc, add_addc, sub_subc_cp, sub_subc_cp,
		and_xor_or, and_xor_or, and_xor_or, sub_subc_cp};

	getw = g_getw_fptr[op_code];
	setw = g_setw_fptr[op_code];
	idx = (((op_code - 0x80) / 8) % 8);
	if (op_code <= 0xBF && ((op_code & 0xF) == 0x6 || (op_code & 0xF) == 0xE))
		res = read_mrr_idhl_tick(emu, op_code);
	else if ((op_code & 0xF) == 0x6 || (op_code & 0xF) == 0xE)
		res = read_pc_byte_tick(emu);
	else
		res = getw(emu->cpu);
	res = op_fptr[idx](&emu->cpu, op_code, res);
	setw(&emu->cpu, res);
}

void	jp(t_emu *emu, t_byte op_code)
{
	t_word			addr;

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
	(void)op_code;
	emu->cpu.ime = FALSE;
	emu->cpu.ime_countdown = 0;
}

void	ei(t_emu *emu, t_byte op_code)
{
	(void)op_code;
	emu->cpu.ime_countdown = 2;
}

void	ld_m(t_emu *emu, t_byte op_code)
{
	t_word	data;

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

void	push_word(t_emu *emu, t_word data)
{
	--(emu->cpu.sp);
	bus_write(emu, emu->cpu.sp, (data >> 8) & 0xFF);
	emu_tick(emu, 4);
	--(emu->cpu.sp);
	bus_write(emu, emu->cpu.sp, data & 0xFF);
	emu_tick(emu, 4);
	emu_tick(emu, 4);
}

void	push(t_emu *emu, t_byte op_code)
{
	t_word	data;

	data = g_getw_fptr[op_code](emu->cpu);
	push_word(emu, data);
}

void	pop(t_emu *emu, t_byte op_code)
{
	t_word	data;

	data = bus_read(emu, emu->cpu.sp);
	++(emu->cpu.sp);
	emu_tick(emu, 4);
	data += bus_read(emu, emu->cpu.sp) << 8;
	++(emu->cpu.sp);
	emu_tick(emu, 4);
	g_setw_fptr[op_code](&emu->cpu, data);
}

void	rst(t_emu *emu, t_byte op_code)
{
	t_byte			idx;
	const t_byte	addr[8] = {0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38};

	idx = ((op_code - 0xC0) / 8) % 8;
	push(emu, op_code);
	emu->cpu.pc = addr[idx];
}

void	ret_reti(t_emu *emu, t_byte op_code)
{
	t_byte		idx;
	static int	(*cc_arr[4])(t_cpu) = {
		get_flag_nz, get_flag_z, get_flag_nc, get_flag_c};

	idx = (op_code - 0xC0) / 8 % 8;
	if (((op_code & 0xF) != 0x0 && (op_code & 0xF) != 0x8)
		|| (((op_code & 0xF) == 0x0 || (op_code & 0xF) == 0x8)
			&& cc_arr[idx](emu->cpu)))
		pop(emu, op_code);
	if ((op_code & 0xF) == 0x0 || (op_code & 0xF) == 0x8)
		emu_tick(emu, 4);
	if (op_code == 0xD9)
		ei(emu, 0);
	emu_tick(emu, 4);
}

void	call(t_emu *emu, t_byte op_code)
{
	t_word		addr;
	t_byte		idx;
	static int	(*cc_arr[4])(t_cpu) = {
		get_flag_nz, get_flag_z, get_flag_nc, get_flag_c};

	idx = (op_code - 0xC4) / 8 % 8;
	addr = read_pc_word_tick(emu);
	if (op_code == 0xCD || (op_code != 0xCD && cc_arr[idx](emu->cpu)))
	{
		push_word(emu, pc_of(emu->cpu));
		emu->cpu.pc = addr;
	}
}

void	add_16(t_emu *emu, t_byte op_code)
{
	t_word	data;
	t_word	old_data;
	char	offset;

	if (op_code == 0xE8)
		offset = (char)read_pc_byte_tick(emu);
	else
		data = g_getw_fptr[op_code](emu->cpu);
	old_data = hl_of(emu->cpu);
	if (op_code == 0xE8)
		old_data = sp_of(emu->cpu);
	if (op_code == 0xE8)
		// data = sp_of(emu->cpu) + offset;
		set_sp(&emu->cpu, sp_of(emu->cpu) + offset);
	else
		// data = hl_of(emu->cpu) + data;
		g_setw_fptr[op_code](&emu->cpu, hl_of(emu->cpu) + data);
	// g_setw_fptr[op_code](&emu->cpu, data);
	set_flag_n(&emu->cpu, 0);
	if (hl_of(emu->cpu) < old_data || hl_of(emu->cpu) < data)
		set_flag_c(&emu->cpu, 1);
	if (((data & 0xFFF) + (old_data & 0xFFF)) & 0xF000)
		set_flag_h(&emu->cpu, 1);
	else
		set_flag_h(&emu->cpu, 0);
	if (op_code == 0xE8)
		set_flag_0xE8_0xF8(&emu->cpu, old_data, offset);
}

void	daa(t_emu *emu, t_byte op_code)
{
	t_word	a;

	(void)op_code;
	a = a_of(emu->cpu);
	if (!get_flag_n(emu->cpu))
	{
		if ((a & 0xF) > 9 || get_flag_h(emu->cpu))
			a += 0x6;// + ((a & 0xF) / 0xA) * 0x10;
		if (a > 0x9F || get_flag_c(emu->cpu))
		{
			a += 0x60;
			set_flag_c(&emu->cpu, 1);
		}
	}
	else
	{
		if (get_flag_h(emu->cpu))
			a -= 0x6;
		if (get_flag_c(emu->cpu))
			a -= 0x60;
	}
	set_a(&emu->cpu, a);
	set_flag_z(&emu->cpu, !a_of(emu->cpu));
	set_flag_h(&emu->cpu, 0);
}

void	cpl(t_emu *emu, t_byte op_code)
{
	(void)op_code;
	emu->cpu.a = ~emu->cpu.a;
	set_flag_n(&emu->cpu, 1);
	set_flag_h(&emu->cpu, 1);
}

void	scf(t_emu *emu, t_byte op_code)
{
	(void)op_code;
	set_flag_n(&emu->cpu, 0);
	set_flag_h(&emu->cpu, 0);
	set_flag_c(&emu->cpu, 1);
}

void	ccf(t_emu *emu, t_byte op_code)
{
	(void)op_code;
	set_flag_n(&emu->cpu, 0);
	set_flag_h(&emu->cpu, 0);
	set_flag_c(&emu->cpu, ~get_flag_c(emu->cpu) & 0x1);
}

t_byte	rlc(t_emu *emu, t_byte data)
{
	data = data >> 7 | data << 1;
	set_flag_c(&emu->cpu, data & 0x1);
	return (data);
}

t_byte	rrc(t_emu *emu, t_byte data)
{
	data = data >> 1 | data << 7;
	set_flag_c(&emu->cpu, data & 0x80);
	return (data);
}

t_byte	rl(t_emu *emu, t_byte data)
{
	t_byte	left_most;

	left_most = data & 0x80;
	data <<= 1;
	if (get_flag_c(emu->cpu))
		data |= 0x1;
	set_flag_c(&emu->cpu, left_most);
	return (data);
}

t_byte	rr(t_emu *emu, t_byte data)
{
	t_byte	right_most;

	right_most = data & 0x1;
	data >>= 1;
	if (get_flag_c(emu->cpu))
		data |= 0x80;
	set_flag_c(&emu->cpu, right_most);
	return (data);
}

t_byte	sla(t_emu *emu, t_byte data)
{
	set_flag_c(&emu->cpu, data & 0x8);
	return (data << 1);
}

t_byte	sra(t_emu *emu, t_byte data)
{
	set_flag_c(&emu->cpu, data & 0x1);
	return ((data & 0x80) | (data >> 1));
}

t_byte	swap(t_emu *emu, t_byte data)
{
	(void)emu;
	return (data >> 8 | data << 8);
}

t_byte	srl(t_emu *emu, t_byte data)
{
	set_flag_c(&emu->cpu, data & 0x1);
	return (data >> 1);
}

t_byte	bit_res_set(t_byte op_code, t_byte data)
{
	t_byte	pos;

	pos = ((op_code - 0x40) % 0x40) / 8;
	if (op_code <= 0x7F)
		data &= 1 << pos;
	else if (op_code <= 0xBF)
		data &= ~(1 << pos);
	else
		data |= 1 << pos;
	return (data);
}

t_getw	*g_rotate_getw_fptr[8] = {
	b_of, c_of, d_of, e_of, h_of, l_of, hl_of, a_of
};

t_setw	*g_rotate_setw_fptr[8] = {
	set_b, set_c, set_d, set_e, set_h, set_l, NULL, set_a
};
t_byte	(*g_rotate_fptr[8])(t_emu *, t_byte) = {
rlc, rrc, rl, rr, sla, sra, swap, srl
};

void	prefix_cb(t_emu *emu, t_byte op_code)
{
	t_byte	idx;
	t_byte	data;

	if (op_code == 0xCB)
		op_code = read_pc_byte_tick(emu);
	idx = op_code % 8;
	if (op_code >= 0x40)
		idx = ((op_code - 0x40) / 0x40) / 8;
	data = g_rotate_getw_fptr[idx](emu->cpu);
	if ((op_code & 0xF) == 0x6 || (op_code & 0xF) == 0xE)
	{
		data = bus_read(emu, data);
		emu_tick(emu, 4);
	}
	if (op_code >= 0x40)
		data = bit_res_set(op_code, data);
	else
		data = g_rotate_fptr[op_code / 8](emu, data);
	if (op_code <= 0x7F)
	{
		set_flag_z(&emu->cpu, !data);
		set_flag_n(&emu->cpu, 0);
	}
	if (op_code < 0x40)
		set_flag_h(&emu->cpu, 0);
	if (op_code >= 0x40 && op_code <= 0x7F)
		set_flag_h(&emu->cpu, 1);
	if ((op_code & 0xF) == 0x6 || (op_code & 0xF) == 0xE)
	{
		bus_write(emu, hl_of(emu->cpu), data);
		emu_tick(emu, 4);
	}
	else
		g_rotate_setw_fptr[op_code % 8](&emu->cpu, data);
}

void	rotate_a(t_emu *emu, t_byte op_code)
{
	prefix_cb(emu, op_code);
	set_flag_z(&emu->cpu, 0);
}

t_inst	*g_op_map[256] = {
	nop, ld_16, ld_r_r, inc_rr, inc_r, dec_r, ld_r_r, rotate_a,
	ld_16, add_16, ld_r_r, dec_rr, inc_r, dec_r, ld_r_r, rotate_a,
	stop, ld_16, ld_r_r, inc_rr, inc_r, dec_r, ld_r_r, rotate_a,
	jp, add_16, ld_r_r, dec_rr, inc_r, dec_r, ld_r_r, rotate_a,
	jp, ld_16, ld_r_r, inc_rr, inc_r, dec_r, ld_r_r, daa,
	jp, add_16, ld_r_r, dec_rr, inc_r, dec_r, ld_r_r, cpl,
	jp, ld_16, ld_r_r, inc_rr, inc_r, dec_r, ld_r_r, scf,
	jp, add_16, ld_r_r, dec_rr, inc_r, dec_r, ld_r_r, ccf,
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
	ret_reti, pop, jp, jp, call, push, bit_op, rst,
	ret_reti, ret_reti, jp, prefix_cb, call, call, bit_op, rst,
	ret_reti, pop, jp, NULL, call, push, bit_op, rst,
	ret_reti, ret_reti, jp, NULL, call, NULL, bit_op, rst,
	ld_m, pop, ld_m, NULL, NULL, push, bit_op, rst,
	add_16, jp, ld_m, NULL, NULL, NULL, bit_op, rst,
	ld_m, pop, ld_m, di, NULL, push, bit_op, rst,
	ld_16, ld_16, ld_m, ei, NULL, NULL, bit_op, rst
};
