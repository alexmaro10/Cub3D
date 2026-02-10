/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almaldon <almaldon@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 19:33:55 by almaldon          #+#    #+#             */
/*   Updated: 2026/01/14 19:35:27 by almaldon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void parsing(t_game *game)
{
	parsing_map_data(game);
	parsing_player_data(game);
}