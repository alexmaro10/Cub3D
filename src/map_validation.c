/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almaldon <almaldon@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:49:02 by almaldon          #+#    #+#             */
/*   Updated: 2026/01/14 15:54:13 by almaldon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	exist_token(char **map, t_map_data *map_data)
{
	int	no_so_we_ea_f_c[6];
	int	i;

	no_so_we_ea_f_c[0] = 0;
	no_so_we_ea_f_c[1] = 0;
	no_so_we_ea_f_c[2] = 0;
	no_so_we_ea_f_c[3] = 0;
	no_so_we_ea_f_c[4] = 0;
	no_so_we_ea_f_c[5] = 0;

	i = -1;
	while (map[++i])
	{
		if (map[i][0] && map[i][1] && map[i][2] && map[i][0] == 'N' &&
			map[i][1] == 'O' && map[i][2] == ' ')
			{
				no_so_we_ea_f_c[0]++;
				map_data->no_line = i;
			}
		if (map[i][0] && map[i][1] && map[i][2] && map[i][0] == 'S' &&
			map[i][1] == 'O' && map[i][2] == ' ')
			{
				no_so_we_ea_f_c[1]++;
				map_data->so_line = i;
			}
		if (map[i][0] && map[i][1] && map[i][2] && map[i][0] == 'W' &&
			map[i][1] == 'E' && map[i][2] == ' ')
			{
				no_so_we_ea_f_c[2]++;
				map_data->we_line = i;
			}
		if (map[i][0] && map[i][1] && map[i][2] && map[i][0] == 'E' &&
			map[i][1] == 'A' && map[i][2] == ' ')
			{
				no_so_we_ea_f_c[3]++;
				map_data->ea_line = i;
			}
		if (map[i][0] && map[i][1] && map[i][0] == 'F' && map[i][1] == ' ')
			{
				no_so_we_ea_f_c[4]++;
				map_data->f_line = i;
			}
		if (map[i][0] && map[i][1] && map[i][0] == 'C' && map[i][1] == ' ')
			{
				no_so_we_ea_f_c[5]++;
				map_data->c_line = i;
			}
	}
	i = -1;
	while (++i < 6)
	{
		if (no_so_we_ea_f_c[i] == 0)
			return (1);
		if (no_so_we_ea_f_c[i] > 1)
			return (2);
	}
	return (0);
}

static int	end_with_map(char **map, t_map_data *map_data)
{
	int	m;
	int	i;

	i = -1;
	m = 0;
	while (map[++i])
	{
		if (map[i][0] && (map[i][0] == '1' || map[i][0] == ' ') &&
			ft_strlen(map[i]) > 1 && m == 0)
			{
				m = 1;
				map_data->map_init_line = i;
			}
		if (m == 1 && map[i][0] && (map[i][0] != '1' && map[i][0] != ' '))
			return (1);
	}
	return (0);
}

static int	colours_validation(char **map, t_map_data *map_data)
{
	char	**colours;
	int		n1;
	int		n2;
	int		n3;

	if (extract_colours_f(map, map_data) != 0 || extract_colours_c(map, map_data) != 0)
		return (1);
	colours = ft_split(map_data->f, ',');
	if (!colours || !colours[0] || !colours[1] || !colours[2])
	{
		ft_free_map(map);
		ft_free_struct(map_data);
		return (1);
	}
	n1 = ft_atoi(colours[0]);
	n2 = ft_atoi(colours[1]);
	n3 = ft_atoi(colours[2]);
	if ((n1 < 0 || n1 > 255) || (n2 < 0 || n2 > 255) || (n3 < 0 || n3 > 255))
		return (1);
	ft_free_map(colours);
	colours = ft_split(map_data->c, ',');
	if (!colours || !colours[0] || !colours[1] || !colours[2])
	{
		ft_free_map(map);
		ft_free_struct(map_data);
		return (1);
	}
	n1 = ft_atoi(colours[0]);
	n2 = ft_atoi(colours[1]);
	n3 = ft_atoi(colours[2]);
	if ((n1 < 0 || n1 > 255) || (n2 < 0 || n2 > 255) || (n3 < 0 || n3 > 255))
		return (1);
	ft_free_map(colours);
	return (0);
}

static int	surrounded_map(char **map, t_map_data *map_data)
{
	int	i;
	int	j;

	i = map_data->map_init_line ;
	j = 0;
	while (map[i][j])
	{
		while (map[i][j] && map[i][j] == ' ')
			j++;
		if (map[i][j] && map[i][j] != '1' && map[i][j] != ' ' && map[i][j] != '\n')
			return (1);
		j++;
	}
	while (map[i])
	{
		j = 0;
		while (map[i][j] && map[i][j] == ' ')
			j++;
		if (map[i][j] && map[i][j] != '1')
			return (1);
		j = ft_strlen(map[i]) - 2;
		if (map[i][j] && map[i][j] != '1')
			return (1);
		i++;
	}
	j = 0;
	i--;
	while (map[i][j])
	{
		while (map[i][j] && map[i][j] == ' ')
			j++;
		if (map[i][j] && map[i][j] != '1' && map[i][j] != ' ' && map[i][j] != '\n')
			return (1);
		j++;
	}
	return (0);
}

int	valid_map(char **map, t_map_data *map_data)
{

	int	ret;
	ret = exist_token(map, map_data);
	if ( ret != 0)
	{
		if (ret == 1)
			return (ft_exit(1, "Can't found some tokens in map."));
		if (ret == 2)
			return (ft_exit(1, "Duplicate token found in map."));
	}
	if (end_with_map(map, map_data) != 0)
		return (ft_exit(1, "Map must be at the end of the file."));
	if (colours_validation(map, map_data) != 0)
		return (ft_exit(1, "The values of field F and C must be between 0 and 255."));
	if (surrounded_map(map, map_data) != 0)
		return (ft_exit(1, "The map is not surrounded with 1's."));
	if (only_allowed_tokens_in_map(map, map_data) != 0)
		return (ft_exit(1, "Forbidden token founded on the map."));
	if (only_one_player(map, map_data) != 0)
		return (ft_exit(1, "The number of player's character must be 1."));
	return (0);
}
