/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cartridge.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:19:30 by mayeung           #+#    #+#             */
/*   Updated: 2024/10/16 17:10:42 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	print_byte_arr(unsigned char *arr, size_t n, int in_char, int with_nl)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (!in_char || !i)
			printf(" ");
		if (in_char)
			printf("%c", arr[i]);
		else
			printf("%02x", arr[i]);
		i++;
	}
	if (with_nl)
		printf("\n");
}

void	print_cart_header(t_cart cart)
{
	printf("File size: %d\n", cart.file_size);
	printf("Entry point:");
	print_byte_arr((void *)&cart.header.entry_point, 4, FALSE, TRUE);
	printf("Logo:");
	print_byte_arr((void *)&cart.header.logo, 48, FALSE, TRUE);
	printf("Title:");
	print_byte_arr((void *)&cart.header.title, 16, TRUE, TRUE);
	printf("Mcode:");
	print_byte_arr((void *)&cart.header.mcode, 4, FALSE, TRUE);
	printf("CGB flag:");
	print_byte_arr((void *)&cart.header.cgb_flag, 1, FALSE, TRUE);
	printf("SGB flag:");
	print_byte_arr((void *)&cart.header.sgb_flag, 1, FALSE, TRUE);
	printf("Cart type:");
	print_byte_arr((void *)&cart.header.cart_type, 1, FALSE, TRUE);
	printf("Rom size:");
	print_byte_arr((void *)&cart.header.rom_size, 1, FALSE, TRUE);
	printf("Ram size:");
	print_byte_arr((void *)&cart.header.ram_size, 1, FALSE, TRUE);
	printf("Dest code:");
	print_byte_arr((void *)&cart.header.dest_code, 1, FALSE, TRUE);
	printf("Old licensee code:");
	print_byte_arr((void *)&cart.header.old_licensee_code, 1, FALSE, TRUE);
	if (cart.header.old_licensee_code == 0x33)
		printf("New licensee code:");
	if (cart.header.old_licensee_code == 0x33)
		print_byte_arr((void *)&cart.header.new_licensee_code, 2, TRUE, TRUE);
}

int	read_cartridge_header(t_cart *cart)
{
	if (!cart)
		return (fprintf(stderr, "Empty pointer\n"), NOT_OK);
	if (cart->file_size < 0x150)
		return (fprintf(stderr, "File size too small..\n"), NOT_OK);
	memcpy(&cart->header, cart->data + 0x100, 0x50);
	memcpy(&cart->header.mcode, cart->data + 0x13F, 4);
	memcpy(&cart->header.cgb_flag, cart->data + 0x143, 1);
	return (OK);
}

int	read_cartridge(char *path, t_cart *cart)
{
	int				fd;
	int				read_size;
	unsigned char	buffer[BUFFER_SIZE];
	unsigned char	*new_data;

	if (!path || !cart)
		return (fprintf(stderr, "Empty pointer\n"), NOT_OK);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (fprintf(stderr, "Can't open file\n"), NOT_OK);
	memset(cart, 0, sizeof(t_cart));
	read_size = 1;
	while (read_size > 0)
	{
		read_size = read(fd, buffer, BUFFER_SIZE);
		if (read_size > 0)
		{
			new_data = malloc(sizeof(unsigned char) * (cart->file_size + read_size));
			if (!new_data)
				return (fprintf(stderr, "Allocate mem fail\n"), free(cart->data), close(fd), NOT_OK);
			memcpy(new_data, cart->data, cart->file_size);
			memcpy(new_data + cart->file_size, buffer, read_size);
			free(cart->data);
			cart->data = new_data;
			cart->file_size += read_size;
		}
	}
	close(fd);
	return (OK);
}
