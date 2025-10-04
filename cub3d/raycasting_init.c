/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:34:36 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/04 12:16:39 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void dir_init(t_data *var, double theta)
{
	var->player_dir.x = cos(theta);
	var->player_dir.y = sin(theta);
	normalize(&var->player_dir);
}

void plan_init(t_data *var, double x, double y)
{
	var->plan.x = x;
	var->plan.y = y;
	normalize(&var->plan);
}

void r_c_init(t_data *var)
{
	if (var->starting_dir == 'N')
	{
		dir_init(var, 3 * PI / 2);
		plan_init(var, 0.66, 0);
	}
	else if (var->starting_dir == 'E')
	{
		dir_init(var, 0);
		plan_init(var, 0, 0.66);
	}
	else if (var->starting_dir == 'S')
	{
		dir_init(var, PI / 2);
		plan_init(var, -0.66, 0);
	}
	else if (var->starting_dir == 'W')
	{
		dir_init(var, PI);
		plan_init(var, 0, -0.66);
	}
}
