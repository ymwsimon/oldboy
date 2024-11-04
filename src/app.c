/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 13:14:12 by mayeung           #+#    #+#             */
/*   Updated: 2024/11/04 13:22:49 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

int	tick(t_app *app)
{
	struct timeval	curr_time;
	double			time_diff;

	gettimeofday(&curr_time, NULL);
	time_diff = (curr_time.tv_sec - app->emu.last_tick.tv_sec) * MS_PER_SECOND;
	time_diff += (curr_time.tv_usec - app->emu.last_tick.tv_usec);
	// printf("curr_time_sec:%lu - last_tick_sec:%lu - curr_time_usec:%lu - last_tick_usec:%lu - ", curr_time.tv_sec, app->emu.last_tick.tv_sec, curr_time.tv_usec, app->emu.last_tick.tv_usec);
	// printf("time_diff:%llu\n", time_diff);
	// if (time_diff > (1.0 / FPS))// * CLOCK_SCALE)
	{
		// printf("time to tick cpu %lu - %f\n", curr_time.tv_sec, time_diff);
		app->emu.clock_cycle++;
		cpu_step(&app->emu);
		app->emu.last_tick = curr_time;
	}
	// if (time_diff > (1.0 / FPS))
	if (app->emu.clock_cycle > 400000)
		print_vram_tile(app);
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

void	print_pixel(t_app *app, t_byte *pixels, t_byte cid, t_word ti, t_word tj, t_word pi, t_word pj)
{
	SDL_Surface			*s;
	t_word				i;
	t_word				j;
	unsigned int		colour;
	static unsigned int	colour_map[4] = {WHITE, LIGHT_GREEN, DARK_GREEN, BLACK};

	(void)pixels;
	// SDL_UpdateTexture
	s = SDL_GetWindowSurface(app->window);
	j = 0;
	colour = colour_map[cid];
	while (j < 4)
	{
		i = 0;
		while (i < 4)
		{
			// pixels[tj * 20 * 8 * 4 + ti * 8 * 4 + pj * 8 * 4 + pi * 4 + j * 4 + i * 4] = 0xFF000000 | colour;
			SDL_WriteSurfacePixel(s,
				ti * 8 * 4 + pi * 4 + i,
				tj * 8 * 4 + pj * 4 + j,
				colour >> 16, (colour & 0xFF00) >> 8, colour & 0xFF, 255);
			++i;
		}
		++j;
	}
}

void	print_tile(t_app *app, t_byte *pixels, t_word ti, t_word tj)
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
			offset = tj * 16 * 20 + ti * 16 + pj * 2;
			cid = (app->emu.vram[offset]
					& (1 << (7 - pi))) >> (7 - pi);
			cid += ((app->emu.vram[tj * 16 * 20 + ti * 16 + pj * 2 + 1]
						& (1 << (7 - pi))) >> (7 - pi)) << 1;
			print_pixel(app, pixels, cid, ti, tj, pi, pj);
			++pi;
		}
		++pj;
	}
}

void	print_vram_tile(t_app *app)
{
	t_word		i;
	t_word		j;
	t_byte		*pixels;
	// SDL_Texture	*t;
	// SDL_Surface	*s;

	// s = SDL_CreateSurface(WINDOW_W, WINDOW_H, SDL_PIXELFORMAT_ARGB8888);
	// s = SDL_GetWindowSurface(app->window);
	// t = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_W, WINDOW_H);
	// pixels = malloc(sizeof(t_byte) * 384 * 64 * 4);
	// (void)t;
	pixels = NULL;
	(void)pixels;
	// SDL_LockSurface(s);
	j = 0;
	while (j < 19)
	{
		i = 0;
		while (i < 20)
		{
			print_tile(app, pixels, i, j);
			++i;
		}
		++j;
	}
	// SDL_UnlockSurface(s);
	// SDL_WriteSurfacePixel(s, 300, 300, 255, 0, 255, 255);
	// free(pixels);
	SDL_UpdateWindowSurface(app->window);

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
			else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_UP)
				SDL_SetRenderDrawColor(app->renderer, 80, 0, 60, 50);
			else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_DOWN)
				SDL_SetRenderDrawColor(app->renderer, 0, 80, 60, 50);
			else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_LEFT)
				SDL_SetRenderDrawColor(app->renderer, 80, 60, 0, 50);
			else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_RIGHT)
				SDL_SetRenderDrawColor(app->renderer, 80, 100, 60, 50);
			SDL_RenderPresent(app->renderer);
			// SDL_RenderClear(app->renderer);
		}
		// print_vram_tile(app);
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
	return (OK);
}
