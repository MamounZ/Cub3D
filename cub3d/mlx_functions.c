/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:10:56 by mazaid            #+#    #+#             */
/*   Updated: 2025/09/25 20:25:05 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void put_block(t_data *game_data, int x, int y, int size, uint32_t color)
{
	int i;
	int j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			if (j + x >= 0 && j + x < 340 && i + y >= 0 && i + y < 180)
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
	int player_size;
	int player_x;
	int player_y;


	i = 0;
	if (game_data->map_rows < game_data->map_cols)
		game_data->block_size = 340 / game_data->map_cols;
	else
		game_data->block_size = 340 / game_data->map_rows;
	while (i < game_data->map_rows)
	{
		j = 0;
		while (j < game_data->map_cols && game_data->map[i][j])
		{
			if (game_data->map[i][j] == '1')
				put_block(game_data, j * game_data->block_size, i * game_data->block_size, game_data->block_size, 0x000000FF);
			else if (game_data->map[i][j] == '0' || ft_strchr("NSEW", game_data->map[i][j]))
				put_block(game_data, j * game_data->block_size, i * game_data->block_size, game_data->block_size, 0xB0B0B0FF);
			if (j == (int)game_data->player_pos.x && i == (int)game_data->player_pos.y)
			{
				player_size = game_data->block_size * 0.4;
				player_x = (int)(game_data->player_pos.x * game_data->block_size - player_size / 2);
				player_y = (int)(game_data->player_pos.y * game_data->block_size - player_size / 2);
				put_block(game_data, player_x, player_y, player_size, 0xFFFFFFFF);
			}
			j++;
		}
		i++;
	}
}
void rotation(t_dpoint *vector, double theta)
{
	double tmp_x;

	tmp_x = vector->x;
	vector->x = vector->x * cos(theta) - vector->y * sin(theta);
	vector->y = tmp_x * sin(theta) + vector->y * cos(theta);
}
void normalize(t_dpoint *vector)
{
	double magnitude = sqrt(vector->x * vector->x + vector->y * vector->y);
	if (magnitude > 0)
	{
		vector->x /= magnitude;
		vector->y /= magnitude;
	}
}

int is_position_safe(t_data *game_data, double x, double y, double buffer)
{
	// Calculate the bounding box of the player with buffer
	int min_x = (int)(x - buffer);
	int max_x = (int)(x + buffer);
	int min_y = (int)(y - buffer);
	int max_y = (int)(y + buffer);

	// Check all cells that the buffer zone overlaps
	for (int check_y = min_y; check_y <= max_y; check_y++)
	{
		for (int check_x = min_x; check_x <= max_x; check_x++)
		{
			// Check bounds
			if (check_x < 0 || check_x >= game_data->map_cols ||
				check_y < 0 || check_y >= game_data->map_rows)
				return 0; // Outside map bounds

			// Check if cell contains a wall
			if (game_data->map[check_y][check_x] == '1')
				return 0; // Wall detected in buffer zone
		}
	}
	return 1; // Position is safe
}

void move_player_with_sliding(t_data *game_data, double move_x, double move_y, double buffer)
{
	double next_x = game_data->player_pos.x + move_x;
	double next_y = game_data->player_pos.y + move_y;

	// Try to move in both directions
	int can_move_x = is_position_safe(game_data, next_x, game_data->player_pos.y, buffer);
	int can_move_y = is_position_safe(game_data, game_data->player_pos.x, next_y, buffer);

	// Apply movement based on what's possible
	if (can_move_x && can_move_y)
	{
		// Can move in both directions - normal diagonal movement
		game_data->player_pos.x = next_x;
		game_data->player_pos.y = next_y;
	}
	else if (can_move_x)
	{
		// Can only move in X direction - slide along Y-axis wall
		game_data->player_pos.x = next_x;
	}
	else if (can_move_y)
	{
		// Can only move in Y direction - slide along X-axis wall
		game_data->player_pos.y = next_y;
	}
	// If neither direction is possible, player stays in place
}

void ft_hook(void *arg)
{
	t_data *game_data = (t_data *)arg;
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game_data->mlx);

	double moveSpeed = 0.05;
	double rotSpeed = 0.04;
	double buffer = 0.1;

	// Move forward
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_W))
	{
		double move_x = game_data->player_dir.x * moveSpeed;
		double move_y = game_data->player_dir.y * moveSpeed;
		move_player_with_sliding(game_data, move_x, move_y, buffer);
	}

	// Move backward
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_S))
	{
		double move_x = -game_data->player_dir.x * moveSpeed;
		double move_y = -game_data->player_dir.y * moveSpeed;
		move_player_with_sliding(game_data, move_x, move_y, buffer);
	}

	// Move right (strafe)
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_D))
	{
		double move_x = -game_data->player_dir.y * moveSpeed;
		double move_y = game_data->player_dir.x * moveSpeed;
		move_player_with_sliding(game_data, move_x, move_y, buffer);
	}

	// Move left (strafe)
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_A))
	{
		double move_x = game_data->player_dir.y * moveSpeed;
		double move_y = -game_data->player_dir.x * moveSpeed;
		move_player_with_sliding(game_data, move_x, move_y, buffer);
	}
	// Rotate right
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_RIGHT))
	{
		rotation(&game_data->player_dir, rotSpeed);
		rotation(&game_data->plan, rotSpeed);
		normalize(&game_data->player_dir); // Prevent drift
		normalize(&game_data->plan); // Keep plan consistent
	}
	// Rotate left
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_LEFT))
	{
		rotation(&game_data->player_dir, -rotSpeed);
		rotation(&game_data->plan, -rotSpeed);
		normalize(&game_data->player_dir); // Prevent drift
		normalize(&game_data->plan); // Keep plan consistent
	}
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_W) ||
		mlx_is_key_down(game_data->mlx, MLX_KEY_S) ||
		mlx_is_key_down(game_data->mlx, MLX_KEY_A) ||
		mlx_is_key_down(game_data->mlx, MLX_KEY_D) ||
		mlx_is_key_down(game_data->mlx, MLX_KEY_LEFT) ||
		mlx_is_key_down(game_data->mlx, MLX_KEY_RIGHT))
	{
		game_data->needs_redraw = 1;
	}
}

void dir_init(t_data *game_data, double theta)
{
	game_data->player_dir.x = cos(theta);
	game_data->player_dir.y = sin(theta);
	normalize(&game_data->player_dir);
}

void plan_init(t_data *game_data, double x, double y)
{
	game_data->plan.x = x;
	game_data->plan.y = y;
	normalize(&game_data->plan); // Keep plan consistent
}

void r_c_init(t_data *game_data)
{
	if (game_data->starting_dir == 'N')
	{
		dir_init(game_data, 3 * PI / 2);
		plan_init(game_data, 0.66, 0);
	}
	else if (game_data->starting_dir == 'E')
	{
		dir_init(game_data, 0);
		plan_init(game_data, 0, 0.66);
	}
	else if (game_data->starting_dir == 'S')
	{
		dir_init(game_data, PI / 2);
		plan_init(game_data, -0.66, 0);
	}
	else if (game_data->starting_dir == 'W')
	{
		dir_init(game_data, PI);
		plan_init(game_data, 0, -0.66);
	}
}

uint32_t get_texture_pixel(mlx_texture_t *texture, int x, int y)
{
	if (x < 0 || x >= (int)texture->width || y < 0 || y >= (int)texture->height)
		return 0xFF0000FF; // Red for error
	uint8_t *pixel = &texture->pixels[(y * texture->width + x) * 4];
	return (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
}
int rgb_to_int(int *rgb)
{
	return ((rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 0xFF);
}

void draw_vertical_line(t_data *game_data, int x, int start, int end)
{
	if (x < 0 || x >= WIDTH)
		return;
	if (start < 0)
		start = 0;
	if (end >= HEIGHT)
		end = HEIGHT - 1;

	uint32_t ceiling_color = rgb_to_int(game_data->ceiling_rgb);
	uint32_t floor_color = rgb_to_int(game_data->floor_rgb);
	printf("ceiling_color is %x\n", ceiling_color);
	printf("floor_color is %x\n", floor_color);

	for (int y = 0; y < start; y++)
	{
		mlx_put_pixel(game_data->world, x, y, ceiling_color);
	}
	if (start <= end)
	{
		if (game_data->textures[0] && game_data->textures[1] && game_data->textures[2] && game_data->textures[3])
		{
			int texNum = 0; // Determine texture: 0=NO, 1=SO, 2=EA, 3=WE
			if (game_data->side == 0 && game_data->raydir.x > 0)
				texNum = 2; // EA
			else if (game_data->side == 0 && game_data->raydir.x <= 0)
				texNum = 3; // WE
			else if (game_data->side == 1 && game_data->raydir.y > 0)
				texNum = 1; // SO
			else if (game_data->side == 1 && game_data->raydir.y <= 0)
				texNum = 0; // NO

			double wallX;
			if (game_data->side == 0)
				wallX = game_data->player_pos.y + game_data->perpwalldist * game_data->raydir.y;
			else
				wallX = game_data->player_pos.x + game_data->perpwalldist * game_data->raydir.x;
			wallX -= floor(wallX);

			int texX = (int)(wallX * (double)game_data->textures[texNum]->width);
			// merroring for west and north textures
			if ((game_data->side == 0 && game_data->raydir.x <= 0) || (game_data->side == 1 && game_data->raydir.y >= 0))
				texX = game_data->textures[texNum]->width - texX - 1;

			double step = 1.0 * game_data->textures[texNum]->height / game_data->lineHeight;
			double texPos = (start - HEIGHT / 2 + game_data->lineHeight / 2) * step;
			for (int y = start; y <= end; y++)
			{
				int texY = (int)texPos % game_data->textures[texNum]->height;
				if (texY < 0)
					texY += game_data->textures[texNum]->height;
				texPos += step;
				uint32_t color = get_texture_pixel(game_data->textures[texNum], texX, texY);
				mlx_put_pixel(game_data->world, x, y, color);
			}
		}
	}
	for (int y = end + 1; y < HEIGHT; y++)
	{
		mlx_put_pixel(game_data->world, x, y, floor_color);
	}
}

void ray_caster(void *arg)
{
	t_data *game_data = (t_data *)arg;

	if (!game_data->needs_redraw)
		return;

	game_data->needs_redraw = 0;
	// Raycasting
	for (int x = 0; x < WIDTH; x++)
	{
		game_data->hit = 0;
		game_data->camerax = 2 * x / (double)WIDTH - 1;
		game_data->raydir.x = game_data->player_dir.x + game_data->plan.x * game_data->camerax;
		game_data->raydir.y = game_data->player_dir.y + game_data->plan.y * game_data->camerax;
		game_data->player_pos_box.x = (int)game_data->player_pos.x;
		game_data->player_pos_box.y = (int)game_data->player_pos.y;
		game_data->delta_dist.x = (game_data->raydir.x == 0) ? 1e30 : fabs(1 / game_data->raydir.x);
		game_data->delta_dist.y = (game_data->raydir.y == 0) ? 1e30 : fabs(1 / game_data->raydir.y);

		if (game_data->raydir.x < 0)
		{
			game_data->step.x = -1;
			game_data->side_dist.x = (game_data->player_pos.x - game_data->player_pos_box.x) * game_data->delta_dist.x;
		}
		else
		{
			game_data->step.x = 1;
			game_data->side_dist.x = (game_data->player_pos_box.x + 1.0 - game_data->player_pos.x) * game_data->delta_dist.x;
		}
		if (game_data->raydir.y < 0)
		{
			game_data->step.y = -1;
			game_data->side_dist.y = (game_data->player_pos.y - game_data->player_pos_box.y) * game_data->delta_dist.y;
		}
		else
		{
			game_data->step.y = 1;
			game_data->side_dist.y = (game_data->player_pos_box.y + 1.0 - game_data->player_pos.y) * game_data->delta_dist.y;
		}

		while (game_data->hit == 0)
		{
			if (game_data->side_dist.x < game_data->side_dist.y)
			{
				game_data->side_dist.x += game_data->delta_dist.x;
				game_data->player_pos_box.x += game_data->step.x;
				game_data->side = 0;
			}
			else
			{
				game_data->side_dist.y += game_data->delta_dist.y;
				game_data->player_pos_box.y += game_data->step.y;
				game_data->side = 1;
			}
			if (game_data->player_pos_box.y >= 0 && game_data->player_pos_box.y < game_data->map_rows &&
				game_data->player_pos_box.x >= 0 && game_data->player_pos_box.x < game_data->map_cols &&
				game_data->map[game_data->player_pos_box.y][game_data->player_pos_box.x] == '1')
				game_data->hit = 1;
		}

		if (game_data->side == 0)
			game_data->perpwalldist = game_data->side_dist.x - game_data->delta_dist.x;
		else
			game_data->perpwalldist = game_data->side_dist.y - game_data->delta_dist.y;

		game_data->lineHeight = (int)(HEIGHT / game_data->perpwalldist);
		int drawStart = -game_data->lineHeight / 2 + HEIGHT / 2;
		int drawEnd = game_data->lineHeight / 2 + HEIGHT / 2;
		draw_vertical_line(game_data, x, drawStart, drawEnd);
	}

	// Draw minimap

	put_mini_map(game_data);
}

void mlx_stuff(t_data *game_data)
{
	// Initialize MLX
	game_data->mlx = mlx_init(WIDTH, HEIGHT, "Cub3D", true);
	if (!game_data->mlx)
	{
		printf("Error: MLX initialization failed\n");
		exit(1);
	}

	// Load textures
	if (!game_data->no_tex || !game_data->so_tex || !game_data->we_tex || !game_data->ea_tex)
	{
		printf("Error: Missing texture path\n");
		exit(1);
	}
	game_data->textures[0] = mlx_load_png(game_data->no_tex);
	if (!game_data->textures[0])
	{
		printf("Error: Failed to load texture %s\n", game_data->no_tex);
		exit(1);
	}
	game_data->textures[1] = mlx_load_png(game_data->so_tex);
	if (!game_data->textures[1])
	{
		printf("Error: Failed to load texture %s\n", game_data->so_tex);
		exit(1);
	}
	game_data->textures[2] = mlx_load_png(game_data->ea_tex);
	if (!game_data->textures[2])
	{
		printf("Error: Failed to load texture %s\n", game_data->ea_tex);
		exit(1);
	}
	game_data->textures[3] = mlx_load_png(game_data->we_tex);
	if (!game_data->textures[3])
	{
		printf("Error: Failed to load texture %s\n", game_data->we_tex);
		exit(1);
	}

	// Initialize images
	game_data->world = mlx_new_image(game_data->mlx, WIDTH, HEIGHT);
	if (!game_data->world)
	{
		printf("Error: Failed to create world image\n");
		exit(1);
	}
	game_data->m_map = mlx_new_image(game_data->mlx, 340, 180);
	if (!game_data->m_map)
	{
		printf("Error: Failed to create minimap image\n");
		exit(1);
	}

	// Display images ONCE during initialization
	mlx_image_to_window(game_data->mlx, game_data->world, 0, 0);
	mlx_image_to_window(game_data->mlx, game_data->m_map, 0, 0);

	// Initialize needs_redraw
	game_data->needs_redraw = 1; // Force initial render

	// Initialize player direction
	r_c_init(game_data);

	// Start rendering
	mlx_loop_hook(game_data->mlx, ray_caster, game_data);
	mlx_loop_hook(game_data->mlx, ft_hook, game_data);
	mlx_loop(game_data->mlx);
}