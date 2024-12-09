/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cartridge.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:19:30 by mayeung           #+#    #+#             */
/*   Updated: 2024/12/07 20:11:31 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/emulator.h"

void	print_byte_arr(t_byte *arr, size_t n, int in_char, int with_nl)
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
		++i;
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
	if (cart->file_size < 0x150)
		return (fprintf(stderr, "File size too small..\n"), NOT_OK);
	memcpy(&cart->header, cart->data + 0x100, 0x50);
	memcpy(&cart->header.mcode, cart->data + 0x13F, 4);
	memcpy(&cart->header.cgb_flag, cart->data + 0x143, 1);
	return (OK);
}

int	read_cartridge(char *path, t_cart *cart)
{
	int		fd;
	int		read_size;
	t_byte	buffer[BUFFER_SIZE];
	t_byte	*new_data;

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
			new_data = malloc(sizeof(t_byte) * (cart->file_size + read_size));
			if (!new_data)
				return (fprintf(stderr, "Allocate memory fail\n"), free(cart->data), close(fd), NOT_OK);
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

uint32_t	n_rom_bank_bit_mask(t_emu *emu)
{
	uint32_t	mask;
	t_byte		n_rom_bank_bit;

	mask = 1;
	n_rom_bank_bit = emu->cart.header.rom_size;
	while (n_rom_bank_bit--)
		mask = (mask << 1) | 1;
	return (mask);
}

t_byte	n_ram_bank_bit_mask(t_emu *emu)
{
	t_byte			ram_bank_type;
	static t_byte	offsets[] = {0, 0, 1, 7, 31, 15};

	ram_bank_type = emu->cart.header.ram_size;
	if (ram_bank_type <= 5)
		return (offsets[ram_bank_type]);
	return (0);
}

void	update_cart_ptr_offset(t_emu *emu)
{

	emu->cart.rom_bank0_ptr = emu->cart.data;
	emu->cart.rom_bankx_ptr
		= emu->cart.data + 0x4000 * (n_rom_bank_bit_mask(emu)
			& ((emu->cart.ram_bank_id << 5) + emu->cart.rom_bank_id));
	if (!emu->cart.rom_bank_id)
		emu->cart.rom_bankx_ptr += 0x4000;
	emu->cart.ram_bank_ptr = NULL;
	if (n_ram_bank_bit_mask(emu))
		emu->cart.ram_bank_ptr = emu->cart.ram;
		// + 0x2000 * ((n_ram_bank_bit_mask(emu) / 2) & emu->cart.ram_bank_id);
	if (emu->cart.banking_mode == 1)
	{
		emu->cart.rom_bank0_ptr = emu->cart.data + 0x4000
			* (n_rom_bank_bit_mask(emu) & (emu->cart.ram_bank_id << 5));
		if (n_ram_bank_bit_mask(emu))
			emu->cart.ram_bank_ptr = emu->cart.data + 0x2000
				* ((n_ram_bank_bit_mask(emu) / 2) & emu->cart.ram_bank_id);
	}
}

uint32_t	get_offset_rom_mbc1(t_emu *emu, t_byte is_upper)
{
	uint32_t	offset;

	offset = 0;
	if (!is_upper && emu->cart.banking_mode == 1)
		offset = 0x4000
			* (n_rom_bank_bit_mask(emu) & (emu->cart.ram_bank_id << 5));
	if (is_upper)
	{
		offset = 0x4000 * (n_rom_bank_bit_mask(emu)
				& (((emu->cart.ram_bank_id & 3) << 5) + emu->cart.rom_bank_id));
		if (!emu->cart.rom_bank_id)
			offset += 0x4000;
	}
	return (offset);
}

uint32_t	get_offset_rom_upper(t_emu *emu)
{
	t_byte	cart_type;

	cart_type = emu->cart.header.cart_type;
	if (cart_type == 0 || cart_type == 8 || cart_type == 9)
		return (0x4000);
	if (cart_type >= 1 && cart_type <= 3)
		return (get_offset_rom_mbc1(emu, TRUE));
	return (0x4000);
}

uint32_t	get_offset_rom_lower(t_emu *emu)
{
	t_byte	cart_type;

	cart_type = emu->cart.header.cart_type;
	if (cart_type == 0 || cart_type == 8 || cart_type == 9)
		return (0);
	if (cart_type >= 1 && cart_type <= 3)
		return (get_offset_rom_mbc1(emu, FALSE));
	return (0);
}

uint32_t	get_offset_ram(t_emu *emu)
{
	(void)emu;
	return (0);
}

void	cart_write(t_emu *emu, t_word addr, t_byte data)
{
	// printf("writing cart- addr:%4X data:%4X\n", addr, data);
	if (addr <= 0x1FFF)
	{
		emu->cart.ram_enbaled = (data & 0xF) == 0xA;
		return ;
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF)
		emu->cart.rom_bank_id = data & 0x1F;
	else if (addr >= 0x4000 && addr <= 0x5FFF)
		emu->cart.ram_bank_id = data & 3;
	else if (addr >= 0x6000 && addr <= 0x7FFF)
		emu->cart.banking_mode = data & 1;
	update_cart_ptr_offset(emu);
}

t_byte	cart_read(t_emu *emu, t_word addr)
{
	if (addr <= 0x3FFF)
		return (emu->cart.rom_bank0_ptr[addr]);
	if (addr >= 0x4000 && addr <= 0x7FFF)
		return (emu->cart.rom_bankx_ptr[addr - 0x4000]);
	if (addr >= 0xA000 && addr <= 0xBFFF && emu->cart.ram_bank_ptr
		&& emu->cart.ram_enbaled)
		return (emu->cart.ram_bank_ptr[addr - 0xA000]);
	return (0xFF);
}
