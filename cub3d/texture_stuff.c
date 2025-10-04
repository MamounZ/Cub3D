/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_stuff.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thdaib <thdaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:38:49 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/04 15:01:05 by thdaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

uint32_t	get_texture_pixel(mlx_texture_t *texture, int x, int y)
{
	uint8_t	*pixel;

	if (x < 0 || x >= (int)texture->width || y < 0 || y >= (int)texture->height)
		return (0xFF0000FF);
	pixel = &texture->pixels[(y * texture->width + x) * 4];
	return ((pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3]);
}

int	rgb_to_int(int *rgb)
{
	return ((rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 0xFF);
}

void	determine_texture(t_data *var)
{
	if (var->side == 0 && var->raydir.x > 0)
		var->texnum = 2;
	else if (var->side == 0 && var->raydir.x <= 0)
		var->texnum = 3;
	else if (var->side == 1 && var->raydir.y > 0)
		var->texnum = 1;
	else if (var->side == 1 && var->raydir.y <= 0)
		var->texnum = 0;
}

void	calculate_wall_hit_pos_to_texture(t_data *var, double *wallx)
{
	if (var->side == 0)
		*wallx = var->player_pos.y + var->perpwalldist * var->raydir.y;
	else
		*wallx = var->player_pos.x + var->perpwalldist * var->raydir.x;
	*wallx -= floor(*wallx);
	var->texx = (int)(*wallx * (double)var->textures[var->texnum]->width);
	if ((var->side == 0 && var->raydir.x <= 0) || (var->side == 1
			&& var->raydir.y >= 0))
		var->texx = var->textures[var->texnum]->width - var->texx - 1;
}

void	draw_texture_on_wall(t_data *var, int start,
		int end, int x)
{
	double		step;
	double		texpos;
	int			y;
	int			texy;
	uint32_t	color;

	step = 1.0 * var->textures[var->texnum]->height / var->lineheight;
	texpos = (start - HEIGHT / 2 + var->lineheight / 2) * step;
	y = start;
	while (y <= end)
	{
		texy = (int)texpos % var->textures[var->texnum]->height;
		if (texy < 0)
			texy += var->textures[var->texnum]->height;
		texpos += step;
		color = get_texture_pixel(var->textures[var->texnum], var->texx, texy);
		mlx_put_pixel(var->world, x, y, color);
		y++;
	}
}
