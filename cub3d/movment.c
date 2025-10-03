/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:29:55 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/03 20:30:03 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

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