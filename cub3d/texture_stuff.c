/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_stuff.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:38:49 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/04 12:16:39 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

uint32_t get_texture_pixel(mlx_texture_t *texture, int x, int y)
{
	if (x < 0 || x >= (int)texture->width || y < 0 || y >= (int)texture->height)
		return (0xFF0000FF); // Red for error
	uint8_t *pixel = &texture->pixels[(y * texture->width + x) * 4];
	return (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
}
int rgb_to_int(int *rgb)
{
	return ((rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 0xFF);
}

void Determine_texture(t_data *var, int *texnum)
{
	if (var->side == 0 && var->raydir.x > 0)
		*texnum = 2; // EA
	else if (var->side == 0 && var->raydir.x <= 0)
		*texnum = 3; // WE
	else if (var->side == 1 && var->raydir.y > 0)
		*texnum = 1; // SO
	else if (var->side == 1 && var->raydir.y <= 0)
		*texnum = 0; // NO
}
void calculate_wall_hit_pos_to_texture(t_data *var, double *wallx, int *texx, int *texnum)
{
	if (var->side == 0)
		*wallx = var->player_pos.y + var->perpwalldist * var->raydir.y;
	else
		*wallx = var->player_pos.x + var->perpwalldist * var->raydir.x;
	*wallx -= floor(*wallx);
	*texx = (int)(*wallx * (double)var->textures[*texnum]->width);
	// merroring for west and north textures
	if ((var->side == 0 && var->raydir.x <= 0) || (var->side == 1 && var->raydir.y >= 0))
		*texx = var->textures[*texnum]->width - *texx - 1;
}
void draw_texture_on_wall(t_data *var, int *texnum, int *texx, int start, int end, int x)
{
	double step;
	double texpos;
	int y;
	int texy;
	uint32_t color;

	step = 1.0 * var->textures[*texnum]->height / var->lineHeight;
	texpos = (start - HEIGHT / 2 + var->lineHeight / 2) * step;
	y = start;
	while (y <= end)
	{
		texy = (int)texpos % var->textures[*texnum]->height;
		if (texy < 0)
			texy += var->textures[*texnum]->height;
		texpos += step;
		color = get_texture_pixel(var->textures[*texnum], *texx, texy);
		mlx_put_pixel(var->world, x, y, color);
		y++;
	}
}