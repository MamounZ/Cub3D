/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thdaib <thdaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:37:31 by thdaib            #+#    #+#             */
/*   Updated: 2025/10/04 14:54:54 by thdaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void	choose_config(t_data *var, char *id, char *value)
{
	if (ft_strcmp(id, "NO") == 0)
		var->tex_paths[0] = value;
	else if (ft_strcmp(id, "SO") == 0)
		var->tex_paths[1] = value;
	else if (ft_strcmp(id, "EA") == 0)
		var->tex_paths[2] = value;
	else if (ft_strcmp(id, "WE") == 0)
		var->tex_paths[3] = value;
}

void	check_for_player(char *dirictions, t_data *var, int i, int j)
{
	if (ft_strchr(dirictions, var->map[i][j]))
	{
		var->there_is_a_player++;
		var->starting_dir = var->map[i][j];
		var->player_pos.x = j + 0.5;
		var->player_pos.y = i + 0.5;
	}
}

int	validate_map_content_loop(t_data *var, char *valid_content,
		char *dirictions)
{
	int	i;
	int	j;
	int	longest_row;

	longest_row = 0;
	i = 0;
	while (var->map[i])
	{
		j = 0;
		while (var->map[i][j])
		{
			if (!ft_strchr(valid_content, var->map[i][j]))
				return (1);
			check_for_player(dirictions, var, i, j);
			if (var->there_is_a_player > 1)
				return (1);
			j++;
		}
		if (j > longest_row)
			longest_row = j;
		i++;
	}
	var->map_rows = i;
	var->map_cols = longest_row;
	return (0);
}

int	validate_map_content(t_data *var)
{
	char	*valid_content;
	char	*dirictions;

	valid_content = " 01NEWS";
	dirictions = valid_content + 3;
	if (validate_map_content_loop(var, valid_content, dirictions))
		return (1);
	if (var->there_is_a_player != 1)
		return (1);
	return (0);
}

int	validate_map(int fd, t_data *var)
{
	int	res;

	res = get_map(fd, var);
	if (res || !var->map || !var->map_copy)
		return (1);
	if (validate_map_content(var))
		return (1);
	return (flood_fill(var->map_copy, var->player_pos.y, var->player_pos.x,
			var->map_rows));
}
