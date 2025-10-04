/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thdaib <thdaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:29:55 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/04 14:12:34 by thdaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void	rotation(t_dpoint *vector, double theta)
{
	double	tmp_x;

	tmp_x = vector->x;
	vector->x = vector->x * cos(theta) - vector->y * sin(theta);
	vector->y = tmp_x * sin(theta) + vector->y * cos(theta);
}

void	normalize(t_dpoint *vector)
{
	double	magnitude;

	magnitude = sqrt(vector->x * vector->x + vector->y * vector->y);
	if (magnitude > 0)
	{
		vector->x /= magnitude;
		vector->y /= magnitude;
	}
}

int	is_position_safe(t_data *var, double x, double y, double buffer)
{
	int	check_y;
	int	check_x;

	var->min_x = (int)(x - buffer);
	var->max_x = (int)(x + buffer);
	var->min_y = (int)(y - buffer);
	var->max_y = (int)(y + buffer);
	check_y = var->min_y;
	while (check_y <= var->max_y)
	{
		check_x = var->min_x;
		while (check_x <= var->max_x)
		{
			if (check_x < 0 || check_x >= var->map_cols
				|| check_y < 0 || check_y >= var->map_rows)
				return (0);
			if (var->map[check_y][check_x] == '1')
				return (0);
			check_x++;
		}
		check_y++;
	}
	return (1);
}

void	move_player_with_sliding(t_data *var, double buffer)
{
	double	next_x;
	double	next_y;
	int		can_move_x;
	int		can_move_y;

	next_x = var->player_pos.x + var->move.x;
	next_y = var->player_pos.y + var->move.y;
	can_move_x = is_position_safe(var, next_x, var->player_pos.y, buffer);
	can_move_y = is_position_safe(var, var->player_pos.x, next_y, buffer);
	if (can_move_x && can_move_y)
	{
		var->player_pos.x = next_x;
		var->player_pos.y = next_y;
	}
	else if (can_move_x)
	{
		var->player_pos.x = next_x;
	}
	else if (can_move_y)
	{
		var->player_pos.y = next_y;
	}
}
