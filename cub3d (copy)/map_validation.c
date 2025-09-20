
#include "./cub3d.h"

int check_edges(t_data *game)
{
    int	i;
    int	j;
	size_t counter;
	int after_spaces;

	i = 0;
    while (game->map[i])
    {
		j = 0;
		counter = 0;
		skip_spaces(game->map[i],&j);
		after_spaces = j;
		if ( i == 0 || !game->map[i +1] )
		{
			while (game->map[i][j] == '1' || (ft_is_space(game->map[i][j])))
			{
				counter++;
				j++;
			}
			if (counter != ft_strlen (game->map[i] + after_spaces))
				return (1);
		}
		else
		{
			if (game->map[i][after_spaces] !='1' || (game->map[i][ft_strlen(game->map[i]) -1] != '1' && !ft_is_space( game->map[i][ft_strlen(game->map[i]) -1]) ))
				return (1);
		}
        i++;
    }
	return (0);
}

static char *skip_initial_newlines(int fd)
{
    char *line;

    line = get_next_line(fd);
    while (line && line[0] == '\n')
    {
        free(line);
        line = get_next_line(fd);
    }
    return (line);
}

static int handle_newline_after_map(int fd, char *line, char *result ,t_data *data)
{
	data->map_done = 1;
    while (line && is_empty_line(line))
    {
        free(line);
        line = get_next_line(fd);
        if (line && !is_empty_line(line))
        {
            free_gnl(fd, line, NULL);
            free(result);
            return (1);
        }
    }
    return (0);
}

static char *collect_map_lines(int fd, t_data *data, char *result, char *line)
{
    char *joined;

    while (line && !data->map_done)
    {
        if (line[0] == '\n')
        {
            if (handle_newline_after_map(fd, line, result,data))
                return (NULL);
        }
        else
        {
            joined = ft_strjoin1(result, line);
            if (!joined)
            {
                free(result);
                free_gnl(fd, line, NULL);
                return (NULL);
            }
            free(line);
            result = joined;
            line = get_next_line(fd);
        }
    }
    return (result);
}

int get_map(int fd, t_data *data)
{
    char *line;
    char *result;

    result = ft_strdup("");
    if (!result)
        return (1);
    line = skip_initial_newlines(fd);
    result = collect_map_lines(fd, data, result, line);
    if (!result)
        return (1);
    data->map = ft_split(result, '\n');
    data->map_copy = ft_split(result, '\n');
    free(result);
    return (0);
}


int flood_fill(char **map, int x, int y, int map_rows)
{
    if (x < 0 || y < 0 || x >= map_rows || y >= (int)ft_strlen(map[x]))
	{
		ft_printf("\nout of bounds x=%d y=%d\n",x,y);
        return (1);
	}
    //if (ft_is_space(map[x][y]))
       // return (1);
    if (map[x][y] == '1' || map[x][y] == 'T')
        return (0);
    map[x][y] = 'T';
    if (flood_fill(map, x + 1, y, map_rows))
        return (1);
    if (flood_fill(map, x - 1, y, map_rows))
        return (1);
    if (flood_fill(map, x, y + 1, map_rows))
        return (1);
    if (flood_fill(map, x, y - 1, map_rows))
        return (1);

    return (0);
}
