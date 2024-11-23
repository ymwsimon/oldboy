/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 13:14:12 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/23 20:29:23 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

double	calculate_time_diff(struct timeval old_time)
{
	struct timeval	curr_time;
	double			time_diff;

	gettimeofday(&curr_time, NULL);
	time_diff = (curr_time.tv_sec - old_time.tv_sec) * MS_PER_SECOND;
	time_diff += ((double)curr_time.tv_usec - old_time.tv_usec);
	time_diff /= MS_PER_SECOND;
	return (time_diff);
}

int	tick(t_app *app)
{
	// struct timeval	curr_time;
	double			time_diff;

	// gettimeofday(&curr_time, NULL);
	// time_diff = (curr_time.tv_sec - app->emu.last_tick.tv_sec) * MS_PER_SECOND;
	// time_diff += ((double)curr_time.tv_usec - app->emu.last_tick.tv_usec);
	// time_diff /= MS_PER_SECOND;
	time_diff = calculate_time_diff(app->emu.last_tick);
	// printf("curr_time_sec:%lu - last_tick_sec:%lu - curr_time_usec:%lu - last_tick_usec:%lu - ", curr_time.tv_sec, app->emu.last_tick.tv_sec, curr_time.tv_usec, app->emu.last_tick.tv_usec);
	// printf("time_diff:%f\n", time_diff);
	if (time_diff + app->emu.last_render_time > 1.0 / FPS)
	{
		// printf("time to tick cpu - %f - %f - %f\n", time_diff, app->emu.last_render_time, 1.0 / FPS);
		// app->emu.clock_cycle++;
		// cpu_step(&app->emu);
		update_frame(&app->emu, time_diff);
		// print_vram_tile(app);
		app->emu.last_render_time = calculate_time_diff(app->emu.last_tick) - time_diff;
		gettimeofday(&app->emu.last_tick, NULL);
		// app->emu.last_tick = curr_time;
	}
	// if (time_diff > (1.0 / FPS))
	// if (!(app->emu.clock_cycle % (FREQUENCY / FPS)))
		// print_vram_tile(app);
	return (OK);
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

void	print_pixel(t_emu *emu, SDL_Surface *s, unsigned int colour, t_tile_pix_info t)
{
	t_word			i;
	t_word			j;
	// unsigned int	colour;

	j = 0;
	(void)emu;
	// colour = get_colour_map(emu, cid);
	while (j < 4)
	{
		i = 0;
		while (i < 4)
		{
			SDL_WriteSurfacePixel(s,
				t.ti * 8 * 4 + t.pi * 4 + i,
				t.tj * 8 * 4 + t.pj * 4 + j,
				colour >> 16, (colour & 0xFF00) >> 8, colour & 0xFF, 255);
			++i;
		}
		++j;
	}
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
			print_pixel(&app->emu, s,
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

int	run_app(t_app *app)
{
	SDL_Event		event;

	init_emu(&app->emu);
	while (OK)
	{
		tick(app);
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_Q))
				break;
			else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_P)
				print_tile_map(&app->emu);
			else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_M)
				print_gbmicro_result(&app->emu);
			else if (event.type == SDL_EVENT_KEY_DOWN)
				handle_input_down(&app->emu, event);
			else if (event.type == SDL_EVENT_KEY_UP)
				handle_input_up(&app->emu, event);
		}
		usleep(5);
	}
	// if (app->emu.serial.idx_out_buf)
	// 	write_serial_buf_file(&app->emu);
	SDL_DestroyWindow(app->window);
	SDL_DestroyRenderer(app->renderer);
	SDL_Quit();
	return (OK);
}

int	init_sdl(t_app *app)
{
	if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		return (fprintf(stderr, "Can't init sdl\n"), NOT_OK);
	app->window = SDL_CreateWindow(WINDOW_NAME, WINDOW_W, WINDOW_H, 0);
	if (!app->window)
		return (fprintf(stderr, "Can't create window\n"), NOT_OK);
	app->renderer = SDL_CreateRenderer(app->window, NULL);
	if (!app->renderer)
		return (fprintf(stderr, "Can't create renderer\n"), NOT_OK);
	gettimeofday(&app->emu.last_tick, NULL);
	app->emu.window = app->window;
	return (OK);
}
