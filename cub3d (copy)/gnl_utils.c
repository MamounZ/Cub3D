
#include "./cub3d.h"

int is_empty_line(const char *line)
{
    int i = 0;
    while (line[i])
    {
        if (!ft_is_space(line[i]))
            return 0;
        i++;
    }
    return 1;
}

int free_gnl(int fd,char *line ,t_list **head)
{
	 if (head && *head)
		ft_lstclear(head, free);
	if(line)
		free(line);
	while ((line = get_next_line(fd)))
			free(line);
		return (1);
}
char *process_line(char *line, t_list **head )
{
    int i;
    char *value;

    i = 0;
    skip_spaces(line, &i);
    value = copy_identifire(line + i);
    if (!value || remove_id_from_list(head,value))
	{
		//print_list(*head);
		//free_gnl(-1,NULL,head,value);
		free (value);
        return (NULL);
	}
    return (value);
}

int gnl_loop(int fd , t_data *game_data ,t_list *head)
{
	char	*line;
	char	*id;

	while (!(game_data->configs_done == 6))
	{
		line = get_next_line(fd);
		if(!line)
			return(free_gnl(fd,NULL,&head));
		if (is_empty_line(line))
        {
            free(line);
            continue;
        }
		id = process_line(line, &head);
		if(*line && !id)
			return(free_gnl(fd,line,&head));
		game_data->configs_done++;
		if (save_configs(line , game_data , id) || game_data->floor_rgb[0] == -1 || game_data->ceiling_rgb[0] == -1)
			return(free_gnl(fd,line,&head));
		free (line);
	}
	return (!(game_data->configs_done == 6));
}
