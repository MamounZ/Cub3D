/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:22:37 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/04 12:16:39 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void put_block(t_data *var, int x, int y, int size, uint32_t color)
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
				mlx_put_pixel(var->m_map, j + x, i + y, color);
			j++;
		}
		i++;
	}
}

void draw_mm_blocks(t_data *var, int i, int j)
{
	int player_size;
	int player_x;
	int player_y;

	if (var->map[i][j] == '1')
		put_block(var, j * var->block_size, i * var->block_size, var->block_size, 0x000000FF);
	else if (var->map[i][j] == '0' || ft_strchr("NSEW", var->map[i][j]))
		put_block(var, j * var->block_size, i * var->block_size, var->block_size, 0xB0B0B0FF);
	if (j == (int)var->player_pos.x && i == (int)var->player_pos.y)
	{
		player_size = var->block_size * 0.4;
		player_x = (int)(var->player_pos.x * var->block_size - player_size / 2);
		player_y = (int)(var->player_pos.y * var->block_size - player_size / 2);
		put_block(var, player_x, player_y, player_size, 0xFFFFFFFF);
	}
}

void put_mini_map(t_data *var)
{
	int i;
	int j;
	int block_size_w;
	int block_size_h;

	i = 0;
	block_size_w = 340 / var->map_cols;
	block_size_h = 180 / var->map_rows;
	if (block_size_w < block_size_h)
		var->block_size = block_size_w;
	else
		var->block_size = block_size_h;
	while (i < var->map_rows)
	{
		j = 0;
		while (j < var->map_cols && var->map[i][j])
		{
			draw_mm_blocks(var, i, j);
			j++;
		}
		i++;
	}
}
