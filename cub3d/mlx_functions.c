/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:10:56 by mazaid            #+#    #+#             */
/*   Updated: 2025/10/04 12:16:39 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void mlx_stuff_init(t_data *var)
{
	int i;

	i = 0;
	var->mlx = mlx_init(WIDTH, HEIGHT, "Cub3D", true);
	if (!var->mlx)
		free_and_exit(var, "MLX initialization failed", 1);
	while (i < 4)
	{
		var->textures[i] = mlx_load_png(var->tex_paths[i]);
		if (!var->textures[i])
			free_and_exit(var, "Failed to load texture", 1);
		i++;
	}
	var->world = mlx_new_image(var->mlx, WIDTH, HEIGHT);
	if (!var->world)
		free_and_exit(var, "Failed to create world image", 1);
	var->m_map = mlx_new_image(var->mlx, 340, 180);
	if (!var->m_map)
		free_and_exit(var, "Failed to create minimap image", 1);
	if (mlx_image_to_window(var->mlx, var->world, 0, 0) == -1)
		free_and_exit(var, "Failed to load game image", 1);
	if (mlx_image_to_window(var->mlx, var->m_map, 0, 0) == -1)
		free_and_exit(var, "Failed to load minimap image", 1);
}

void mlx_stuff(t_data *var)
{
	mlx_stuff_init(var);
	var->needs_redraw = 1;
	var->mouse_sensitivity = 0.00050;
	var->mouse_initialized = 0;
	r_c_init(var);
	mlx_loop_hook(var->mlx, ray_caster, var);
	mlx_loop_hook(var->mlx, ft_hook, var);
	mlx_cursor_hook(var->mlx, &mouse_hook, var);
	mlx_set_cursor_mode(var->mlx, MLX_MOUSE_HIDDEN);
	mlx_loop(var->mlx);
}
