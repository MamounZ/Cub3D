/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_stuff.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:44:06 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/03 20:44:19 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

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