/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 13:14:12 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/24 14:48:58 by mayeung          ###   ########.fr       */
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
	if (time_diff > (1.0 / FPS) * MS_PER_SECOND * 0.05)// * CLOCK_SCALE)
	{
		// printf("time to tick cpu %lu - %f\n", curr_time.tv_sec, time_diff);
		cpu_step(emu);
		emu->last_tick = curr_time;
	}
	return (OK);
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
	SDL_DestroyWindow(app->window);
	SDL_DestroyRenderer(app->renderer);
	SDL_Quit();
	return (OK);
}
