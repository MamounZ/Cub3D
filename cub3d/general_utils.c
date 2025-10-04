/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:46:31 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/04 12:16:39 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void free_mlx_stuff(t_data *var)
{
	int i;

	i = 0;
	if (var->world)
		mlx_delete_image(var->mlx, var->world);
	if (var->m_map)
		mlx_delete_image(var->mlx, var->m_map);
	while (i < 4)
	{
		if (var->textures[i])
			mlx_delete_texture(var->textures[i]);
		i++;
	}
	mlx_terminate(var->mlx);
}

void draw_ceiling_floor(t_data *var, int x, int start, int end)
{
	uint32_t ceiling_color;
	uint32_t floor_color;
	int y;

	y = 0;
	ceiling_color = rgb_to_int(var->ceiling_rgb);
	floor_color = rgb_to_int(var->floor_rgb);
	while (y < start)
	{
		mlx_put_pixel(var->world, x, y, ceiling_color);
		y++;
	}
	y = end + 1;
	while (y < HEIGHT)
	{
		mlx_put_pixel(var->world, x, y, floor_color);
		y++;
	}
}

void draw_vertical_line(t_data *var, int x, int start, int end)
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
	draw_ceiling_floor(var, x, start, end);
	if (start <= end)
	{
		if (var->textures[0] && var->textures[1] && var->textures[2] && var->textures[3])
		{
			Determine_texture(var, &texnum);
			calculate_wall_hit_pos_to_texture(var, &wallx, &texx, &texnum);
			draw_texture_on_wall(var, &texnum, &texx, start, end, x);
		}
	}
}
