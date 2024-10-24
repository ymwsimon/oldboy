/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 19:26:44 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/23 14:02:31 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

t_word	two_byte_of(t_cpu cpu, t_byte offset)
{
	t_byte	*res;

	res = (t_byte *)&cpu;
	res += offset;
	return (*res << 8 | *(res + 1));
}

t_word	af_of(t_cpu cpu)
{
	return (two_byte_of(cpu, 0));
}

t_word	bc_of(t_cpu cpu)
{
	return (two_byte_of(cpu, 2));
}

t_word	de_of(t_cpu cpu)
{
	return (two_byte_of(cpu, 4));
}

t_word	hl_of(t_cpu cpu)
{
	return (two_byte_of(cpu, 6));
}

t_word	sp_of(t_cpu cpu)
{
	return (cpu.sp);
}

t_word	pc_of(t_cpu cpu)
{
	return (cpu.pc);
}

t_word	a_of(t_cpu cpu)
{
	return (cpu.a);
}

t_word	b_of(t_cpu cpu)
{
	return (cpu.b);
}

t_word	c_of(t_cpu cpu)
{
	return (cpu.c);
}

t_word	d_of(t_cpu cpu)
{
	return (cpu.d);
}

t_word	e_of(t_cpu cpu)
{
	return (cpu.e);
}

t_word	f_of(t_cpu cpu)
{
	return (cpu.f);
}

t_word	h_of(t_cpu cpu)
{
	return (cpu.h);
}

t_word	l_of(t_cpu cpu)
{
	return (cpu.l);
}

void	set_two_byte(t_cpu *cpu, t_word two_byte, t_byte offset)
{
	t_byte	*byte_pointer;

	if (!cpu)
		return ;
	byte_pointer = (t_byte *)cpu;
	byte_pointer += offset;
	// printf("f:%x s:%x\n", *byte_pointer, *(byte_pointer + 1));
	// printf("tb:%x tf:%x ts:%x\n", two_byte, (two_byte >> 8) & 0xFF, two_byte & 0xFF);
	*byte_pointer = (two_byte >> 8) & 0xFF;
	if (offset)
		*(byte_pointer + 1) = two_byte & 0xFF;
	else
		*(byte_pointer + 1) = two_byte & 0xF0;
}

void	set_af(t_cpu *cpu, t_word af)
{
	set_two_byte(cpu, af, 0);
}

void	set_bc(t_cpu *cpu, t_word bc)
{
	set_two_byte(cpu, bc, 2);
}

void	set_de(t_cpu *cpu, t_word de)
{
	set_two_byte(cpu, de, 4);
}

void	set_hl(t_cpu *cpu, t_word hl)
{
	set_two_byte(cpu, hl, 6);
}

void	set_sp(t_cpu *cpu, t_word sp)
{
	if (cpu)
		cpu->sp = sp;
}

void	set_pc(t_cpu *cpu, t_word pc)
{
	if (cpu)
		cpu->pc = pc;
}

void	set_a(t_cpu *cpu, t_word a)
{
	if (cpu)
		cpu->a = a;
}

void	set_b(t_cpu *cpu, t_word b)
{
	if (cpu)
		cpu->b = b;
}
void	set_c(t_cpu *cpu, t_word c)
{
	if (cpu)
		cpu->c = c;
}

void	set_d(t_cpu *cpu, t_word d)
{
	if (cpu)
		cpu->d = d;
}

void	set_e(t_cpu *cpu, t_word e)
{
	if (cpu)
		cpu->e = e;
}

void	set_f(t_cpu *cpu, t_word f)
{
	if (cpu)
		cpu->f = f & 0xF0;
}

void	set_h(t_cpu *cpu, t_word h)
{
	if (cpu)
		cpu->h = h;
}

void	set_l(t_cpu *cpu, t_word l)
{
	if (cpu)
		cpu->l = l;
}

void	set_id(t_cpu *cpu, t_word id)
{
	(void)cpu;
	(void)id;
	return ;
}

int	get_flag(t_cpu cpu, t_byte offset)
{
	return (cpu.f & (1 << offset));
}

int	get_flag_z(t_cpu cpu)
{
	return (get_flag(cpu, 7));
}

int	get_flag_nz(t_cpu cpu)
{
	return (!get_flag(cpu, 7));
}

int	get_flag_n(t_cpu cpu)
{
	return (get_flag(cpu, 6));
}

int	get_flag_h(t_cpu cpu)
{
	return (get_flag(cpu, 5));
}

int	get_flag_c(t_cpu cpu)
{
	return (get_flag(cpu, 4));
}

int	get_flag_nc(t_cpu cpu)
{
	return (!get_flag(cpu, 4));
}

int	get_flag_true(t_cpu cpu)
{
	(void)cpu;
	return (TRUE);
}

void	set_flag(t_cpu *cpu, t_byte offset, int value)
{
	t_byte	v;

	if (!cpu)
		return ;
	v = 0;
	if (value)
		v = 1;
	cpu->f &= ~(1 << offset);
	cpu->f |= v << offset;
}

void	set_flag_z(t_cpu *cpu, int value)
{
	set_flag(cpu, 7, value);
}

void	set_flag_n(t_cpu *cpu, int value)
{
	set_flag(cpu, 6, value);
}

void	set_flag_h(t_cpu *cpu, int value)
{
	set_flag(cpu, 5, value);
}

void	set_flag_c(t_cpu *cpu, int value)
{
	set_flag(cpu, 4, value);
}

void	init_cpu(t_cpu *cpu)
{
	if (!cpu)
		return ;
	set_af(cpu, 0x01B0);
	set_bc(cpu, 0x0013);
	set_de(cpu, 0x00D8);
	set_hl(cpu, 0x014D);
	cpu->pc = 0x100;
	cpu->sp = 0xFFFE;
	cpu->halted = FALSE;
}

void	print_cpu_register(t_cpu *cpu)
{
	if (!cpu)
		return ;
	printf("af:%04X bc:%04X de:%04X hl:%04X pc:%04X sp:%04X halt?:%d\n",
		af_of(*cpu), bc_of(*cpu), de_of(*cpu),
		hl_of(*cpu), cpu->pc, cpu->sp, cpu->halted);
}

int	cpu_step(t_emu *emu)
{
	t_byte	op_code;
	t_inst	*instruction;

	if (!emu)
		return (0);
	if (!emu->cpu.halted)
	{
		op_code = bus_read(emu, emu->cpu.pc);
		instruction = g_op_map[op_code];
		if (instruction)
		{
			printf("Current op_code:%02X ", op_code);
			print_cpu_register(&emu->cpu);
			++(emu->cpu.pc);
			instruction(emu, op_code);
		}
		else
		{
			printf("unknown op code:%02X ", op_code);
			print_cpu_register(&emu->cpu);
			emu->cpu.halted = TRUE;
		}
	}
	else
		emu_tick(emu, 4);
	return (1);
}
