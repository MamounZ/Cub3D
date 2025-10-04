/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:46:31 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/04 15:51:13 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

char	*get_id(char *line, char *id)
{
	int		i;
	char	*temp;
	char	*value;

	i = 0;
	skip_spaces(line, &i);
	i += ft_strlen(id);
	skip_spaces(line, &i);
	temp = ft_strdup(line + i);
	if (!temp)
		return (NULL);
	if (ft_strcmp(id, "F") == 0 || ft_strcmp(id, "C") == 0)
		return (temp);
	value = ft_strtrim(temp, " \t\n\v\f\r");
	free(temp);
	if (!value)
		return (NULL);
	return (value);
}

void	draw_ceiling_floor(t_data *var, int x, int start, int end)
{
	uint32_t	ceiling_color;
	uint32_t	floor_color;
	int			y;

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

void	draw_vertical_line(t_data *var, int x, int start, int end)
{
	double	wallx;

	if (x < 0 || x >= WIDTH)
		return ;
	if (start < 0)
		start = 0;
	if (end >= HEIGHT)
		end = HEIGHT - 1;
	draw_ceiling_floor(var, x, start, end);
	if (start <= end)
	{
		if (var->textures[0] && var->textures[1] && var->textures[2]
			&& var->textures[3])
		{
			determine_texture(var);
			calculate_wall_hit_pos_to_texture(var, &wallx);
			draw_texture_on_wall(var, start, end, x);
		}
	}
}

int	flood_fill(char **map, int x, int y, int rows)
{
	if (x < 0 || y < 0 || x >= rows || y >= (int)ft_strlen(map[x]))
		return (1);
	if (ft_is_space(map[x][y]))
		return (1);
	if (map[x][y] == '1' || map[x][y] == 'T')
		return (0);
	map[x][y] = 'T';
	if (flood_fill(map, x + 1, y, rows))
		return (1);
	if (flood_fill(map, x - 1, y, rows))
		return (1);
	if (flood_fill(map, x, y + 1, rows))
		return (1);
	if (flood_fill(map, x, y - 1, rows))
		return (1);
	if (flood_fill(map, x - 1, y - 1, rows))
		return (1);
	if (flood_fill(map, x - 1, y + 1, rows))
		return (1);
	if (flood_fill(map, x + 1, y - 1, rows))
		return (1);
	if (flood_fill(map, x + 1, y + 1, rows))
		return (1);
	return (0);
}
