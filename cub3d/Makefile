LIBFT_L = -L./libft -lft
LIBFT_PATH = ./libft
LIBFT_A = $(LIBFT_PATH)/libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
NAME = cub3D
SRCS = main.c list_helpers.c gnl_utils.c split_utils.c validations.c validate_f_c.c map_validation.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): LIBFT_A $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_L) -o $(NAME)

LIBFT_A:
	make bonus -C $(LIBFT_PATH)

clean:
	make clean -C $(LIBFT_PATH)
	rm -rf $(OBJS)

fclean: clean
	make fclean -C $(LIBFT_PATH)
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re LIBFT_A
