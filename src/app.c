/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 13:14:12 by mayeung           #+#    #+#             */
/*   Updated: 2025/01/27 13:48:46 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

double	calculate_time_diff(struct timeval old_time)
{
	struct timeval	curr_time;
	double			time_diff;

	gettimeofday(&curr_time, NULL);
	time_diff = (curr_time.tv_sec - old_time.tv_sec) * SDL_US_PER_SECOND;
	time_diff += ((double)curr_time.tv_usec - old_time.tv_usec);
	time_diff /= SDL_US_PER_SECOND;
	return (time_diff);
}

void	write_serial_buf_file(t_emu *emu)
{
	int	fd;

	fd = open("out_buf", O_WRONLY | O_CREAT | O_APPEND, 0755);
	if (fd >= 0)
	{
		write(fd, emu->serial.serial_out_buf, emu->serial.idx_out_buf);
		close(fd);
	}
}

unsigned int	get_colour_map(t_emu *emu, t_byte cid)
{
	static unsigned int	colour_map[4] = {WHITE, LIGHT_GREEN, DARK_GREEN, BLACK};

	(void)emu;
	return (colour_map[cid]);
}

void	print_pixel(SDL_Surface *s, unsigned int colour, t_tile_pix_info t)
{
	SDL_Rect	rect;

	rect.h = RES_SCALE;
	rect.w = RES_SCALE;
	rect.x = t.ti * 8 * RES_SCALE + t.pi * RES_SCALE;
	rect.y = t.tj * 8 * RES_SCALE + t.pj * RES_SCALE;
	SDL_FillSurfaceRect(s, &rect, colour);
}

void	print_tile(t_app *app, SDL_Surface *s, t_word ti, t_word tj)
{
	t_word	pi;
	t_word	pj;
	t_word	offset;
	t_byte	cid;

	pj = 0;
	while (pj < 8)
	{
		pi = 0;
		while (pi < 8)
		{
			offset = tj * 16 * NUM_TILE_PER_ROW + ti * 16 + pj * 2;
			cid = (app->emu.vram[offset]
					& (1 << (7 - pi))) >> (7 - pi);
			cid += ((app->emu.vram[offset + 1]
						& (1 << (7 - pi))) >> (7 - pi)) << 1;
			print_pixel(s,
				get_colour_from_palette(&app->emu, cid, BG_WIN_TILE),
				(t_tile_pix_info){ti, tj, pi, pj});
			++pi;
		}
		++pj;
	}
}

void	print_vram_tile(t_app *app)
{
	t_word		i;
	t_word		j;
	SDL_Surface	*s;

	s = SDL_GetWindowSurface(app->window);
	SDL_LockSurface(s);
	j = 0;
	while (j < NUM_ROW_TILE_DATA)
	{
		i = 0;
		while (i < NUM_TILE_PER_ROW)
		{
			print_tile(app, s, i, j);
			++i;
		}
		++j;
	}
	SDL_UnlockSurface(s);
	SDL_UpdateWindowSurface(app->window);
}

void	print_tile_map(t_emu *emu)
{
	t_word	idx;

	idx = 0;
	printf("LCDC: %02X LY:%02X STAT:%02X SCY:%02X SCX:%02X WY:%02X WX:%02X \n",
		emu->ppu.lcdc, emu->ppu.ly, emu->ppu.stat,
		emu->ppu.scy, emu->ppu.scx, emu->ppu.wy, emu->ppu.wx);
	while (idx < 2048)
	{
		printf("%02X ", emu->vram[0x1800 + idx]);
		++idx;
		if (!(idx % 16))
			printf("\n");
	}
}

void	print_gbmicro_result(t_emu *emu)
{
	printf("Test result-FF80:%X Expected-FF81:%X Pass?FF82:%d\n",
		bus_read(emu, 0xFF80),
		bus_read(emu, 0xFF81),
		(char)bus_read(emu, 0xFF82));
}

void	print_mooneye_result(t_emu *emu)
{
	printf("Test result -- B:%d C:%d D:%d E:%d H:%d L:%d\n", emu->cpu.b,
		emu->cpu.c, emu->cpu.d, emu->cpu.e, emu->cpu.h, emu->cpu.l);
}

void	print_oam_content(t_emu *emu)
{
	t_byte	idx;

	idx = 0;
	while (idx < 160)
	{
		printf("%2X ", emu->ppu.oam[idx]);
		++idx;
		if (!(idx % 16))
			printf("\n");
	}
}

void	print_time_60fps(void)
{
	static struct timeval	start_time;
	static t_byte			fps_idx = 0;

	++fps_idx;
	if (fps_idx == 60)
	{
		fps_idx = 0;
		printf("%f\n", calculate_time_diff(start_time));
		gettimeofday(&start_time, NULL);
	}
}

void	print_debug_info(t_emu *emu, SDL_Keycode code)
{
	if (code == SDLK_P)
		print_tile_map(emu);
	else if (code == SDLK_M)
		print_gbmicro_result(emu);
	else if (code == SDLK_Y)
		print_mooneye_result(emu);
	else if (code == SDLK_O)
		print_oam_content(emu);
	else if (code == SDLK_L)
		emu->print_log = !emu->print_log;
}

t_byte	is_key_for_debug_info(SDL_Keycode code)
{
	if (code == SDLK_P || code == SDLK_M || code == SDLK_Y
		|| code == SDLK_O || code == SDLK_L)
		return (TRUE);
	return (FALSE);
}

t_byte	is_quit(SDL_Event event)
{
	return (event.type == SDL_EVENT_QUIT
		|| (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_Q));
}

void	app_loop(t_emu *emu)
{
	SDL_Event	event;
	double		time_diff;

	while (OK)
	{
		time_diff = calculate_time_diff(emu->last_tick);
		if (time_diff > 1.0 / FPS)
		{
			gettimeofday(&emu->last_tick, NULL);
			update_frame(emu, time_diff);
		}
		if (SDL_PollEvent(&event))
		{
			if (is_quit(event))
				break ;
			else if (event.type == SDL_EVENT_KEY_DOWN
				&& is_key_for_debug_info(event.key.key))
				print_debug_info(emu, event.key.key);
			else if (event.type == SDL_EVENT_KEY_DOWN)
				handle_input_down(emu, event);
			else if (event.type == SDL_EVENT_KEY_UP)
				handle_input_up(emu, event);
		}
		usleep(500);
	}
}

int	run_app(t_app *app)
{
	init_emu(&app->emu);
	load_ram_sav(&app->emu);
	gettimeofday(&app->emu.genesis_tick, NULL);
	gettimeofday(&app->emu.last_tick, NULL);
	app_loop(&app->emu);
	save_ram_save(&app->emu);
	// if (app->emu.serial.idx_out_buf)
	// 	write_serial_buf_file(&app->emu);
	SDL_DestroyWindow(app->window);
	SDL_DestroyRenderer(app->renderer);
	SDL_Quit();
	return (OK);
}

int	init_sdl(t_app *app)
{
	SDL_AudioSpec	src_spec;
	SDL_AudioSpec	dst_spec;

	dst_spec.channels = 2;
	dst_spec.format = SDL_AUDIO_F32;
	dst_spec.freq = SAMPLING_RATE;
	src_spec.channels = 2;
	src_spec.format = SDL_AUDIO_F32;
	src_spec.freq = 65536;
	if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		return (fprintf(stderr, "Can't init sdl\n"), NOT_OK);
	app->window = SDL_CreateWindow(WINDOW_NAME, WINDOW_W, WINDOW_H, 0);
	if (!app->window)
		return (fprintf(stderr, "Can't create window\n"), NOT_OK);
	app->renderer = SDL_CreateRenderer(app->window, NULL);
	if (!app->renderer)
		return (fprintf(stderr, "Can't create renderer\n"), NOT_OK);
	// gettimeofday(&app->emu.last_tick, NULL);
	app->emu.window = app->window;
	app->emu.renderer = app->renderer;
	app->emu.audio_id = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &dst_spec);
	if (!app->emu.audio_id)
		return (fprintf(stderr, "Can't open audio device\n"), NOT_OK);
	app->emu.audio_stream = SDL_CreateAudioStream(&src_spec, &dst_spec);
	// app->emu.audio_stream = SDL_OpenAudioDeviceStream
	// 	(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
	if (!app->emu.audio_stream)
	{
		printf("%s", SDL_GetError());
		return (fprintf(stderr, "Can't create audio stream\n"), NOT_OK);
	}
	if (!SDL_BindAudioStream(app->emu.audio_id, app->emu.audio_stream))
		return (fprintf(stderr, "Can't bind audio stream\n"), NOT_OK);
	// SDL_SetAudioStreamFrequencyRatio(app->emu.audio_stream, 23.77723356);
	SDL_ResumeAudioStreamDevice(app->emu.audio_stream);
	SDL_GetAudioStreamFormat(app->emu.audio_stream, &src_spec, &dst_spec);
	printf("src:%d %d %d %ld\n", src_spec.channels, src_spec.format, src_spec.freq, sizeof(float));
	printf("dst:%d %d %d %ld\n", dst_spec.channels, dst_spec.format, dst_spec.freq, sizeof(float));
	// printf("freq:%f\n", SDL_GetAudioStreamFrequencyRatio(app->emu.audio_stream));
	// SDL_SetAudioStreamFrequencyRatio(app->emu.audio_stream, 0.992);
	return (OK);
}
