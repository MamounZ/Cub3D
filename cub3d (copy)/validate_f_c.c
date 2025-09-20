
#include "./cub3d.h"

int ft_is_number(char *str)
{
    size_t i;
	size_t spaces_count;

    i = 0;
	while (ft_is_space (str[i]))
		i++;
	spaces_count = i;
    while (ft_isdigit(str[i]))
		i++;
    while (ft_is_space (str[i]))
	{
		i++;
		spaces_count++;
	}
	//ft_printf ("str = %s\n",str); ft_printf ("i = %d\n",i); ft_printf ("len = %d\n",ft_strlen(str));
    return (i > 0 && i == ft_strlen(str) && spaces_count != i);
}

int store_rgb_value(t_data *game_data, char *id, int index, int value)
{
    if (value < 0 || value > 255)
        return (0);
    if (ft_strcmp(id, "C") == 0)
        game_data->ceiling_rgb[index] = value;
    else
        game_data->floor_rgb[index] = value;
    return (1);
}

int fill_rgb_values(t_data *game_data, char **rgb, char *id)
{
    int i;
    int value;
	int	split_len;

	split_len = ft_split_len(rgb);
    i = 0;
    while (rgb[i] && split_len== 3 )
    {
		if (ft_is_number(rgb[i]))
			value = ft_atoi(rgb[i]);
		else
			value = -1;
        if (!store_rgb_value(game_data, id, i, value))
            return (0);
        i++;
    }
    return (i);
}

void validate_f_c(t_data *game_data, char *value, char *id)
{
    char **rgb;
    int count;

    rgb = ft_split(value, ',');
    if (!rgb)
        return;
    count = fill_rgb_values(game_data, rgb, id);
    if (count != 3)
        game_data->ceiling_rgb[0] = -1;
		free (value);
    ft_free_split1(rgb);
}
