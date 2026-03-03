/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almaldon <almaldon@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:49:02 by almaldon          #+#    #+#             */
/*   Updated: 2026/02/18 12:06:57 by almaldon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	end_with_map(char **map, t_map_data *map_data)
{
	int	m;
	int	i;

	i = -1;
	m = 0;
	while (map[++i])
	{
		if (map[i][0] && (map[i][0] == '1' || map[i][0] == ' ')
			&& ft_strlen(map[i]) > 1 && m == 0)
		{
			m = 1;
			map_data->map_init_line = i;
		}
		if (m == 1 && map[i][0] && (map[i][0] != '1' && map[i][0] != ' '))
			return (1);
	}
	return (0);
}

static int	validate_rgb(char **colours)
{
	int	i;
	int	n;

	i = -1;
	while (++i < 3)
	{
		n = ft_atoi(colours[i]);
		if (n < 0 || n > 255)
			return (1);
	}
	return (0);
}

static int	colours_validation(char **map, t_map_data *map_data)
{
	char	**coloursc;
	char	**coloursf;

	if (extract_colours_f(map, map_data) != 0
		|| extract_colours_c(map, map_data) != 0)
		return (1);
	coloursc = ft_split(map_data->c, ',');
	coloursf = ft_split(map_data->f, ',');
	if (!coloursc || !coloursc[0] || !coloursc[1] || !coloursc[2]
		|| !coloursf || !coloursf[0] || !coloursf[1] || !coloursf[2])
	{
		ft_free_map(map);
		ft_free_struct(map_data);
		return (1);
	}
	if (validate_rgb(coloursc) || validate_rgb(coloursf))
	{
		ft_free_map(coloursc);
		ft_free_map(coloursf);
		return (1);
	}
	ft_free_map(coloursc);
	ft_free_map(coloursf);
	return (0);
}

int	valid_map(char **map, t_map_data *map_data)
{
	int	ret;

	ret = exist_token(map, map_data);
	if (ret != 0)
	{
		if (ret == 1)
			return (ft_exit(1, "Can't found some tokens in map."));
		if (ret == 2)
			return (ft_exit(1, "Duplicate token found in map."));
	}
	if (end_with_map(map, map_data) != 0)
		return (ft_exit(1, "Map must be at the end of the file."));
	if (colours_validation(map, map_data) != 0)
		return (ft_exit(1, "The values of field F/C must be between 0 - 255."));
	if (surrounded_map(map, map_data) != 0)
		return (ft_exit(1, "The map is not surrounded with 1's."));
	if (not_void(map, map_data) != 0)
		return (ft_exit(1, "The map is not surrounded with 1'sa."));
	if (only_allowed_tokens_in_map(map, map_data) != 0)
		return (ft_exit(1, "Forbidden token founded on the map."));
	if (only_one_player(map, map_data) != 0)
		return (ft_exit(1, "The number of player's character must be 1."));
	return (0);
}
