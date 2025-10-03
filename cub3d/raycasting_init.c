/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:34:36 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/03 20:35:50 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

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
	normalize(&game_data->plan);
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
