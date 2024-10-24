/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:18:37 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/24 14:32:53 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

int	main(int arc, char **arv)
{
	t_app	app;

	if (arc != 2)
		return (fprintf(stdout, "Usage..\n"), 1);
	if (read_cartridge(arv[1], &app.emu.cart) == NOT_OK)
		return (fprintf(stderr, "Can't open cartridge\n"), 1);
	if (init_sdl(&app) == NOT_OK)
		return (1);
	read_cartridge_header(&app.emu.cart);
	// print_cart_header(app.emu.cart);
	run_app(&app);
	free(app.emu.cart.data);
	return (0);
}
