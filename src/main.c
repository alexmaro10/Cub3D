/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almaldon <almaldon@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:09:36 by almaldon          #+#    #+#             */
/*   Updated: 2026/02/02 11:35:16 by almaldon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static void	init_map_data(t_map_data *map_data)
{
	map_data->no = NULL;
	map_data->so = NULL;
	map_data->we = NULL;
	map_data->ea = NULL;
	map_data->f = NULL;
	map_data->c = NULL;
	map_data->only_map = NULL;
}

static char	**extract_map(char **file, int map_start)
{
	int i;
	int lines;
	char **map;

	lines = 0;
	i = map_start;
	while (file[i])
	{
		lines++;
		i++;
	}
	map = malloc(sizeof(char *) * (lines + 1));
	if (!map)
		return NULL;
	i = 0;
	while (i < lines)
	{
		map[i] = ft_strdup_no_nl(file[map_start + i]);
		i++;
	}
	map[i] = NULL;
	return map;
}

static int	fill_structures(char **map, t_map_data *map_data)
{
	int	i;
	int	j;

	i = -1;
	if (extract_no(map, map_data) != 0 || extract_so(map, map_data) != 0 || extract_we(map, map_data) != 0 || extract_ea(map, map_data) != 0)
		return(1);
	map_data->c_color = rgb_to_int(map_data->c);
	map_data->f_color = rgb_to_int(map_data->f);
	while (map_data->only_map[++i])
	{
		j = -1;
		while (map_data->only_map[i][++j])
		{
			if (map_data->only_map[i][j] != '0' &&
				map_data->only_map[i][j] != '1' &&
				map_data->only_map[i][j] != ' ')
			{
				if (map_data->only_map[i][j] == 'N')
					map_data->player_data.angle = 3 * M_PI / 2; // arriba
				if (map_data->only_map[i][j] == 'S')
					map_data->player_data.angle = M_PI / 2; // arriba
				if (map_data->only_map[i][j] == 'W')
					map_data->player_data.angle = M_PI; // arriba
				if (map_data->only_map[i][j] == 'E')
					map_data->player_data.angle = 0; // arriba
				map_data->player_data.x = (float)j + 0.5;
				map_data->player_data.y = (float)i + 0.5;
			}
		}
	}
	map_data->player_data.move_speed = .1f; // ajustable
	map_data->player_data.rot_speed = .07f;  // radianes (~3° por frame)
	map_data->player_data.fov = 60.f;;
	show_struct(map_data);
	return (0);
}

void free_game(t_game *game)
{
	// Liberar mapa
	ft_free_map(game->map);

	// Liberar strings en map_data
	ft_free_struct(&game->map_data);

	// Liberar imágenes de texturas
	mlx_destroy_image(game->mlx.mlx, game->no.img);
	mlx_destroy_image(game->mlx.mlx, game->so.img);
	mlx_destroy_image(game->mlx.mlx, game->we.img);
	mlx_destroy_image(game->mlx.mlx, game->ea.img);

	// Liberar imagen principal
	mlx_destroy_image(game->mlx.mlx, game->mlx.img);

	// Cerrar ventana
	mlx_destroy_window(game->mlx.mlx, game->mlx.win);
}

int	main(int argc, char **argv)
{
	t_game	game;

	// 1️⃣ Parsing y validación
	init_map_data(&game.map_data);

	if (check_args(argv[1], argc) != 0)
		return (EXIT_FAILURE);

	game.map = file_to_charpp(argv[1]);
	if (!game.map)
		return (1);

	if (valid_map(game.map, &game.map_data) != 0)
	{
		ft_free_map(game.map);
		ft_free_struct(&game.map_data);
		return (EXIT_FAILURE);
	}

	game.map_data.only_map = extract_map(game.map, game.map_data.map_init_line);

	if (fill_structures(game.map, &game.map_data) != 0)
	{
		ft_free_map(game.map);
		ft_free_struct(&game.map_data);
		ft_printf("Calloc has failed\n");
		return (EXIT_FAILURE);
	}

	// 2️⃣ MiniLibX
	if (init_mlx(&game.mlx))
		return (EXIT_FAILURE);

	if (create_image(&game.mlx))
		return (EXIT_FAILURE);

	// 3️⃣ Cargar texturas
	if (load_textures(&game))
	{
		ft_printf("Failed to load textures\n");
		return (EXIT_FAILURE);
	}

	// 4️⃣ Render inicial
	render_scene(&game);

	// 5️⃣ Hooks de teclado y cierre
	mlx_hook(game.mlx.win, 2, 1L<<0, move_player, &game); // teclas
	mlx_hook(game.mlx.win, 17, 0, close_game, &game);     // cerrar ventana

	// 6️⃣ Loop principal
	mlx_loop(game.mlx.mlx);

	// 7️⃣ Liberar memoria
	free_game(&game);
	return (0);
}
