/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_stuff.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thdaib <thdaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:44:06 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/04 15:01:05 by thdaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void	dda_init(t_data *var, int x)
{
	var->hit = 0;
	var->camerax = 2 * x / (double)WIDTH - 1;
	var->raydir.x = var->player_dir.x + var->plan.x * var->camerax;
	var->raydir.y = var->player_dir.y + var->plan.y * var->camerax;
	var->player_pos_box.x = (int)var->player_pos.x;
	var->player_pos_box.y = (int)var->player_pos.y;
	var->delta_dist.x = fabs(1 / var->raydir.x);
	var->delta_dist.y = fabs(1 / var->raydir.y);
}

void	dda_init_2(t_data *var)
{
	if (var->raydir.x < 0)
	{
		var->step.x = -1;
		var->side_dist.x = (var->player_pos.x - var->player_pos_box.x)
			* var->delta_dist.x;
	}
	else
	{
		var->step.x = 1;
		var->side_dist.x = (var->player_pos_box.x + 1.0 - var->player_pos.x)
			* var->delta_dist.x;
	}
	if (var->raydir.y < 0)
	{
		var->step.y = -1;
		var->side_dist.y = (var->player_pos.y - var->player_pos_box.y)
			* var->delta_dist.y;
	}
	else
	{
		var->step.y = 1;
		var->side_dist.y = (var->player_pos_box.y + 1.0 - var->player_pos.y)
			* var->delta_dist.y;
	}
}

void	dda_loop(t_data *var)
{
	while (var->hit == 0)
	{
		if (var->side_dist.x < var->side_dist.y)
		{
			var->side_dist.x += var->delta_dist.x;
			var->player_pos_box.x += var->step.x;
			var->side = 0;
		}
		else
		{
			var->side_dist.y += var->delta_dist.y;
			var->player_pos_box.y += var->step.y;
			var->side = 1;
		}
		if (var->player_pos_box.y >= 0 && var->player_pos_box.y < var->map_rows
			&& var->player_pos_box.x >= 0 && var->player_pos_box.x
			< var->map_cols
			&& var->map[var->player_pos_box.y][var->player_pos_box.x] == '1')
			var->hit = 1;
	}
}

void	get_wall_height(t_data *var, int *wallstart, int *wallend)
{
	if (var->side == 0)
		var->perpwalldist = var->side_dist.x - var->delta_dist.x;
	else
		var->perpwalldist = var->side_dist.y - var->delta_dist.y;
	if (var->perpwalldist == 0)
		var->lineheight = (int)(HEIGHT / 0.000001);
	else
		var->lineheight = (int)(HEIGHT / var->perpwalldist);
	*wallstart = -var->lineheight / 2 + HEIGHT / 2;
	*wallend = var->lineheight / 2 + HEIGHT / 2;
}

void	ray_caster(void *arg)
{
	t_data	*var;
	int		x;
	int		wallstart;
	int		wallend;

	var = (t_data *)arg;
	if (!var->needs_redraw)
		return ;
	var->needs_redraw = 0;
	x = 0;
	while (x < WIDTH)
	{
		dda_init(var, x);
		dda_init_2(var);
		dda_loop(var);
		get_wall_height(var, &wallstart, &wallend);
		draw_vertical_line(var, x, wallstart, wallend);
		x++;
	}
	put_mini_map(var);
}
