#include "./cub3d.h"

int validate_args(char *map_name,int argc)
{

	if (argc !=2)
	{
		ft_printf("invalid args\n");
		return (1);
	}
	if (ft_strlen(map_name) <5)
	{
		ft_printf("unvaled map name\n");
		return (1);
	}
	if (ft_strncmp(map_name + ft_strlen(map_name) - 4, ".cub", 4) != 0)
	{
		ft_printf("unvaled map extension\n");
		return (1);
	}
	return (0);
}

int validate_config(int fd ,t_data *game_data)
{
    t_list	*head;

	head = NULL;
	
    head = create_configs_list(&head);
    if (!head)
	{
		return (1);
	}
	if (gnl_loop(fd ,game_data ,head))
	{
		return (1);
	}
   return (0);
}
int handle_spaces( char *temp , char *value , int *i , int *j)
{
	skip_spaces(temp ,i);
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

int id_value_loop(char *temp , char *value , int i , int j)
{
	while (temp[i])
	{
		if (temp[i] == '/')
		{
			value[j++] = temp[i++];
			skip_spaces(temp ,&i);
		}
		else if (ft_is_space(temp[i]))
		{
			if (handle_spaces(temp , value , &i , &j))
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
void skip_id_spaces(char *str,int *i ,char *id)
{
	skip_spaces(str ,i);
	*i += ft_strlen(id);
	skip_spaces(str ,i);
}
char *get_id (char *line ,char *id)
{
	int i;
	int j;
	char *temp;
	char *value;

	i = 0;
	skip_id_spaces(line ,&i ,id);
	temp = ft_strdup(line +i);
	if (!temp)
		return (NULL);
	if (ft_strcmp(id ,"F") == 0 || ft_strcmp(id ,"C") == 0)
		return (temp);
	value = malloc(sizeof(char) * (ft_strlen(temp) +1));
	if (!value)
	{
		free(temp);
		return (NULL);
	}
	i = 0;
	j = 0;
	if (id_value_loop(temp , value , i , j))
		return (NULL);
	free(temp);
	return (value);
}

int save_configs(char *line , t_data *game_data , char * id)
{
	char *value;

	value = get_id (line , id);
	if(!value)
	{
		free (id);
		return (1);
	}
	if (ft_strcmp(id , "NO") == 0)
		game_data->no_tex =value;
	else if (ft_strcmp(id , "SO") == 0)
		game_data->so_tex =value;
	else if (ft_strcmp(id , "EA") == 0)
		game_data->ea_tex =value;
	else if (ft_strcmp(id , "WE") == 0)
		game_data->we_tex =value;
	else if (ft_strcmp(id , "C") == 0 || ft_strcmp(id , "F") == 0)
		validate_f_c(game_data,value ,id);
	free(id);
	return (0);
}
int validate_map_content_loop (t_data *game_data ,char *valid_content , char *dirictions)
{
	int	i;
	int j;

	i = 0;
	 while (game_data->map[i])
	 {	
        j = 0;
        while (game_data->map[i][j])
        {
            if (!ft_strchr(valid_content,game_data->map[i][j]) && !ft_is_space (game_data->map[i][j]))
                return (1);
			if (ft_strchr(dirictions,game_data->map[i][j]))
			{
				game_data->there_is_a_player++;
				game_data->player_x = j;
				game_data->player_y = i;
			}
			if (game_data->there_is_a_player > 1)
				return (1);
            j++; 
        }
        i++;
    }
	game_data->map_rows = i;
	return (0);
}

int validate_map_content (t_data *game_data)
{

    char	*valid_content;
	char	*dirictions;

    valid_content = "01NEWS";
	dirictions = valid_content + 2;
	if (validate_map_content_loop (game_data ,valid_content ,dirictions))
		return (1);
	if (game_data->there_is_a_player != 1)
				return (1);
    return (0);
}


int validate_map(int fd , t_data *game_data)
{
	//int	i;
	int res;

   res = get_map(fd,game_data);
    if (res || !game_data->map || !game_data->map_copy)
	{
        return (1);
	}
    if (validate_map_content (game_data) || check_edges(game_data))
	{
        return (1);
	}
    return (flood_fill(game_data->map_copy, game_data->player_y, game_data->player_x,game_data->map_rows));
}
