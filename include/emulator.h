/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emulator.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:20:23 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/10 22:29:36 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define BUFFER_SIZE 10000
#define FREQUENCY 4194304
#define MS_PER_SECOND 1000000
#define CLOCK_SCALE 20
#define FPS 59.72750056960583
#define WINDOW_NAME "Old Boy"
#define WINDOW_H 768
#define WINDOW_W 512
#define NUM_TILE_PER_ROW 16
#define NUM_ROW_TILE_DATA 24
#define TRUE 1
#define FALSE 0
#define OK 1
#define NOT_OK 0
#define BLACK 0x081820
#define DARK_GREEN 0x346856
#define LIGHT_GREEN 0x88C070
#define WHITE 0xE0F8D0
#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/time.h>

typedef unsigned char		t_byte;
typedef unsigned short		t_word;
typedef unsigned long long	t_ull;

typedef struct s_cart_header
{
	t_byte	entry_point[4];
	t_byte	logo[48];
	char	title[16];
	t_byte	new_licensee_code[2];
	t_byte	sgb_flag;
	t_byte	cart_type;
	t_byte	rom_size;
	t_byte	ram_size;
	t_byte	dest_code;
	t_byte	old_licensee_code;
	t_byte	rom_ver;
	t_byte	header_checksum;
	t_byte	global_checksum[2];
	t_byte	mcode[4];
	t_byte	cgb_flag;
}	t_cart_header;

typedef struct s_cart
{
	t_byte			*data;
	int				file_size;
	t_cart_header	header;
}	t_cart;

typedef struct s_cpu
{
	t_byte	a;
	t_byte	f;
	t_byte	b;
	t_byte	c;
	t_byte	d;
	t_byte	e;
	t_byte	h;
	t_byte	l;
	t_word	sp;
	t_word	pc;
	int		halted;
	int		ime;
	t_byte	ime_countdown;
}	t_cpu;

typedef struct s_timer
{
	t_word	div;
	t_byte	tima;
	t_byte	tma;
	t_byte	tac;
}	t_timer;

typedef struct s_serial
{
	t_byte	serial_out_buf[1000];
	t_word	idx_out_buf;
	t_byte	sb;
	t_byte	sc;
	t_byte	transferring;
	t_byte	io_byte;
	t_byte	io_byte_bit_idx;
}	t_serial;

typedef enum e_mode
{
	HBLANK,
	VBLANK,
	OAM_SCAN,
	DRAWING
}	t_mode;

typedef struct s_ppu
{
	t_byte	oam[0x100];
	t_byte	dma;
	t_byte	lcdc;
	t_byte	ly;
	t_word	lx;
	t_byte	lyc;
	t_byte	stat;
	t_byte	scy;
	t_byte	scx;
	t_byte	wy;
	t_byte	wx;
	t_byte	bgp;
	t_byte	obp0;
	t_byte	obp1;
	t_mode	ppu_mode;
}	t_ppu;

typedef struct s_emu
{
	t_cart			cart;
	int				paused;
	t_cpu			cpu;
	t_byte			vram[0x2000];
	t_byte			wram[0x2000];
	t_byte			hram[0x7F];
	t_ull			clock_cycle;
	double			clock_scale;
	double			last_render_time;
	t_byte			interrupt_flag;
	t_byte			interrupt_enable;
	t_timer			timer;
	t_serial		serial;
	t_ppu			ppu;
	struct timeval	last_tick;
}	t_emu;

typedef struct s_app
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	t_emu			emu;
}	t_app;

typedef union u_bw
{
	t_byte	b;
	t_word	w;
}	t_ubw;

typedef void(t_inst)(t_emu *, t_byte);
typedef void(t_setw)(t_cpu *, t_word);
typedef void(t_setb)(t_cpu *, t_byte);
typedef void(t_setwb)(t_cpu *, t_ubw);
typedef t_word(t_getw)(t_cpu);
typedef t_byte(t_getb)(t_cpu);

extern	t_inst *g_op_map[256];

//cartridge
void	print_cart_header(t_cart cart);
int		read_cartridge_header(t_cart *cart);
int		read_cartridge(char *path, t_cart *cart);
//app
int		init_sdl(t_app *app);
int		run_app(t_app *app);
void	print_vram_tile(t_app *app);
//emu
int		init_emu(t_emu *emu);
int		update_frame(t_emu *emu, double delta_time);
void	emu_tick(t_emu *emu, t_ull clock_cycle);
//cpu
t_word	af_of(t_cpu cpu);
t_word	bc_of(t_cpu cpu);
t_word	de_of(t_cpu cpu);
t_word	hl_of(t_cpu cpu);
t_word	sp_of(t_cpu cpu);
t_word	pc_of(t_cpu cpu);
t_word	a_of(t_cpu cpu);
t_word	b_of(t_cpu cpu);
t_word	c_of(t_cpu cpu);
t_word	d_of(t_cpu cpu);
t_word	e_of(t_cpu cpu);
t_word	f_of(t_cpu cpu);
t_word	h_of(t_cpu cpu);
t_word	l_of(t_cpu cpu);
int		get_flag_z(t_cpu cpu);
int		get_flag_nz(t_cpu cpu);
int		get_flag_n(t_cpu cpu);
int		get_flag_h(t_cpu cpu);
int		get_flag_c(t_cpu cpu);
int		get_flag_nc(t_cpu cpu);
int		get_flag_true(t_cpu cpu);
void	set_af(t_cpu *cpu, t_word af);
void	set_bc(t_cpu *cpu, t_word bc);
void	set_de(t_cpu *cpu, t_word de);
void	set_hl(t_cpu *cpu, t_word hl);
void	set_sp(t_cpu *cpu, t_word sp);
void	set_pc(t_cpu *cpu, t_word pc);
void	set_a(t_cpu *cpu, t_word a);
void	set_b(t_cpu *cpu, t_word b);
void	set_c(t_cpu *cpu, t_word c);
void	set_d(t_cpu *cpu, t_word d);
void	set_e(t_cpu *cpu, t_word e);
void	set_f(t_cpu *cpu, t_word f);
void	set_h(t_cpu *cpu, t_word h);
void	set_l(t_cpu *cpu, t_word l);
void	set_id(t_cpu *cpu, t_word id);
void	set_flag_z(t_cpu *cpu, t_word value);
void	set_flag_n(t_cpu *cpu, t_word value);
void	set_flag_h(t_cpu *cpu, t_word value);
void	set_flag_c(t_cpu *cpu, t_word value);
void	init_cpu(t_cpu *cpu);
void	print_cpu_register(t_emu *emu);
int		cpu_step(t_emu *emu);
//instruction
void	di(t_emu *emu, t_byte op_code);
void	ei(t_emu *emu, t_byte op_code);
void	push_word(t_emu *emu, t_word data);
//bus
t_byte	bus_read(t_emu *emu, t_word addr);
void	bus_write(t_emu *emu, t_word addr, t_byte data);
//interrupt
void	process_interrupt(t_emu *emu);
//timer
t_byte	timer_read(t_emu *emu, t_word addr);
void	timer_write(t_emu *emu, t_word addr, t_byte data);
void	timer_tick(t_emu *emu);
void	init_timer(t_emu *emu);
//serial
void	init_serial(t_emu *emu);
void	serial_write(t_emu *emu, t_word addr, t_byte data);
t_byte	serial_read(t_emu *emu, t_word addr);
void	serial_tick(t_emu *emu);
//log serial bus output
void	write_serial_buf_file(t_emu *emu);
//ppu
void	init_ppu(t_emu *emu);
void	ppu_tick(t_emu *emu);
void	ppu_write(t_emu *emu, t_word addr, t_byte data);
t_byte	ppu_read(t_emu *emu, t_word addr);