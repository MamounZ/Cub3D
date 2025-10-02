/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 12:26:33 by thdaib            #+#    #+#             */
/*   Updated: 2025/10/02 13:54:08 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"
#include <stdlib.h>

int check_paths(t_data *game_data)
{
	int fd;
	int i;

	i = 0;
	while (i < 4)
	{
		fd = open(game_data->tex_paths[i], O_RDONLY);
		if (fd == -1)
			return (-1);
		close(fd);
		i++;
	}
	// fd = open(game_data->ea_tex, O_RDONLY);
	// if (fd == -1)
	// 	return (-1);
	// close(fd);
	// fd = open(game_data->so_tex, O_RDONLY);
	// if (fd == -1)
	// 	return (-1);
	// close(fd);
	// fd = open(game_data->we_tex, O_RDONLY);
	// if (fd == -1)
	// 	return (-1);
	// close(fd);
	// fd = open(game_data->no_tex, O_RDONLY);
	// if (fd == -1)
	// 	return (-1);
	// close(fd);
	return (fd);
}

void free_map(char **map)
{
	int i;

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
void free_tex_paths(t_data *data)
{
	int i;

	i = 0;

	while (i < 4)
	{
		if (data->tex_paths[i])
			free(data->tex_paths[i]);
		i++;
	}
}
void free_data(t_data *data)
{
	if (!data)
		return;
	// if (data->no_tex)
	// 	free(data->no_tex);
	// if (data->so_tex)
	// 	free(data->so_tex);
	// if (data->we_tex)
	// 	free(data->we_tex);
	// if (data->ea_tex)
	// 	free(data->ea_tex);
	free_tex_paths(data);
	free_map(data->map);
	free_map(data->map_copy);
}

void print_list(t_list *head)
{
	t_list *temp;

	temp = head;

	while (temp)
	{
		ft_printf("%s->\n", temp->content);
		temp = temp->next;
	}
}
#include <stdio.h>

void print_data(const t_data *data)
{
	int i;

	// printf("NO texture: %s\n", data->no_tex ? data->no_tex : "(null)");
	// printf("SO texture: %s\n", data->so_tex ? data->so_tex : "(null)");
	// printf("WE texture: %s\n", data->we_tex ? data->we_tex : "(null)");
	// printf("EA texture: %s\n", data->ea_tex ? data->ea_tex : "(null)");

	printf("Ceiling RGB: %d, %d, %d\n",
		   data->ceiling_rgb[0], data->ceiling_rgb[1], data->ceiling_rgb[2]);
	printf("Floor RGB: %d, %d, %d\n",
		   data->floor_rgb[0], data->floor_rgb[1], data->floor_rgb[2]);

	if (data->map)
	{
		i = 0;
		while (data->map[i])
		{
			printf("%s\n", data->map[i]);
			i++;
		}
	}
	printf("----------------------------------map copy-----------------------------------------\n");
	if (data->map_copy)
	{
		i = 0;
		while (data->map_copy[i])
		{
			printf("%s\n", data->map_copy[i]);
			i++;
		}
	}

	// printf("Map (%dx%d):\n", data->map_width, data->map_height);
	// =
	// printf("Player position: (%d, %d)\n", data->player_x, data->player_y);
	// printf("Player direction: %c\n", data->player_dir);
	// printf("Configs done: %d\n", data->configs_done);
}

void free_and_exit(t_data *game_data, char *message, int ret)
{
	if (game_data)
		free_data(game_data);
	if (message)
		printf("Error\n%s\n", message);
	if (game_data->mlx)
		free_mlx_stuff(game_data);
	close(game_data->fd);
	exit (ret);
}
/*
int check_after_map(int fd)
{
	char *line;

	line = get_next_line(fd);
	while (line)
	{
		if (!line || line[0] != '\n')
		{
			printf("line after map: %s\n",line);
			return (free_gnl(fd,line,NULL));
		}
		free(line);
		line = get_next_line(fd);
	}

	return (0);
}
*/

int main(int argc, char **argv)
{
	t_data game_data;

	if (validate_args(argv[1], argc))
		return (1);
	ft_bzero(&game_data, sizeof(t_data));
	game_data.fd = open(argv[1], O_RDONLY);
	if (game_data.fd == -1)
		return (1);
	if (validate_config(game_data.fd, &game_data))
		free_and_exit(&game_data, "bad config", 1);
	if (check_paths(&game_data) == -1)
	{
		free_gnl(game_data.fd, NULL, NULL);
		free_and_exit(&game_data, "bad path", 1);
	}
	if (validate_map(game_data.fd, &game_data))
		free_and_exit(&game_data, "bad map", 1);
	print_data(&game_data);
	mlx_stuff(&game_data);
	free_and_exit(&game_data, NULL, 0);
}
