NAME = emu

SRC = main cartridge init app cpu bus emu instruction

SRC_DIR = src

OBJ = $(SRC:%=$(OBJ_DIR)/%.o)

OBJ_DIR = obj

INC_DIR = include

FLAGS = -Wall -Werror -Wextra -g

LIB = -lSDL3

HEADER = include/emulator.h

CC = cc

all : $(NAME)

$(NAME) : $(OBJ) $(HEADER)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LIB)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(HEADER)
	mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

clean :
	rm -rf $(OBJ_DIR)/*

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re