/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 19:26:44 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/10 14:07:41 by mayeung          ###   ########.fr       */
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

	byte_pointer = (t_byte *)cpu;
	byte_pointer += offset;
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
	cpu->sp = sp;
}

void	set_pc(t_cpu *cpu, t_word pc)
{
	cpu->pc = pc;
}

void	set_a(t_cpu *cpu, t_word a)
{
	cpu->a = a;
}

void	set_b(t_cpu *cpu, t_word b)
{
	cpu->b = b;
}

void	set_c(t_cpu *cpu, t_word c)
{
	cpu->c = c;
}

void	set_d(t_cpu *cpu, t_word d)
{
	cpu->d = d;
}

void	set_e(t_cpu *cpu, t_word e)
{
	cpu->e = e;
}

void	set_f(t_cpu *cpu, t_word f)
{
	cpu->f = f & 0xF0;
}

void	set_h(t_cpu *cpu, t_word h)
{
	cpu->h = h;
}

void	set_l(t_cpu *cpu, t_word l)
{
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
	return ((cpu.f & (1 << offset)) >> offset);
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

void	set_flag(t_cpu *cpu, t_byte offset, t_word value)
{
	t_byte	v;

	v = 0;
	if (value)
		v = 1;
	cpu->f &= ~(1 << offset);
	cpu->f |= v << offset;
}

void	set_flag_z(t_cpu *cpu, t_word value)
{
	set_flag(cpu, 7, value);
}

void	set_flag_n(t_cpu *cpu, t_word value)
{
	set_flag(cpu, 6, value);
}

void	set_flag_h(t_cpu *cpu, t_word value)
{
	set_flag(cpu, 5, value);
}

void	set_flag_c(t_cpu *cpu, t_word value)
{
	set_flag(cpu, 4, value);
}

void	init_cpu(t_cpu *cpu)
{
	set_af(cpu, 0x01B0);
	set_bc(cpu, 0x0013);
	set_de(cpu, 0x00D8);
	set_hl(cpu, 0x014D);
	cpu->pc = 0x100;
	cpu->sp = 0xFFFE;
	cpu->halted = FALSE;
	cpu->ime = FALSE;
	cpu->ime_countdown = 0;
}

static char	g_f_flag_str[5];

char	*f_flag_str(t_cpu cpu)
{
	strcpy(g_f_flag_str, "----");
	if (get_flag_z(cpu))
		g_f_flag_str[0] = 'Z';
	if (get_flag_n(cpu))
		g_f_flag_str[1] = 'N';
	if (get_flag_h(cpu))
		g_f_flag_str[2] = 'H';
	if (get_flag_c(cpu))
		g_f_flag_str[3] = 'C';
	return (g_f_flag_str);
}

void	print_cpu_register(t_emu *emu)
{
	printf("%02X %02X %02X ", bus_read(emu, emu->cpu.pc),
		bus_read(emu, emu->cpu.pc + 1),
		bus_read(emu, emu->cpu.pc + 2));
	printf("A: %02X F: %s BC: %04X DE: %04X HL: %04X PC: %04X SP: %04X\n",
		a_of(emu->cpu), f_flag_str(emu->cpu), bc_of(emu->cpu), de_of(emu->cpu),
		hl_of(emu->cpu), emu->cpu.pc, emu->cpu.sp);
}

int	cpu_step(t_emu *emu)
{
	t_byte	op_code;
	t_inst	*instruction;

	if (!emu->cpu.halted)
	{
		if (emu->cpu.ime && (emu->interrupt_enable & emu->interrupt_flag))
			process_interrupt(emu);
		else
		{
			op_code = bus_read(emu, emu->cpu.pc);
			instruction = g_op_map[op_code];
			if (instruction)
			{
				// print_cpu_register(emu);
				++(emu->cpu.pc);
				emu_tick(emu, 4);
				instruction(emu, op_code);
			}
			else
			{
				printf("unknown op code:%02X ", op_code);
				print_cpu_register(emu);
				emu->cpu.halted = TRUE;
			}
		}
	}
	else
	{
		emu_tick(emu, 4);
		if (emu->interrupt_enable & emu->interrupt_flag)
			emu->cpu.halted = FALSE;
	}
	if (emu->cpu.ime_countdown)
	{
		--(emu->cpu.ime_countdown);
		if (!emu->cpu.ime_countdown)
			emu->cpu.ime = TRUE;
	}
	return (1);
}
