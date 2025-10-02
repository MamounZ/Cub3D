/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:10:56 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/02 17:33:09 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void free_mlx_stuff(t_data *game_data)
{
	int i;

	i = 0;
	if (game_data->world)
		mlx_delete_image(game_data->mlx, game_data->world);
	if (game_data->m_map)
		mlx_delete_image(game_data->mlx, game_data->m_map);
	while (i < 4)
	{
		if (game_data->textures[i])
			mlx_delete_texture(game_data->textures[i]);
		i++;
	}
	mlx_terminate(game_data->mlx);
}

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

void draw_mm_blocks(t_data *game_data, int i, int j)
{
	int player_size;
	int player_x;
	int player_y;

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
}

void put_mini_map(t_data *game_data)
{
	int i;
	int j;

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
			draw_mm_blocks(game_data, i, j);
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
	double magnitude;

	magnitude = sqrt(vector->x * vector->x + vector->y * vector->y);
	if (magnitude > 0)
	{
		vector->x /= magnitude;
		vector->y /= magnitude;
	}
}

int is_position_safe(t_data *game_data, double x, double y, double buffer)
{
	int min_x;
	int max_x;
	int min_y;
	int max_y;

	min_x = (int)(x - buffer);
	max_x = (int)(x + buffer);
	min_y = (int)(y - buffer);
	max_y = (int)(y + buffer);
	for (int check_y = min_y; check_y <= max_y; check_y++)
	{
		for (int check_x = min_x; check_x <= max_x; check_x++)
		{
			if (check_x < 0 || check_x >= game_data->map_cols ||
				check_y < 0 || check_y >= game_data->map_rows)
				return (0);
			if (game_data->map[check_y][check_x] == '1')
				return (0);
		}
	}
	return (1);
}

void move_player_with_sliding(t_data *game_data, double buffer)
{
	double next_x;
	double next_y;
	int can_move_x;
	int can_move_y;

	next_x = game_data->player_pos.x + game_data->move.x;
	next_y = game_data->player_pos.y + game_data->move.y;
	can_move_x = is_position_safe(game_data, next_x, game_data->player_pos.y, buffer);
	can_move_y = is_position_safe(game_data, game_data->player_pos.x, next_y, buffer);
	if (can_move_x && can_move_y)
	{
		game_data->player_pos.x = next_x;
		game_data->player_pos.y = next_y;
	}
	else if (can_move_x)
	{
		game_data->player_pos.x = next_x;
	}
	else if (can_move_y)
	{
		game_data->player_pos.y = next_y;
	}
}
void movment_hooks(t_data *game_data, double moveSpeed, double buffer)
{
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_W))
	{
		game_data->move.x = game_data->player_dir.x * moveSpeed;
		game_data->move.y = game_data->player_dir.y * moveSpeed;
		move_player_with_sliding(game_data, buffer);
		game_data->needs_redraw = 1;
	}
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_S))
	{
		game_data->move.x = -game_data->player_dir.x * moveSpeed;
		game_data->move.y = -game_data->player_dir.y * moveSpeed;
		move_player_with_sliding(game_data, buffer);
		game_data->needs_redraw = 1;
	}
}
void movment_hooks2(t_data *game_data, double moveSpeed, double buffer)
{
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_D))
	{
		game_data->move.x = -game_data->player_dir.y * moveSpeed;
		game_data->move.y = game_data->player_dir.x * moveSpeed;
		move_player_with_sliding(game_data, buffer);
		game_data->needs_redraw = 1;
	}
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_A))
	{
		game_data->move.x = game_data->player_dir.y * moveSpeed;
		game_data->move.y = -game_data->player_dir.x * moveSpeed;
		move_player_with_sliding(game_data, buffer);
		game_data->needs_redraw = 1;
	}
}

void rotation_hooks(t_data *game_data, double rotSpeed)
{
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_RIGHT))
	{
		rotation(&game_data->player_dir, rotSpeed);
		rotation(&game_data->plan, rotSpeed);
		normalize(&game_data->player_dir);
		normalize(&game_data->plan);
		game_data->needs_redraw = 1;
	}
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_LEFT))
	{
		rotation(&game_data->player_dir, -rotSpeed);
		rotation(&game_data->plan, -rotSpeed);
		normalize(&game_data->player_dir);
		normalize(&game_data->plan);
		game_data->needs_redraw = 1;
	}
}

void ft_hook(void *arg)
{
	t_data *game_data;
	double moveSpeed;
	double rotSpeed;
	double buffer;

	game_data = (t_data *)arg;
	moveSpeed = 0.05;
	rotSpeed = 0.04;
	buffer = 0.1;
	if (mlx_is_key_down(game_data->mlx, MLX_KEY_ESCAPE))
		{
			// free_mlx_stuff(game_data);
			mlx_close_window(game_data->mlx);
		}
	movment_hooks(game_data, moveSpeed, buffer);
	movment_hooks2(game_data, moveSpeed, buffer);
	rotation_hooks(game_data, rotSpeed);
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
		return (0xFF0000FF); // Red for error
	uint8_t *pixel = &texture->pixels[(y * texture->width + x) * 4];
	return (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
}
int rgb_to_int(int *rgb)
{
	return ((rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 0xFF);
}

void Determine_texture(t_data *game_data, int *texnum)
{
	if (game_data->side == 0 && game_data->raydir.x > 0)
		*texnum = 2; // EA
	else if (game_data->side == 0 && game_data->raydir.x <= 0)
		*texnum = 3; // WE
	else if (game_data->side == 1 && game_data->raydir.y > 0)
		*texnum = 1; // SO
	else if (game_data->side == 1 && game_data->raydir.y <= 0)
		*texnum = 0; // NO
}
void calculate_wall_hit_pos_to_texture(t_data *game_data, double *wallx, int *texx, int *texnum)
{
	if (game_data->side == 0)
		*wallx = game_data->player_pos.y + game_data->perpwalldist * game_data->raydir.y;
	else
		*wallx = game_data->player_pos.x + game_data->perpwalldist * game_data->raydir.x;
	*wallx -= floor(*wallx);
	*texx = (int)(*wallx * (double)game_data->textures[*texnum]->width);
	// merroring for west and north textures
	if ((game_data->side == 0 && game_data->raydir.x <= 0) || (game_data->side == 1 && game_data->raydir.y >= 0))
		*texx = game_data->textures[*texnum]->width - *texx - 1;
}
void draw_texture_on_wall(t_data *game_data, int *texnum, int *texx, int start, int end, int x)
{
	double step;
	double texpos;
	int y;
	int texy;
	uint32_t color;

	step = 1.0 * game_data->textures[*texnum]->height / game_data->lineHeight;
	texpos = (start - HEIGHT / 2 + game_data->lineHeight / 2) * step;
	y = start;
	while(y <= end)
	{
		texy = (int)texpos % game_data->textures[*texnum]->height;
		if (texy < 0)
			texy += game_data->textures[*texnum]->height;
		texpos += step;
		color = get_texture_pixel(game_data->textures[*texnum], *texx, texy);
		mlx_put_pixel(game_data->world, x, y, color);
		y++;
	}
}

void draw_ceiling_floor(t_data *game_data, int x, int start, int end)
{
	uint32_t ceiling_color;
	uint32_t floor_color;
	int y;

	y = 0;
	ceiling_color = rgb_to_int(game_data->ceiling_rgb);
	floor_color = rgb_to_int(game_data->floor_rgb);
	while (y < start)
	{
		mlx_put_pixel(game_data->world, x, y, ceiling_color);
		y++;
	}
	y = end + 1;
	while (y < HEIGHT)
	{
		mlx_put_pixel(game_data->world, x, y, floor_color);
		y++;
	}
}

void draw_vertical_line(t_data *game_data, int x, int start, int end)
{
	int texnum;
	double wallx;
	int texx;

	if (x < 0 || x >= WIDTH)
		return;
	if (start < 0)
		start = 0;
	if (end >= HEIGHT)
		end = HEIGHT - 1;
	draw_ceiling_floor(game_data, x, start, end);
	if (start <= end)
	{
		if (game_data->textures[0] && game_data->textures[1] && game_data->textures[2] && game_data->textures[3])
		{
			Determine_texture(game_data, &texnum);
			calculate_wall_hit_pos_to_texture(game_data, &wallx, &texx, &texnum);
			draw_texture_on_wall(game_data, &texnum, &texx, start, end, x);
		}
	}
}
void dda_init(t_data *game_data, int x)
{
	game_data->hit = 0;
	game_data->camerax = 2 * x / (double)WIDTH - 1;
	game_data->raydir.x = game_data->player_dir.x + game_data->plan.x * game_data->camerax;
	game_data->raydir.y = game_data->player_dir.y + game_data->plan.y * game_data->camerax;
	game_data->player_pos_box.x = (int)game_data->player_pos.x;
	game_data->player_pos_box.y = (int)game_data->player_pos.y;
	game_data->delta_dist.x = fabs(1 / game_data->raydir.x);
	game_data->delta_dist.y = fabs(1 / game_data->raydir.y);
}
void dda_init_2(t_data *game_data)
{
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
}
void dda_loop(t_data *game_data)
{
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
}
void get_wall_height(t_data *game_data, int *wallstart, int *wallend)
{
	if (game_data->side == 0)
		game_data->perpwalldist = game_data->side_dist.x - game_data->delta_dist.x;
	else
		game_data->perpwalldist = game_data->side_dist.y - game_data->delta_dist.y;
	if (game_data->perpwalldist == 0)
		game_data->lineHeight = (int)(HEIGHT / 0.000001);
	else
		game_data->lineHeight = (int)(HEIGHT / game_data->perpwalldist);
	*wallstart = -game_data->lineHeight / 2 + HEIGHT / 2;
	*wallend = game_data->lineHeight / 2 + HEIGHT / 2;
}

void ray_caster(void *arg)
{
	t_data *game_data;
	int x;
	int wallstart;
	int wallend;

	game_data = (t_data *)arg;
	if (!game_data->needs_redraw)
		return;
	game_data->needs_redraw = 0;
	x = 0;
	while (x < WIDTH)
	{
		dda_init(game_data, x);
		dda_init_2(game_data);
		dda_loop(game_data);
		get_wall_height(game_data, &wallstart, &wallend);
		draw_vertical_line(game_data, x, wallstart, wallend);
		x++;
	}
	put_mini_map(game_data);
}

void mouse_hook(double xpos, double ypos, void *param)
{
	t_data *game_data = (t_data *)param;
	int delta_x;
	double rot_angle;

	(void)ypos;
	if (!game_data->mouse_initialized)
	{
		mlx_set_mouse_pos(game_data->mlx, WIDTH / 2, HEIGHT / 2);
		game_data->mouse_initialized = 1;
		return;
	}
	delta_x = (int)xpos - WIDTH / 2;
	if (delta_x != 0)
	{
		rot_angle = delta_x * game_data->mouse_sensitivity;
		rotation(&game_data->player_dir, rot_angle);
		rotation(&game_data->plan, rot_angle);
		normalize(&game_data->player_dir);
		normalize(&game_data->plan);
		game_data->needs_redraw = 1;
	}
	mlx_set_mouse_pos(game_data->mlx, WIDTH / 2, HEIGHT / 2);
}

void mlx_stuff_init(t_data *game_data)
{
	int i;

	i = 0;
	game_data->mlx = mlx_init(WIDTH, HEIGHT, "Cub3D", true);
	if (!game_data->mlx)
		free_and_exit(game_data, "MLX initialization failed", 1);
	while (i < 4)
	{
		game_data->textures[i] = mlx_load_png(game_data->tex_paths[i]);
		if (!game_data->textures[i])
			free_and_exit(game_data, "Failed to load texture", 1);
		i++;
	}
	game_data->world = mlx_new_image(game_data->mlx, WIDTH, HEIGHT);
	if (!game_data->world)
		free_and_exit(game_data, "Failed to create world image", 1);
	game_data->m_map = mlx_new_image(game_data->mlx, 340, 180);
	if (!game_data->m_map)
		free_and_exit(game_data, "Failed to create minimap image", 1);
	if (mlx_image_to_window(game_data->mlx, game_data->world, 0, 0) == -1)
		free_and_exit(game_data, "Failed to load game image", 1);
	if (mlx_image_to_window(game_data->mlx, game_data->m_map, 0, 0) == -1)
		free_and_exit(game_data, "Failed to load minimap image", 1);
}

void mlx_stuff(t_data *game_data)
{
	mlx_stuff_init(game_data);
	game_data->needs_redraw = 1;
	game_data->mouse_sensitivity = 0.00050;
	game_data->mouse_initialized = 0;
	r_c_init(game_data);
	mlx_loop_hook(game_data->mlx, ray_caster, game_data);
	mlx_loop_hook(game_data->mlx, ft_hook, game_data);
	mlx_cursor_hook(game_data->mlx, &mouse_hook, game_data);
	mlx_set_cursor_mode(game_data->mlx, MLX_MOUSE_HIDDEN);
	mlx_loop(game_data->mlx);
}
