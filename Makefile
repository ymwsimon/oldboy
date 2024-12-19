NAME = emu

SRC = main cartridge app cpu bus emu instruction interrupt timer serial ppu input apu

SRC_DIR = src

OBJ = $(SRC:%=$(OBJ_DIR)/%.o)

OBJ_DIR = obj

INC_DIR = include

FLAGS = -Wall -Werror -Wextra -g

LIB = -lSDL3 -lm

INCLUDE_PATH = -I/home/$(USER)/SDL/include

LD_PATH = -L/home/$(USER)/SDL/build

HEADER = include/emulator.h

CC = cc

all : $(NAME)

$(NAME) : $(OBJ) $(HEADER)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LD_PATH) $(LIB)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(HEADER)
	mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) $(INCLUDE_PATH) -c $< -o $@

rebuild_emu_obj_w_log :
	$(CC) $(FLAGS) $(INCLUDE_PATH) -c $(SRC_DIR)/emu.c -o $(OBJ_DIR)/emu.o -D PRINT_CPU_LOG=1

rebuild_emu_obj_wo_log :
	$(CC) $(FLAGS) $(INCLUDE_PATH) -c $(SRC_DIR)/emu.c -o $(OBJ_DIR)/emu.o -D PRINT_CPU_LOG=0

with_log : rebuild_emu_obj_w_log $(OBJ) $(HEADER)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LD_PATH) $(LIB)

wo_log : rebuild_emu_obj_wo_log $(OBJ) $(HEADER)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LD_PATH) $(LIB)

clean :
	rm -rf $(OBJ_DIR)/*

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
