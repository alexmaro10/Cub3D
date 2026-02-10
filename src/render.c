/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almaldon <almaldon@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 19:23:13 by almaldon          #+#    #+#             */
/*   Updated: 2026/02/10 11:38:09 by almaldon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color)
{
	char *dst;

	if (x < 0 || x >= mlx->win_w || y < 0 || y >=mlx->win_h)
		return ;
	dst = mlx->addr + (y * mlx->line_len + x * (mlx->bpp / 8));
	*(unsigned int *)dst = color;
}

static void	draw_background(t_mlx *mlx, int ceil_color, int floor_color)
{
	int x;
	int y;

	y = -1;
	while (++y < mlx->win_h)
	{
		x = -1;
		while (++x < mlx->win_w)
		{
			if (y < mlx->win_h / 2)
			{
				my_mlx_pixel_put(mlx, x, y, ceil_color);
			}
			else
				my_mlx_pixel_put(mlx, x, y, floor_color);
		}
	}
}

void draw_minimap(t_mlx *mlx, t_map_data *map, t_player_data *player)
{
	int i, j;
	int map_h = 0;

	while (map->only_map[map_h])
		map_h++;
	for (i = 0; i < map_h; i++)
	{
		int map_w = ft_strlen(map->only_map[i]);
		for (j = 0; j < map_w; j++)
		{
			int color;
			if (map->only_map[i][j] == '1')
				color = 0xFFFFFF;
			else
				color = 0x000000;
			for (int y = 0; y < MINIMAP_SCALE; y++)
			{
				for (int x = 0; x < MINIMAP_SCALE; x++)
				{
					my_mlx_pixel_put(mlx,
						MINIMAP_MARGIN + j * MINIMAP_SCALE + x,
						MINIMAP_MARGIN + i * MINIMAP_SCALE + y,
						color);
				}
			}
		}
	}
	int px = MINIMAP_MARGIN + (int)(player->x * MINIMAP_SCALE);
	int py = MINIMAP_MARGIN + (int)(player->y * MINIMAP_SCALE);
	for (int y = -2; y <= 2; y++)
		for (int x = -2; x <= 2; x++)
			my_mlx_pixel_put(mlx, px + x, py + y, 0xFF0000);
}

void render_scene(t_game *game)
{
	int col, y;
	float projection_plane;
	t_ray ray;
	t_texture *tex;
	float FOV = game->map_data.player_data.fov * (M_PI / 180.0f);
	projection_plane = (float)(game->mlx.win_w / 2) / tan(FOV / 2);
	draw_background(&game->mlx, game->map_data.c_color, game->map_data.f_color);
	for (col = 0; col < game->mlx.win_w; col++)
	{
		ray.ray_angle = game->map_data.player_data.angle - FOV / 2
						+ (col * FOV / game->mlx.win_w);
		ray.ray_dir_x = cos(ray.ray_angle);
		ray.ray_dir_y = sin(ray.ray_angle);
		cast_ray(&ray, &game->map_data, &game->map_data.player_data);
		int wall_height = (int)((float)projection_plane / ray.distance);
		int y_start = game->mlx.win_h / 2 - wall_height / 2;
		int y_end   = game->mlx.win_h / 2 + wall_height / 2;
		if (y_start < 0) y_start = 0;
		if (y_end >= game->mlx.win_h) y_end = game->mlx.win_h - 1;
		if (ray.hit_vertical)
			tex = (ray.ray_dir_x > 0) ? &game->we : &game->ea;
		else
			tex = (ray.ray_dir_y > 0) ? &game->no : &game->so;
		float hit_pos;
		if (ray.hit_vertical)
			hit_pos = ray.hit_y - floor(ray.hit_y);
		else
			hit_pos = ray.hit_x - floor(ray.hit_x);
		int tex_x = (int)(hit_pos * tex->width);
		if (tex_x < 0) tex_x = 0;
		if (tex_x >= tex->width) tex_x = tex->width - 1;
		for (y = y_start; y <= y_end; y++)
		{
			int tex_y = (int)(((float)(y - y_start) / (y_end - y_start)) * tex->height);
			int color = *((int *)(tex->addr + (tex_y * tex->line_len + tex_x * (tex->bpp / 8))));
			my_mlx_pixel_put(&game->mlx, col, y, color);
		}
	}
	mlx_put_image_to_window(game->mlx.mlx, game->mlx.win, game->mlx.img, 0, 0);
	draw_minimap(&game->mlx, &game->map_data, &game->map_data.player_data);
	mlx_put_image_to_window(game->mlx.mlx, game->mlx.win, game->mlx.img, 0, 0);
}

//DEBUG
void edit_fov(char c, t_player_data *player_data)
{
	if (c == '+')
		player_data->fov += 1.f;
	else
		player_data->fov -= 1.f;
	printf("%2.f\n", player_data->fov);
}