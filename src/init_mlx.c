/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almaldon <almaldon@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:04:09 by almaldon          #+#    #+#             */
/*   Updated: 2026/02/10 11:32:01 by almaldon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	init_mlx(t_mlx *mlx)
{
	mlx->win_w = WIN_W;
	mlx->win_h = WIN_H;

	mlx->mlx = mlx_init();
	if (!mlx->mlx)
		return (1);
	mlx->win = mlx_new_window(mlx->mlx, WIN_W, WIN_H, "cub3d");
	if (!mlx->win)
		return (1);
	return (0);
}

int create_image(t_mlx *mlx)
{
	mlx->img = mlx_new_image(mlx->mlx, mlx->win_w, mlx->win_h);
	if (!mlx->img)
		return (1);
	mlx->addr = mlx_get_data_addr(mlx->img, &mlx->bpp, &mlx->line_len, &mlx->endian);
	return (0);
}

int	close_game(t_game *game)
{
	if (game->map)
		ft_free_map(game->map);
	ft_free_struct(&game->map_data);
	if (game->mlx.img)
		mlx_destroy_image(game->mlx.mlx, game->mlx.img);
	if (game->mlx.win)
		mlx_destroy_window(game->mlx.mlx, game->mlx.win);

	exit(0);
	return (0);
}
