/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:22:37 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/03 20:32:51 by mazaid           ###   ########.fr       */
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
	int block_size_w;
	int block_size_h;

	i = 0;
	block_size_w = 340 / game_data->map_cols;
	block_size_h = 180 / game_data->map_rows;
	if (block_size_w < block_size_h)
		game_data->block_size = block_size_w;
	else
		game_data->block_size = block_size_h;
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
