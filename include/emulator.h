/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emulator.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:20:23 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/16 16:48:59 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define BUFFER_SIZE 10000
#define FREQUENCY 4194304
#define WINDOW_NAME "Old Boy"
#define WINDOW_H 500
#define WINDOW_W 500
#define TRUE 1
#define FALSE 0
#define OK 1
#define NOT_OK 0
#include <SDL3/SDL.h>
#include <SDL3/SDL_keycode.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct s_cart_header
{
	unsigned char	cart_type;
	unsigned char	rom_size;
	unsigned char	ram_size;
	unsigned char	dest_code;
	unsigned char	old_licensee_code;
	unsigned char	rom_ver;
	unsigned char	header_checksum;
	unsigned char	global_checksum[2];
	char			title[16];
	unsigned char	logo[48];
	unsigned char	mcode[4];
	unsigned char	new_licensee_code[2];
	unsigned char	cgb_flag;
	unsigned char	sgb_flag;
	int				ram_type;
	unsigned char	entry_point[4];
}	t_cart_header;

typedef struct s_cart
{
	unsigned char	*data;
	int				file_size;
	t_cart_header	header;
}	t_cart;

typedef struct s_emu
{
	t_cart	cart;
}	t_emu;

typedef struct s_app
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	t_emu			emu;
}	t_app;

void	print_cart_header(t_cart cart);
int		read_cartridge_header(t_cart *cart);
int		read_cartridge(char *path, t_cart *cart);
int		init_sdl(t_app *app);
int		run_app(t_app *app);
// void	app_loop(void);