/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 13:14:12 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/16 13:24:26 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

int	run_app(t_app *app)
{
	SDL_Event	event;

	if (!app)
		return (fprintf(stderr, "Empty pointer\n"), NOT_OK);
	while (OK)
	{
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
