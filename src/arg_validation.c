/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almaldon <almaldon@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:41:07 by almaldon          #+#    #+#             */
/*   Updated: 2026/01/14 15:54:13 by almaldon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int check_extension(char *name)
{
	size_t	i = ft_strlen(name);

	if (name[i - 1] != 'b' || name[i - 2] != 'u' ||
		name[i - 3] != 'c' ||name[i - 4] != '.')
		return (1);
	return (0);
}

int	check_args(char *argv, int argc)
{
	if (argc != 2)
		return (ft_exit(1, "Invalid number of arguments"));
	if (check_extension(argv) != 0)
		return (ft_exit(1, "Invalid extension file"));
	return (0);
}
