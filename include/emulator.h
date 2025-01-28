/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emulator.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:20:23 by mayeung           #+#    #+#             */
/*   Updated: 2025/01/28 12:15:29 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EMULATOR_H
# define EMULATOR_H
# ifndef PRINT_CPU_LOG
#  define PRINT_CPU_LOG 0
# endif
# define BUFFER_SIZE 10000
# define FREQUENCY 4194304
// # define FPS 59.72750056960583
# define FPS 100.0
# define WINDOW_NAME "Old Boy"
# define RES_SCALE 4
# define SCR_H 144
# define SCR_W 160
# define WINDOW_H SCR_H * RES_SCALE
# define WINDOW_W SCR_W * RES_SCALE
# define NUM_TILE_PER_ROW 16
# define NUM_ROW_TILE_DATA 24
# define SCREEN_NUM_TILE_PER_ROW 32
# define SCREEN_NUM_TILE_ROW 18
# define TRUE 1
# define FALSE 0
# define OK 1
# define NOT_OK 0
# define BLACK 0x081820
# define DARK_GREEN 0x346856
# define LIGHT_GREEN 0x88C070
# define WHITE 0xE0F8D0
# define BG_WIN_TILE 0
# define OBJ0_TILE 1
# define OBJ1_TILE 2
# define M_PI 3.1415926535
# define SAMPLING_RATE 44100
# include <SDL3/SDL.h>
# include <SDL3/SDL_audio.h>
# include <SDL3/SDL_keycode.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <stdbool.h>
# include <math.h>
# include <time.h>
# include <sys/time.h>

typedef unsigned char		t_byte;
typedef unsigned short		t_word;
typedef unsigned int		t_ui;
typedef unsigned long long	t_ull;
typedef SDL_AudioDeviceID	t_sdlaod;

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

typedef struct s_rtc
{
	t_byte	s;
	t_byte	m;
	t_byte	h;
	t_byte	dl;
	t_byte	dh;
	t_byte	last_latch_write;
	t_byte	is_latched;
	t_ull	cycle;
}	t_rtc;

typedef struct s_cart
{
	t_byte			*data;
	int				file_size;
	t_byte			ram[0x20000];
	t_byte			*rom_bank0_ptr;
	t_byte			*rom_bankx_ptr;
	t_byte			*ram_bank_ptr;
	t_byte			ram_timer_enbaled;
	t_byte			rom_bank_id;
	t_byte			rom_bank_id2;
	t_byte			ram_bank_id;
	t_byte			banking_mode;
	char			*cart_file_name;
	t_rtc			rtc;
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
	t_word	p_div;
	t_byte	tima;
	t_byte	tma;
	t_byte	tac;
	t_byte	interrupt_countdown;
}	t_timer;

typedef struct s_serial
{
	t_byte	serial_out_buf[0xFFFF];
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

typedef struct s_tile_pix_info
{
	t_word	ti;
	t_word	tj;
	t_word	pi;
	t_word	pj;
}	t_tile_pix_info;

typedef struct s_ppu
{
	t_byte	oam[0xA0];
	t_byte	object_queue[40];
	t_byte	num_obj_scanline;
	t_word	dma_write_counter;
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
	t_byte	win_xl;
	t_byte	win_yl;
	t_byte	dma_cancelled;
	t_mode	ppu_mode;
}	t_ppu;

typedef struct s_apu
{
	t_byte	nr10_c1_sweep;
	t_byte	nr11_c1_timer_duty;
	t_byte	nr12_c1_vol_env;
	t_byte	nr13_c1_per_low;
	t_byte	nr14_c1_per_high_ctrl;
	t_byte	nr21_c2_timer_duty;
	t_byte	nr22_c2_vol_env;
	t_byte	nr23_c2_per_low;
	t_byte	nr24_c2_per_high_ctrl;
	t_byte	nr30_c3_dac;
	t_byte	nr31_c3_timer;
	t_byte	nr32_c3_out_lvl;
	t_byte	nr33_c3_per_low;
	t_byte	nr34_c3_per_high_ctrl;
	t_byte	nr41_c4_timer;
	t_byte	nr42_c4_vol_env;
	t_byte	nr43_c4_freq_ran;
	t_byte	nr44_c4_ctrl;
	t_byte	nr50_mas_vol_vin_pan;
	t_byte	nr51_sound_pan;
	t_byte	nr52_mas_ctrl;
	t_byte	wave_ram[0x10];
	t_byte	apu_div;
	t_word	ch1_period_counter;
	t_byte	ch1_sample_idx;
	t_byte	ch1_vol;
	t_byte	ch1_vol_sweep_counter;
	t_byte	ch1_freq_sweep_pace;
	t_byte	ch1_freq_sweep_enabled;
	// t_byte	ch1_freq_sweep_dir;
	t_byte	ch1_length_timer;
	float	ch1_value;
	t_word	ch2_period_counter;
	t_byte	ch2_sample_idx;
	t_byte	ch2_vol;
	t_byte	ch2_vol_sweep_counter;
	t_byte	ch2_length_timer;
	float	ch2_value;
}	t_apu;

typedef struct s_joypad
{
	t_byte	up;
	t_byte	down;
	t_byte	left;
	t_byte	right;
	t_byte	a;
	t_byte	b;
	t_byte	select;
	t_byte	start;
	t_byte	joyp;
}	t_joypad;

typedef struct s_emu
{
	t_cart			cart;
	t_byte			paused;
	t_byte			quit;
	t_cpu			cpu;
	t_byte			vram[0x2000];
	t_byte			wram[0x2000];
	t_byte			hram[0x7F];
	t_ull			clock_cycle;
	double			clock_scale;
	double			last_render_time;
	t_byte			interrupt_flag;
	t_byte			interrupt_enable;
	t_joypad		joypad;
	t_timer			timer;
	t_serial		serial;
	t_ppu			ppu;
	t_apu			apu;
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_AudioStream	*audio_stream;
	t_sdlaod		audio_id;
	struct timeval	last_tick;
	struct timeval	genesis_tick;
	t_ull			sec_elapsed;
	t_byte			print_log;
}	t_emu;

typedef struct s_app
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	t_emu			emu;
}	t_app;

typedef t_byte		(t_inst)(t_emu *, t_byte);
typedef	void		(t_setw)(t_cpu *, t_word);
typedef	t_word	(t_getw)(t_cpu);
extern t_inst	*g_op_map[256];

//cartridge
void	print_cart_header(t_cart cart);
int		read_cartridge_header(t_cart *cart);
int		read_cartridge(char *path, t_cart *cart);
void	cart_write(t_emu *emu, t_word addr, t_byte data);
t_byte	cart_read(t_emu *emu, t_word addr);
void	init_cart(t_emu *emu);
void	rtc_tick(t_emu *emu);
void	load_ram_sav(t_emu *emu);
void	save_ram_save(t_emu *emu);
//app
int		init_sdl(t_app *app);
int		run_app(t_app *app);
void	print_vram_tile(t_app *app);
double	calculate_time_diff(struct timeval old_time);
//emu
int		init_emu(t_emu *emu);
int		update_frame(t_emu *emu, double delta_time);
void	emu_tick(t_emu *emu, t_ull clock_cycle);
void	get_data_callback(void *userdata, SDL_AudioStream *stream,
			int additional_amount, int total_amount);
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
t_word	empty_of(t_cpu cpu);
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
t_byte	di(t_emu *emu, t_byte op_code);
t_byte	ei(t_emu *emu, t_byte op_code);
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
int		is_ppu_enabled(t_emu *emu);
void	ppu_tick(t_emu *emu);
void	ppu_write(t_emu *emu, t_word addr, t_byte data);
t_byte	ppu_read(t_emu *emu, t_word addr);
void	print_pixel(SDL_Surface *s,
			unsigned int colour, t_tile_pix_info t);
t_ui	get_colour_from_palette(t_emu *emu,
			t_byte cid, t_byte tile_type);
void	print_oam(t_emu *emu);
//input
void	handle_input_down(t_emu *emu, SDL_Event event);
void	handle_input_up(t_emu *emu, SDL_Event event);
void	input_write(t_emu *emu, t_byte data);
void	input_tick(t_emu *emu);
t_byte	input_read(t_emu *emu);
void	init_input(t_emu *emu);
//apu
void	init_apu(t_emu *emu);
void	apu_write(t_emu *emu, t_word addr, t_byte data);
t_byte	apu_read(t_emu *emu, t_word addr);
void	apu_tick(t_emu *emu);
void	apu_enable_ch1(t_emu *emu);
void	apu_enable_ch2(t_emu *emu);
void	apu_enable_ch3(t_emu *emu);
void	apu_enable_ch4(t_emu *emu);
t_word	apu_get_ch1_period(t_emu *emu);
t_word	apu_get_ch2_period(t_emu *emu);
void	apu_disable_ch1(t_emu *emu);
void	apu_disable_ch2(t_emu *emu);
void	apu_disable_ch3(t_emu *emu);
void	apu_disable_ch4(t_emu *emu);
#endif