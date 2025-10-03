/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:46:31 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/03 20:47:25 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void free_mlx_stuff(t_data *game_data)
{
	int i;

	i = 0;
	if (game_data->world)
		mlx_delete_image(game_data->mlx, game_data->world);
	if (game_data->m_map)
		mlx_delete_image(game_data->mlx, game_data->m_map);
	while (i < 4)
	{
		if (game_data->textures[i])
			mlx_delete_texture(game_data->textures[i]);
		i++;
	}
	mlx_terminate(game_data->mlx);
}

void draw_ceiling_floor(t_data *game_data, int x, int start, int end)
{
	uint32_t ceiling_color;
	uint32_t floor_color;
	int y;

	y = 0;
	ceiling_color = rgb_to_int(game_data->ceiling_rgb);
	floor_color = rgb_to_int(game_data->floor_rgb);
	while (y < start)
	{
		mlx_put_pixel(game_data->world, x, y, ceiling_color);
		y++;
	}
	y = end + 1;
	while (y < HEIGHT)
	{
		mlx_put_pixel(game_data->world, x, y, floor_color);
		y++;
	}
}

void draw_vertical_line(t_data *game_data, int x, int start, int end)
{
	int texnum;
	double wallx;
	int texx;

	if (x < 0 || x >= WIDTH)
		return;
	if (start < 0)
		start = 0;
	if (end >= HEIGHT)
		end = HEIGHT - 1;
	draw_ceiling_floor(game_data, x, start, end);
	if (start <= end)
	{
		if (game_data->textures[0] && game_data->textures[1] && game_data->textures[2] && game_data->textures[3])
		{
			Determine_texture(game_data, &texnum);
			calculate_wall_hit_pos_to_texture(game_data, &wallx, &texx, &texnum);
			draw_texture_on_wall(game_data, &texnum, &texx, start, end, x);
		}
	}
}
