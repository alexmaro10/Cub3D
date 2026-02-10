/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almaldon <almaldon@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 19:24:04 by almaldon          #+#    #+#             */
/*   Updated: 2026/02/10 11:36:56 by almaldon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int move_player(int keycode, void *param)
{
	t_game *game = (t_game *)param;
	float new_x = game->map_data.player_data.x;
	float new_y = game->map_data.player_data.y;
	float move = game->map_data.player_data.move_speed;
	float rot  = game->map_data.player_data.rot_speed;
	int rows = 0;
	int cols = 0;

	while (game->map_data.only_map[rows])
		rows++;
	if (keycode == KEY_W)
	{
		write(1, "A", 1);
		new_x += cos(game->map_data.player_data.angle) * move;
		new_y += sin(game->map_data.player_data.angle) * move;
	}
	else if (keycode == KEY_S)
	{
		new_x -= cos(game->map_data.player_data.angle) * move;
		new_y -= sin(game->map_data.player_data.angle) * move;
	}
	else if (keycode == KEY_A)
	{
		new_x += cos(game->map_data.player_data.angle - M_PI/2) * move;
		new_y += sin(game->map_data.player_data.angle - M_PI/2) * move;
	}
	else if (keycode == KEY_D)
	{
		new_x += cos(game->map_data.player_data.angle + M_PI/2) * move;
		new_y += sin(game->map_data.player_data.angle + M_PI/2) * move;
	}
	else if (keycode == KEY_LA)
		game->map_data.player_data.angle -= rot;
	else if (keycode == KEY_RA)
		game->map_data.player_data.angle += rot;
	else if (keycode == KEY_ESC)
	{
		mlx_destroy_window(game->mlx.mlx, game->mlx.win);
		ft_free_map(game->map);
		ft_free_struct(&game->map_data);
		exit(0);
	}
	else if (keycode == 65362)
		edit_fov('+', &game->map_data.player_data);
	else if (keycode == 65364)
		edit_fov('-', &game->map_data.player_data);
	if ((int)new_y >= 0 && (int)new_y < rows - 1)
	{
		cols = 0;
		while (game->map_data.only_map[(int)new_y][cols])
			cols++;
		if ((int)new_x >= 0 && (int)new_x < cols - 1 &&
			game->map_data.only_map[(int)new_y][(int)new_x] != '1')
		{
			printf("[%.2f][%.2f]\n", new_y, new_x);
			game->map_data.player_data.x = new_x;
			game->map_data.player_data.y = new_y;
		}
	}

	render_scene(game);
	return (0);
}
