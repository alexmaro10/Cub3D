/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almaldon <almaldon@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:49:02 by almaldon          #+#    #+#             */
/*   Updated: 2026/01/14 15:54:13 by almaldon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	is_valid(char **map, int i, int j, t_map_data *map_data)
{
	if (i >= ft_maplen(map) || i < map_data->map_init_line)
		return (1);
	if (j < 0 || j >= (int)ft_strlen(map[i]))
		return (1);
	return (0);
}

int	is_sorrounded(char **map, int i, int j,t_map_data *map_data)
{
	if (is_valid(map, (i - 1), (j - 1), map_data) == 0 &&
		map[i - 1][j - 1] != '1' && map[i - 1][j - 1] != ' ')
		return (1);
	if (is_valid(map, (i - 1), j, map_data) == 0 &&
		map[i - 1][j] != '1' && map[i - 1][j] != ' ')
		return (1);
	if (is_valid(map, (i - 1), (j + 1), map_data) == 0 &&
		map[i - 1][j + 1] != '1' && map[i - 1][j + 1] != ' ')
		return (1);
	if (is_valid(map, i, (j - 1), map_data) == 0 &&
		map[i][j - 1] != '1' && map[i][j - 1] != ' ')
		return (1);
	if (is_valid(map, i, (j + 1), map_data) == 0 &&
		map[i][j + 1] != '1' && map[i][j + 1] != ' ')
		return (1);
	if (is_valid(map, (i + 1), (j - 1), map_data) == 0 &&
		map[i + 1][j - 1] != '1' && map[i + 1][j - 1] != ' ')
		return (1);
	if (is_valid(map, (i + 1), j, map_data) == 0 &&
		map[i + 1][j] != '1' && map[i + 1][j] != ' ')
		return (1);
	if (is_valid(map, (i + 1), (j + 1), map_data) == 0 &&
		map[i + 1][j + 1] != '1' && map[i + 1][j + 1] != ' ')
		return (1);
	return (0);
}

int	only_allowed_tokens_in_map(char **map, t_map_data *map_data)
{
	int	i;
	int	j;

	i = map_data->map_init_line;
	while (map[i])
	{
		j = 0;
		while (map[i][j] && map[i][j] == ' ')
			j++;
		while (map[i][j])
		{
			if (map[i][j] == ' ')
			{
				if (is_sorrounded(map, i, j, map_data) != 0)
					return (1);
			}
			else if (map[i][j] != '0' && map[i][j] != '1' && map[i][j] != 'N'
					&& map[i][j] != 'S' && map[i][j] != 'W' && map[i][j] != 'E' && map[i][j] != '\n')
					return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	only_one_player(char **map, t_map_data *map_data)
{
	int	i;
	int	j;
	int np;
	
	np = 0;
	i = map_data->map_init_line - 1;
	while (map[++i])
	{
		j = -1;
		while (map[i][++j])
		{
			if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'W' || map[i][j] == 'E')
				np++;
		}
	}
	if (np != 1)
		return (1);
	return (0);
}
