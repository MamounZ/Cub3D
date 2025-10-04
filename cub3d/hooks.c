/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thdaib <thdaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:26:28 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/04 13:42:03 by thdaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void	mouse_hook(double xpos, double ypos, void *param)
{
	t_data	*var;
	int		delta_x;
	double	rot_angle;

	var = (t_data *)param;
	(void)ypos;
	if (!var->mouse_initialized)
	{
		mlx_set_mouse_pos(var->mlx, WIDTH / 2, HEIGHT / 2);
		var->mouse_initialized = 1;
		return ;
	}
	delta_x = (int)xpos - WIDTH / 2;
	if (delta_x != 0)
	{
		rot_angle = delta_x * var->mouse_sensitivity;
		rotation(&var->player_dir, rot_angle);
		rotation(&var->plan, rot_angle);
		normalize(&var->player_dir);
		normalize(&var->plan);
		var->needs_redraw = 1;
	}
	mlx_set_mouse_pos(var->mlx, WIDTH / 2, HEIGHT / 2);
}

void	movment_hooks(t_data *var, double movespeed, double buffer)
{
	if (mlx_is_key_down(var->mlx, MLX_KEY_W))
	{
		var->move.x = var->player_dir.x * movespeed;
		var->move.y = var->player_dir.y * movespeed;
		move_player_with_sliding(var, buffer);
		var->needs_redraw = 1;
	}
	if (mlx_is_key_down(var->mlx, MLX_KEY_S))
	{
		var->move.x = -var->player_dir.x * movespeed;
		var->move.y = -var->player_dir.y * movespeed;
		move_player_with_sliding(var, buffer);
		var->needs_redraw = 1;
	}
}

void	movment_hooks2(t_data *var, double movespeed, double buffer)
{
	if (mlx_is_key_down(var->mlx, MLX_KEY_D))
	{
		var->move.x = -var->player_dir.y * movespeed;
		var->move.y = var->player_dir.x * movespeed;
		move_player_with_sliding(var, buffer);
		var->needs_redraw = 1;
	}
	if (mlx_is_key_down(var->mlx, MLX_KEY_A))
	{
		var->move.x = var->player_dir.y * movespeed;
		var->move.y = -var->player_dir.x * movespeed;
		move_player_with_sliding(var, buffer);
		var->needs_redraw = 1;
	}
}

void	rotation_hooks(t_data *var, double rotspeed)
{
	if (mlx_is_key_down(var->mlx, MLX_KEY_RIGHT))
	{
		rotation(&var->player_dir, rotspeed);
		rotation(&var->plan, rotspeed);
		normalize(&var->player_dir);
		normalize(&var->plan);
		var->needs_redraw = 1;
	}
	if (mlx_is_key_down(var->mlx, MLX_KEY_LEFT))
	{
		rotation(&var->player_dir, -rotspeed);
		rotation(&var->plan, -rotspeed);
		normalize(&var->player_dir);
		normalize(&var->plan);
		var->needs_redraw = 1;
	}
}

void	ft_hook(void *arg)
{
	t_data	*var;
	double	movespeed;
	double	rotspeed;
	double	buffer;

	var = (t_data *)arg;
	movespeed = 0.05;
	rotspeed = 0.04;
	buffer = 0.1;
	if (mlx_is_key_down(var->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(var->mlx);
	movment_hooks(var, movespeed, buffer);
	movment_hooks2(var, movespeed, buffer);
	rotation_hooks(var, rotspeed);
}
