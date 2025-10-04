#include "./cub3d.h"

int validate_args(char *map_name, int argc)
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

int validate_config(int fd, t_data *var)
{
	t_list *head;

	head = NULL;

	head = create_configs_list(&head);
	if (!head)
		return (1);
	if (gnl_loop(fd, var, head))
		return (1);

	return (0);
}
int handle_spaces(char *temp, char *value, int *i, int *j)
{
	skip_spaces(temp, i);
	if (temp[*i] != '\0' && temp[*i] != '/')
	{
		if (temp[*i] != '\0')
			value[*(j)++] = temp[*(i)++];
		free(temp);
		free(value);
		return (1);
	}
	return (0);
}

int id_value_loop(char *temp, char *value, int i, int j)
{
	while (temp[i])
	{
		if (temp[i] == '/')
		{
			value[j++] = temp[i++];
			skip_spaces(temp, &i);
		}
		else if (ft_is_space(temp[i]))
		{
			if (handle_spaces(temp, value, &i, &j))
				return (1);
			if (temp[i] != '\0' && temp[i] != '/')
				value[j++] = temp[i++];
		}
		else
			value[j++] = temp[i++];
	}
	value[j] = '\0';
	return (0);
}
void skip_id_spaces(char *str, int *i, char *id)
{
	skip_spaces(str, i);
	*i += ft_strlen(id);
	skip_spaces(str, i);
}

char *get_id(char *line, char *id)
{
	int i;
	char *temp;
	char *value;

	i = 0;
	skip_id_spaces(line, &i, id);
	temp = ft_strdup(line + i);
	if (!temp)
		return (NULL);
	if (ft_strcmp(id, "F") == 0 || ft_strcmp(id, "C") == 0)
		return (temp);
	value = ft_strtrim(temp, " \t\n\v\f\r");
	free(temp);
	if (!value)
		return NULL;
	return (value);
}
// char *get_id(char *line, char *id)
// {
// 	int i;
// 	int j;
// 	char *temp;
// 	char *value;

// 	i = 0;
// 	skip_id_spaces(line, &i, id);
// 	temp = ft_strdup(line + i);
// 	if (!temp)
// 		return (NULL);
// 	if (ft_strcmp(id, "F") == 0 || ft_strcmp(id, "C") == 0)
// 		return (temp);
// 	value = malloc(sizeof(char) * (ft_strlen(temp) + 1));
// 	if (!value)
// 	{
// 		free(temp);
// 		return (NULL);
// 	}
// 	i = 0;
// 	j = 0;
// 	if (id_value_loop(temp, value, i, j))
// 		return (NULL);
// 	free(temp);
// 	return (value);
// }

int save_configs(char *line, t_data *var, char *id)
{
	char *value;

	value = get_id(line, id);
	if (!value)
	{
		free(id);
		return (1);
	}
	if (ft_strcmp(id, "NO") == 0)
		var->tex_paths[0] = value;
	else if (ft_strcmp(id, "SO") == 0)
		var->tex_paths[1] = value;
	else if (ft_strcmp(id, "EA") == 0)
		var->tex_paths[2] = value;
	else if (ft_strcmp(id, "WE") == 0)
		var->tex_paths[3] = value;
	else if (ft_strcmp(id, "C") == 0 || ft_strcmp(id, "F") == 0)
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
void check_for_player(char *dirictions, t_data *var, int i, int j)
{
	if (ft_strchr(dirictions, var->map[i][j]))
	{
		var->there_is_a_player++;
		var->starting_dir = var->map[i][j];
		var->player_pos.x = j + 0.5;
		var->player_pos.y = i + 0.5;
	}
}

int validate_map_content_loop(t_data *var, char *valid_content, char *dirictions)
{
	int i;
	int j;
	int longest_row;

	longest_row = 0;
	i = 0;
	while (var->map[i])
	{
		j = 0;
		while (var->map[i][j])
		{
			if (!ft_strchr(valid_content, var->map[i][j]) && (var->map[i][j] != ' '))
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

int validate_map_content(t_data *var)
{
	char *valid_content;
	char *dirictions;

	valid_content = "01NEWS";
	dirictions = valid_content + 2;
	if (validate_map_content_loop(var, valid_content, dirictions))
		return (1);
	if (var->there_is_a_player != 1)
		return (1);
	return (0);
}

int validate_map(int fd, t_data *var)
{
	// int	i;
	int res;

	res = get_map(fd, var);
	if (res || !var->map || !var->map_copy)
		return (1);
	if (validate_map_content(var))
		return (1);
	return (flood_fill(var->map_copy, var->player_pos.y, var->player_pos.x, var->map_rows));
}
