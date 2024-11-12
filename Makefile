NAME = emu

SRC = main cartridge app cpu bus emu instruction interrupt timer serial ppu input

SRC_DIR = src

OBJ = $(SRC:%=$(OBJ_DIR)/%.o)

OBJ_DIR = obj

INC_DIR = include

FLAGS = -Wall -Werror -Wextra -g

LIB = -lSDL3

INCLUDE_PATH = -I/home/$(USER)/SDL/include

LD_PATH = -L/home/$(USER)/SDL/build

HEADER = include/emulator.h

CC = cc

all : $(NAME)

$(NAME) : $(OBJ) $(HEADER)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LD_PATH)  $(LIB)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(HEADER)
	mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) $(INCLUDE_PATH) -c $< -o $@

clean :
	rm -rf $(OBJ_DIR)/*

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
