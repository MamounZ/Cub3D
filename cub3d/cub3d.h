/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thdaib <thdaib@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 12:26:43 by thdaib            #+#    #+#             */
/*   Updated: 2025/08/20 14:42:49 by thdaib           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/includes/libft.h"
#include "libft/includes/ft_printf.h"
#include "libft/includes/get_next_line.h"

typedef struct s_data {
    char    *no_tex;
    char    *so_tex;
    char    *we_tex;
    char    *ea_tex;
    char    *sprite_tex;
    int     floor_rgb[3];
    int     ceiling_rgb[3];
    char    **map;
    char    **map_copy;
    int		there_is_a_player;
    int     configs_done;
	int		player_x;
	int		player_y;
    int		map_done;
	int		map_rows;
} t_data;

char    *copy_identifire(char *str);
int     remove_id_from_list(t_list **head, char *value);
t_list  * create_configs_list(t_list **head);
int     is_empty_line(const char *line);
int     free_gnl(int fd,char *line ,t_list **head);
char    *process_line(char *line, t_list **head );
int     gnl_loop(int fd , t_data *game_data ,t_list *head);
int save_configs(char *line , t_data *game_data , char * id);
char *ft_strjoin1(char *left, char *buff);
void	*ft_calloc1(size_t nitem, size_t size);
int ft_split_len(char **split);
void	ft_free_split1(char **split);
int validate_map(int fd , t_data *game_data);
int validate_config(int fd ,t_data *game_data);
int validate_args(char *map_name,int argc);

int    get_map(int fd,t_data *data);
void validate_f_c(t_data *game_data, char *value, char *id);
int fill_rgb_values(t_data *game_data, char **rgb, char *id);
int check_edges(t_data *geme);
int flood_fill(char **map, int x, int y,int rows);
int	ft_countwords(char const *s, char c);
int store_rgb_value(t_data *game_data, char *id, int index, int value);
