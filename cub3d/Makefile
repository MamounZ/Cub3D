LIBFT_L = -L./libft -lft
MLX_L = -L./MLX42/build -lmlx42
LIBFT_PATH = ./libft
MLXPATH = ./MLX42
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
MLXFLAGS = -ldl -lglfw -pthread -lm
NAME = cub3D
SRCS = main.c list_helpers.c gnl_utils.c split_utils.c validations.c validate_f_c.c map_validation.c mlx_functions.c

OBJS = $(SRCS:.c=.o)

all: mlx42 $(NAME)

mlx42:
	cmake -B $(MLXPATH)/build $(MLXPATH)
	cmake --build $(MLXPATH)/build -j4

$(NAME): $(OBJS)
	make -C $(LIBFT_PATH)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_L) $(MLXFLAGS) $(LIBFT_L) -o $(NAME)

clean:
	make clean -C $(LIBFT_PATH)
	rm -rf $(OBJS)

fclean: clean
	make fclean -C $(LIBFT_PATH)
	rm -rf $(MLXPATH)/build
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re mlx42