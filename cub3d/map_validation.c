/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thdaib <thdaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:48:42 by thdaib            #+#    #+#             */
/*   Updated: 2025/10/04 13:49:50 by thdaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

char	*skip_initial_newlines(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line && is_empty_line(line))
	{
		free(line);
		line = get_next_line(fd);
	}
	return (line);
}

int	handle_newline_after_map(int fd, char *line, char *result, t_data *data)
{
	data->map_done = 1;
	while (line && line[0] == '\n')
	{
		free(line);
		line = get_next_line(fd);
		if (line && line[0] != '\n')
		{
			free_gnl(fd, line, NULL);
			free(result);
			return (1);
		}
	}
	return (0);
}

char	*joining_process(int fd, char **line, char *result)
{
	char	*joined;

	joined = ft_strjoin1(result, *line);
	if (!joined)
	{
		free(result);
		free_gnl(fd, *line, NULL);
		return (NULL);
	}
	free(*line);
	result = joined;
	return (result);
}

char	*collect_map_lines(int fd, t_data *data, char *result, char *line)
{
	while (line && !data->map_done)
	{
		if (line[0] == '\n')
		{
			if (handle_newline_after_map(fd, line, result, data))
				return (NULL);
		}
		else
		{
			result = joining_process(fd, &line, result);
			if (!result)
			{
				return (NULL);
			}
			line = get_next_line(fd);
			if (!line)
				free_gnl(fd, line, NULL);
		}
	}
	return (result);
}

int	get_map(int fd, t_data *data)
{
	char	*line;
	char	*result;

	result = NULL;
	line = skip_initial_newlines(fd);
	if (!line)
		return (1);
	result = collect_map_lines(fd, data, result, line);
	if (!result)
		return (1);
	data->map = ft_split(result, '\n');
	data->map_copy = ft_split(result, '\n');
	free(result);
	return (0);
}
