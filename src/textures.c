/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almaldon <almaldon@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 10:06:17 by almaldon          #+#    #+#             */
/*   Updated: 2026/01/15 11:10:45 by almaldon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_ray(t_ray *ray, t_player_data *player, int screen_x)
{
	double camera_x;

	// Valor entre -1 y 1, según la columna de la pantalla
	camera_x = 2.0 * screen_x / (double)WIN_W - 1.0;

	// Ángulo del rayo dentro del FOV
	ray->ray_angle = player->angle + atan(camera_x * tan(player->fov / 2));

	// Dirección del rayo (vector unitario)
	ray->ray_dir_x = cos(ray->ray_angle);
	ray->ray_dir_y = sin(ray->ray_angle);

	ray->hit_vertical = 0;
	ray->hit_horizontal = 0;
	ray->distance = 0;
	ray->hit_x = 0;
	ray->hit_y = 0;
}

void	init_dda(t_ray *ray, t_player_data *player, int *map_x, int *map_y, double *delta_dist_x, double *delta_dist_y, double *side_dist_x, double *side_dist_y, int *step_x, int *step_y)
{
	*map_x = (int)player->x;
	*map_y = (int)player->y;

	*delta_dist_x = fabs(1 / ray->ray_dir_x);
	*delta_dist_y = fabs(1 / ray->ray_dir_y);

	if (ray->ray_dir_x < 0)
	{
		*step_x = -1;
		*side_dist_x = (player->x - *map_x) * (*delta_dist_x);
	}
	else
	{
		*step_x = 1;
		*side_dist_x = (*map_x + 1.0 - player->x) * (*delta_dist_x);
	}

	if (ray->ray_dir_y < 0)
	{
		*step_y = -1;
		*side_dist_y = (player->y - *map_y) * (*delta_dist_y);
	}
	else
	{
		*step_y = 1;
		*side_dist_y = (*map_y + 1.0 - player->y) * (*delta_dist_y);
	}
}

void cast_ray(t_ray *ray, t_map_data *map, t_player_data *player)
{
	int map_x = (int)player->x;
	int map_y = (int)player->y;

	double delta_dist_x = fabs(1 / ray->ray_dir_x);
	double delta_dist_y = fabs(1 / ray->ray_dir_y);

	int step_x;
	int step_y;

	double side_dist_x;
	double side_dist_y;

	// calcular step y side_dist
	if (ray->ray_dir_x < 0)
	{
		step_x = -1;
		side_dist_x = (player->x - map_x) * delta_dist_x;
	}
	else
	{
		step_x = 1;
		side_dist_x = (map_x + 1.0 - player->x) * delta_dist_x;
	}

	if (ray->ray_dir_y < 0)
	{
		step_y = -1;
		side_dist_y = (player->y - map_y) * delta_dist_y;
	}
	else
	{
		step_y = 1;
		side_dist_y = (map_y + 1.0 - player->y) * delta_dist_y;
	}

	int hit = 0;

	while (!hit)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			ray->hit_vertical = 1;
			ray->hit_horizontal = 0;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
			ray->hit_horizontal = 1;
			ray->hit_vertical = 0;
		}

		// comprobar si choca con pared
		if (map->only_map[map_y][map_x] == '1')
			hit = 1;
	}

	// calcular distancia
	if (ray->hit_vertical)
		ray->distance = side_dist_x - delta_dist_x;
	else
		ray->distance = side_dist_y - delta_dist_y;

	// coordenadas exactas del impacto
	ray->hit_x = player->x + ray->ray_dir_x * ray->distance;
	ray->hit_y = player->y + ray->ray_dir_y * ray->distance;

	// corregir efecto ojo de pez
	ray->distance *= cos(ray->ray_angle - player->angle);
}

void draw_wall_column(t_game *game, t_ray *ray, int x)
{
	t_texture *tex;
	int tex_x;
	int tex_y;
	int y;
	int wall_height;
	double hit_pos;
	double step;

	// 1️⃣ Seleccionar textura según orientación del rayo
	if (ray->hit_vertical)
	{
		if (ray->ray_dir_x > 0)
			tex = &game->so; // pared Este
		else
			tex = &game->so; // pared Oeste
	}
	else
	{
		if (ray->ray_dir_y > 0)
			tex = &game->so; // pared Sur
		else
			tex = &game->so; // pared Norte
	}

	// 2️⃣ Calcular altura de la pared en pantalla
	wall_height = (int)(WIN_H / ray->distance);

	int draw_start = -wall_height / 2 + WIN_H / 2;
	int draw_end = wall_height / 2 + WIN_H / 2;
	if (draw_start < 0) draw_start = 0;
	if (draw_end >= WIN_H) draw_end = WIN_H - 1;

	// 3️⃣ Coordenada horizontal en la textura
	if (ray->hit_vertical)
		hit_pos = ray->hit_y - floor(ray->hit_y);
	else
		hit_pos = ray->hit_x - floor(ray->hit_x);
	tex_x = (int)(hit_pos * (double)tex->width);
	if ((ray->hit_vertical && ray->ray_dir_x < 0) ||
	    (ray->hit_horizontal && ray->ray_dir_y > 0))
		tex_x = tex->width - tex_x - 1;

	// 4️⃣ Paso de la textura por pixel vertical
	step = (double)tex->height / wall_height;
	tex_y = 0;

	for (y = draw_start; y < draw_end; y++)
	{
		tex_y = (int)((y - draw_start) * step);
		// leer color de textura
		char *pixel = tex->addr + (tex_y * tex->line_len + tex_x * (tex->bpp / 8));
		int color = *(unsigned int*)pixel;

		my_mlx_pixel_put(&game->mlx, x, y, color);
	}
}

int	load_textures(t_game *game)
{
	int	width;
	int	height;

	write(1, "Intentando cargar NO: ", 22);
	write(1, game->map_data.no, ft_strlen(game->map_data.no));
	write(1, "\n", 1);

	game->no.img = mlx_xpm_file_to_image(game->mlx.mlx,
				game->map_data.no, &width, &height);

	if (!game->no.img)
	{
		ft_printf("❌ Fallo al cargar textura N: %s\n", game->map_data.no);
		return (1);
	}
	write(1,"✅ N cargada\n", 13);
	game->no.addr = mlx_get_data_addr(game->no.img,
			&game->no.bpp, &game->no.line_len, &game->no.endian);
	game->no.width = width;
	game->no.height = height;

	game->so.img = mlx_xpm_file_to_image(game->mlx.mlx,
			game->map_data.so, &width, &height);
	write(1,"S\n", 2);
	if (!game->so.img)
		return (1);
	write(1,"S\n", 2);
	game->so.addr = mlx_get_data_addr(game->so.img,
			&game->so.bpp, &game->so.line_len, &game->so.endian);
	game->so.width = width;
	game->so.height = height;

	game->we.img = mlx_xpm_file_to_image(game->mlx.mlx,
			game->map_data.we, &width, &height);
	write(1,"W\n", 2);
	if (!game->we.img)
		return (1);
	write(1,"W\n", 2);
	game->we.addr = mlx_get_data_addr(game->we.img,
			&game->we.bpp, &game->we.line_len, &game->we.endian);
	game->we.width = width;
	game->we.height = height;

	game->ea.img = mlx_xpm_file_to_image(game->mlx.mlx,
			game->map_data.ea, &width, &height);
	write(1,"E\n", 2);
	if (!game->ea.img)
		return (1);
	write(1,"E\n", 2);
	game->ea.addr = mlx_get_data_addr(game->ea.img,
			&game->ea.bpp, &game->ea.line_len, &game->ea.endian);
	game->ea.width = width;
	game->ea.height = height;

	return (0);
}
