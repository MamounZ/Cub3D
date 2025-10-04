/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thdaib <thdaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:44:25 by thdaib            #+#    #+#             */
/*   Updated: 2025/10/04 13:48:09 by thdaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void	free_mlx_stuff(t_data *var)
{
	int	i;

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

void	free_map(char **map)
{
	int	i;

	i = 0;
	if (map)
	{
		i = 0;
		while (map[i])
		{
			free(map[i]);
			i++;
		}
		free(map);
	}
}

void	free_tex_paths(t_data *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (data->tex_paths[i])
			free(data->tex_paths[i]);
		i++;
	}
}

void	free_data(t_data *data)
{
	if (!data)
		return ;
	free_tex_paths(data);
	free_map(data->map);
	free_map(data->map_copy);
}

void	free_and_exit(t_data *var, char *message, int ret)
{
	if (var)
		free_data(var);
	if (message)
		printf("Error\n%s\n", message);
	if (var->mlx)
		free_mlx_stuff(var);
	close(var->fd);
	exit(ret);
}
