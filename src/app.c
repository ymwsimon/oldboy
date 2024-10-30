/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 13:14:12 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/30 19:14:30 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

int	tick(t_emu *emu)
{
	struct timeval	curr_time;
	double			time_diff;

	gettimeofday(&curr_time, NULL);
	time_diff = (curr_time.tv_sec - emu->last_tick.tv_sec) * MS_PER_SECOND;
	time_diff += (curr_time.tv_usec - emu->last_tick.tv_usec);
	// printf("curr_time_sec:%lu - last_tick_sec:%lu - curr_time_usec:%lu - last_tick_usec:%lu - ", curr_time.tv_sec, app->emu.last_tick.tv_sec, curr_time.tv_usec, app->emu.last_tick.tv_usec);
	// printf("time_diff:%llu\n", time_diff);
	if (time_diff > (1.0 / FPS))// * CLOCK_SCALE)
	{
		// printf("time to tick cpu %lu - %f\n", curr_time.tv_sec, time_diff);
		cpu_step(emu);
		emu->last_tick = curr_time;
	}
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

int	run_app(t_app *app)
{
	SDL_Event		event;

	init_emu(&app->emu);
	while (OK)
	{
		tick(&app->emu);
		if (SDL_PollEvent(&event))
		{
			SDL_RenderClear(app->renderer);
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
			SDL_RenderRect(app->renderer, NULL);
			SDL_RenderPresent(app->renderer);
		}
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
