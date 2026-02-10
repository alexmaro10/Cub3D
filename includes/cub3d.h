/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almaldon <almaldon@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:01:02 by almaldon          #+#    #+#             */
/*   Updated: 2026/02/10 11:31:26 by almaldon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include <mlx.h>
# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <X11/keysym.h>
# include <X11/X.h>

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_LA 65361
# define KEY_RA 65363
# define KEY_ESC 65307
# define FOV_VALUE 60.f
#define MINIMAP_SCALE 10
#define MINIMAP_MARGIN 10

# define WIN_W 2048
# define WIN_H 1536

typedef struct s_texture
{
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
}	t_texture;

typedef struct s_player_data
{
	float	y;
	float	x;
	float	angle;
	float	move_speed;
	float	rot_speed;
	float	fov;
}	t_player_data;

typedef struct s_ray
{
	double	ray_angle;
	double	ray_dir_x;
	double	ray_dir_y;
	double	hit_x;
	double	hit_y;
	double	distance;
	int		hit_vertical;
	int		hit_horizontal;
}	t_ray;

typedef struct s_map_data
{
	int	map_init_line;
	int	no_line;
	int	so_line;
	int	we_line;
	int	ea_line;
	int	f_line;
	int	c_line;
	char **only_map;
	char *no;
	char *so;
	char *we;
	char *ea;
	char *f;
	char *c;
	int	f_color;
	int	c_color;
	
	t_player_data player_data;
}	t_map_data;

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		win_w;
	int		win_h;
}	t_mlx;

typedef struct s_game
{
	t_mlx		mlx;
	t_map_data	map_data;
	char		**map;

	t_texture	no;
	t_texture	so;
	t_texture	we;
	t_texture	ea;
}	t_game;

//ARG_VALIDATION.C
int		check_args(char *argv, int argc);

//MAP_VALIDATION.C
int		valid_map(char **map, t_map_data *map_data);
int		is_sorrounded(char **map, int i, int j,t_map_data *map_data);
int		only_allowed_tokens_in_map(char **map, t_map_data *map_data);
int		only_one_player(char **map, t_map_data *map_data);

//INIT_MLX.C
int		init_mlx(t_mlx *mlx);
int		create_image(t_mlx *mlx);
int		close_game(t_game *game);

//RENDER.C
void	render_scene(t_game *game);
void	my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color);
void	edit_fov(char c, t_player_data *player_data);

//MOVEMENT.C
int		move_player(int keycode, void *param);

//TEXTURES.C
int	load_textures(t_game *game);
void draw_wall_column(t_game *game, t_ray *ray, int x);
void cast_ray(t_ray *ray, t_map_data *map, t_player_data *player);
void	init_dda(t_ray *ray, t_player_data *player, int *map_x, int *map_y, double *delta_dist_x, double *delta_dist_y, double *side_dist_x, double *side_dist_y, int *step_x, int *step_y);
void	init_ray(t_ray *ray, t_player_data *player, int screen_x);

//UTILS.C
int		ft_exit(int n, char *msg);
int		ft_maplen(char **map);
int		extract_colours_f(char **map, t_map_data *map_data);
int		extract_colours_c(char **map, t_map_data *map_data);
int		extract_no(char **map, t_map_data *map_data);
int		extract_so(char **map, t_map_data *map_data);
int		extract_we(char **map, t_map_data *map_data);
int		extract_ea(char **map, t_map_data *map_data);
int		rgb_to_int(char *colors);
char	*ft_strdup_no_nl(const char *s);
char	**file_to_charpp(const char *path);
void	ft_free_map(char **map);
void	ft_free_struct(t_map_data *map_data);
void	show_struct(t_map_data *map_data);

#endif