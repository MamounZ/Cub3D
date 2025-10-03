/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:10:56 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/03 20:46:55 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void mlx_stuff_init(t_data *game_data)
{
	int i;

	i = 0;
	game_data->mlx = mlx_init(WIDTH, HEIGHT, "Cub3D", true);
	if (!game_data->mlx)
		free_and_exit(game_data, "MLX initialization failed", 1);
	while (i < 4)
	{
		game_data->textures[i] = mlx_load_png(game_data->tex_paths[i]);
		if (!game_data->textures[i])
			free_and_exit(game_data, "Failed to load texture", 1);
		i++;
	}
	game_data->world = mlx_new_image(game_data->mlx, WIDTH, HEIGHT);
	if (!game_data->world)
		free_and_exit(game_data, "Failed to create world image", 1);
	game_data->m_map = mlx_new_image(game_data->mlx, 340, 180);
	if (!game_data->m_map)
		free_and_exit(game_data, "Failed to create minimap image", 1);
	if (mlx_image_to_window(game_data->mlx, game_data->world, 0, 0) == -1)
		free_and_exit(game_data, "Failed to load game image", 1);
	if (mlx_image_to_window(game_data->mlx, game_data->m_map, 0, 0) == -1)
		free_and_exit(game_data, "Failed to load minimap image", 1);
}

void mlx_stuff(t_data *game_data)
{
	mlx_stuff_init(game_data);
	game_data->needs_redraw = 1;
	game_data->mouse_sensitivity = 0.00050;
	game_data->mouse_initialized = 0;
	r_c_init(game_data);
	mlx_loop_hook(game_data->mlx, ray_caster, game_data);
	mlx_loop_hook(game_data->mlx, ft_hook, game_data);
	mlx_cursor_hook(game_data->mlx, &mouse_hook, game_data);
	mlx_set_cursor_mode(game_data->mlx, MLX_MOUSE_HIDDEN);
	mlx_loop(game_data->mlx);
}
