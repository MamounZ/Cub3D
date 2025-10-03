/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_f_c.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 18:47:40 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/03 17:46:37 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "./cub3d.h"

int ft_is_number(char *str)
{
    int i;
    int is_number;
    char *trimed;
    int posetive;

    posetive = 0;
    is_number = 0;
    trimed = ft_strtrim(str, " \t\n\v\f\r");
    if (!trimed)
        return (0);
    if (trimed[0] == '\0')
    {
        free(trimed);
        return 0;
    }
    i = 0;
    while (trimed[posetive] == '+')
        posetive++;
    while (ft_isdigit(trimed[i + posetive]))
        i++;
    if (trimed[i + posetive] == '\0' && i <= 3)
        is_number = 1;
    free(trimed);
    return (is_number);
}

// int ft_is_number(char *str)
// {
// 	int i;
// 	int	is_number;

// 	i = 0;
// 	is_number = 0;
// 	char *trimed = ft_strtrim(str," \t\n\v\f\r");
// 	if (!trimed)
// 		return (0);
// 	if (trimed[0] == '\0')
//     {
//         free(trimed);
//         return 0;
//     }
// 	i = 0;
//     while (ft_isdigit(trimed[i]))
// 		i++;
// 	if (trimed[i] == '\0' && i <= 3)
// 		is_number = 1;
//     free(trimed);
//     return (is_number);

// }

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
	int	i;
	int	value;
	int	split_len;

	split_len = ft_split_len(rgb);
	i = 0;
	while (rgb[i] && split_len == 3 )
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

int validate_f_c(t_data *game_data, char *value, char *id)
{
	char	**rgb;
	int		count;
	int		i;
	int		commas_count;

	i = 0;
	commas_count = 0;
	while (value[i])
	{
		if (value[i] == ',')
			commas_count++;
		if(commas_count > 2)
			return (1);
		i++;
	}
    rgb = ft_split(value, ',');
    if (!rgb)
        return (1);
    count = fill_rgb_values(game_data, rgb, id);
    if (count != 3)
        game_data->ceiling_rgb[0] = -1;
		free (value);
    ft_free_split1(rgb);
    return (0);
}
