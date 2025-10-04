/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 12:26:33 by thdaib            #+#    #+#             */
/*   Updated: 2025/10/04 16:06:43 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"
#include <stdlib.h>

int	save_configs(char *line, t_data *var, char *id)
{
	char	*value;

	value = get_id(line, id);
	if (!value)
	{
		free(id);
		return (1);
	}
	choose_config(var, id, value);
	if (ft_strcmp(id, "C") == 0 || ft_strcmp(id, "F") == 0)
	{
		if (validate_f_c(var, value, id) == 1)
		{
			free(id);
			free(value);
			return (1);
		}
	}
	free(id);
	return (0);
}

int	validate_args(char *map_name, int argc)
{
	if (argc != 2)
	{
		ft_printf("invalid args\n");
		return (1);
	}
	if (ft_strlen(map_name) < 5)
	{
		ft_printf("invalid map name\n");
		return (1);
	}
	if (ft_strncmp(map_name + ft_strlen(map_name) - 4, ".cub", 4) != 0)
	{
		ft_printf("invalid map extension\n");
		return (1);
	}
	return (0);
}

int	validate_config(int fd, t_data *var)
{
	t_list	*head;

	head = NULL;
	head = create_configs_list(&head);
	if (!head)
		return (1);
	if (gnl_loop(fd, var, head))
		return (1);
	return (0);
}

int	check_paths(t_data *var)
{
	int	fd;
	int	i;

	i = 0;
	while (i < 4)
	{
		fd = open(var->tex_paths[i], O_RDONLY);
		if (fd == -1)
			return (-1);
		close(fd);
		i++;
	}
	return (fd);
}

int	main(int argc, char **argv)
{
	t_data	var;

	if (validate_args(argv[1], argc))
		return (1);
	ft_bzero(&var, sizeof(t_data));
	var.fd = open(argv[1], O_RDONLY);
	if (var.fd == -1)
	{
		printf("Error\ncannot open file\n");
		return (1);
	}
	if (validate_config(var.fd, &var))
		free_and_exit(&var, "bad config", 1);
	if (check_paths(&var) == -1)
	{
		free_gnl(var.fd, NULL, NULL);
		free_and_exit(&var, "bad path", 1);
	}
	if (validate_map(var.fd, &var))
		free_and_exit(&var, "bad map", 1);
	mlx_stuff(&var);
	free_and_exit(&var, NULL, 0);
}
