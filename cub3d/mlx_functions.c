/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:10:56 by mazaid            #+#    #+#             */
/*   Updated: 2025/09/20 16:30:26 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

mlx_image_t *image;

void put_block(t_data *game_data, int x, int y, uint32_t color)
{
	int i;
	int j;

	i = 0;
	while (i < game_data->block_size)
	{
		j = 0;
		while (j < game_data->block_size)
		{
			if (j + x < 340 && i + y < 180)
				mlx_put_pixel(game_data->m_map, j + x, i + y, color);
			j++;
		}
		i++;
	}
}
void put_mini_map(t_data *game_data)
{
	int i;
	int j;

	if (game_data->map_rows < game_data->map_cols)
		game_data->block_size = 340 / game_data->map_cols;
	else
		game_data->block_size = 340 / game_data->map_rows;
	i = 0;
	while (i < game_data->map_rows)
	{
		j = 0;
		while (game_data->map[i][j])
		{
			if (game_data->map[i][j] == '1')
				put_block(game_data, j * game_data->block_size, i * game_data->block_size, 0x0F3F6FFF);
			else if (game_data->map[i][j] == '0')
				put_block(game_data, j * game_data->block_size, i * game_data->block_size, 0x669999FF);
			else if (j == game_data->player_pos.x && i == game_data->player_pos.y)
				put_block(game_data, j * game_data->block_size, i * game_data->block_size, 0xFFFFFFFF);
			j++;
		}
		i++;
	}
}

void ft_hook(void *arg)
{
	t_data *game_data;

	game_data = (t_data *) arg;
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game_data->mlx);
	if (game_data->player_pos.y >= 0 && game_data->player_pos.x >= 0)
	{
		if (mlx_is_key_down(game_data->mlx, MLX_KEY_UP))
		{
			//image->instances[0].y -= 5;
			game_data->map[(int)game_data->player_pos.y][(int)game_data->player_pos.x] = '0';
			game_data->map[(int)game_data->player_pos.y - 1][(int)game_data->player_pos.x] = 'S';
			game_data->player_pos.y -= 1;
		}
		if (mlx_is_key_down(game_data->mlx, MLX_KEY_DOWN))
		{
			//image->instances[0].y += 5;
			game_data->map[(int)game_data->player_pos.y][(int)game_data->player_pos.x] = '0';
			game_data->map[(int)game_data->player_pos.y + 1][(int)game_data->player_pos.x] = 'S';
			game_data->player_pos.y += 1;
		}
		if (mlx_is_key_down(game_data->mlx, MLX_KEY_LEFT))
		{
			//image->instances[0].x -= 5;
			game_data->map[(int)game_data->player_pos.y][(int)game_data->player_pos.x] = '0';
			game_data->map[(int)game_data->player_pos.y][(int)game_data->player_pos.x - 1] = 'S';
			game_data->player_pos.x -= 1;
		}
		if (mlx_is_key_down(game_data->mlx, MLX_KEY_RIGHT))
		{
			//image->instances[0].x += 5;
			game_data->map[(int)game_data->player_pos.y][(int)game_data->player_pos.x] = '0';
			game_data->map[(int)game_data->player_pos.y][(int)game_data->player_pos.x + 1] = 'S';
			game_data->player_pos.x += 1;
		}
	}
	put_mini_map(game_data);
}

void ft_randomize(void *arg)
{
	t_data *game_data;

	game_data = (t_data *)arg;
	for (uint32_t i = 0; i < 11; ++i)
	{
		for (uint32_t y = 0; y < 11; ++y)
		{
			uint32_t color = 0xFFFFFFFF;
			mlx_put_pixel(image, i, y, color);
		}
	}
}

void	rotation(t_point *vector, double theta)
{
	int	tmp_x;

	tmp_x = vector->x;
	vector->x = vector->x * cos(theta) - vector->y * sin(theta);
	vector->y = tmp_x * sin(theta) + vector->y * cos(theta);
}

void dir_init(t_data *game_data, double theta)
{
	game_data->player_dir.x = cos(theta);
	game_data->player_dir.y = sin(theta);
}
void plan_init(t_data *game_data, double x, double y)
{
	game_data->plan.x = x;
	game_data->plan.y = y;
}

void	r_c_init(t_data *game_data)
{
	if (game_data->starting_dir == 'N')
	{
		dir_init(game_data, 3 * PI / 2);
		plan_init(game_data, 0.66, 0);
	}
	if (game_data->starting_dir == 'E')
	{
		dir_init(game_data, 0);
		plan_init(game_data, 0, 0.66);
	}
	if (game_data->starting_dir == 'S')
	{
		dir_init(game_data, PI / 2);
		plan_init(game_data, -0.66, 0);
	}
	if (game_data->starting_dir == 'W')
	{
		dir_init(game_data, PI);
		plan_init(game_data, 0, -0.66);
	}
}

void	ray_caster(t_data *game_data)
{
	int x;

	x = 0;
	while (x < WIDTH)
	{
		game_data->camerax = 2 * x / (double)WIDTH - 1;
		game_data->raydir.x = game_data->player_dir.x + game_data->plan.x * game_data->camerax;
		game_data->raydir.y = game_data->player_dir.y + game_data->plan.y * game_data->camerax;

		game_data->player_pos_box.x = (int)game_data->player_pos.x;
		game_data->player_pos_box.y = (int)game_data->player_pos.y;
		game_data->delta_dist.x = fabs(1 / game_data->raydir.x);
		game_data->delta_dist.y = fabs(1 / game_data->raydir.y);
		x++;
	}
}

void mlx_stuff(t_data *game_data)
{
	// needs protection!!!!!!!!!!!!!!!
	game_data->mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true);
	game_data->m_map = mlx_new_image(game_data->mlx, 340, 180);
	put_mini_map(game_data);
	mlx_image_to_window(game_data->mlx, game_data->m_map, 0, 0);
	image = mlx_new_image(game_data->mlx, 11, 11);
	mlx_image_to_window(game_data->mlx, image, 0, 0);
	ray_caster(game_data);
	mlx_loop_hook(game_data->mlx, ft_randomize, game_data);
	mlx_loop_hook(game_data->mlx, ft_hook, game_data);
	mlx_loop(game_data->mlx);
	mlx_terminate(game_data->mlx);
}