/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almaldon <almaldon@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:42:18 by almaldon          #+#    #+#             */
/*   Updated: 2026/01/14 15:54:13 by almaldon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	ft_maplen(char **map)
{
	int	i;

	i = -1;
	while (map[++i])
		;
	return (i);
}

static char	**add_line(char **arr, char *line, int count)
{
	char **new;
	int i;

	new = malloc(sizeof(char *) * (count + 2));
	if (!new)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new[i] = arr[i];
		i++;
	}
	new[count] = line;
	new[count + 1] = NULL;
	free(arr);
	return (new);
}

char	**file_to_charpp(const char *path)
{
	int	fd;
	char	*line;
	char	**result;
	int		count;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);

	result = NULL;
	count = 0;
	line = get_next_line(fd);
	while (line)
	{
		result = add_line(result, line, count++);
		if (!result)
			return (NULL);
		line = get_next_line(fd);
	}
	close(fd);
	return (result);
}

int	ft_exit(int n, char *msg)
{
	ft_printf("ERROR:\n%s\n", msg);
	return (n);
}

void	ft_free_map(char **map)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}
