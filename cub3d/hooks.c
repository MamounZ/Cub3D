/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:26:28 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/03 20:41:36 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

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
