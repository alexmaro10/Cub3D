# -------------------------------
# CONFIGURACIÓN
# -------------------------------

NAME        := cub3D
CC          := cc
CFLAGS      := -Wall -Wextra -Werror -Iinclude -Ilibft -Iminilibx
OBJ_DIR     := obj
LIBFT_DIR   := libft
LIBFT       := $(LIBFT_DIR)/libft.a
MLX_DIR     := minilibx
MLX_LIB     := $(MLX_DIR)/libmlx.a

# Todas las fuentes
SRCS	:=	src/main.c \
			src/parser/arg_validation.c \
			src/parser/map_validation.c \
			src/parser/map_validation_2.c \
			src/parser/map_validation_3.c \
			src/utils/utils.c \
			src/utils/utils_2.c \
			src/utils/utils_3.c \
			src/utils/utils_mlx.c \
			src/init/init.c \
			src/init/init_ray.c \
			src/raycast/raycast.c \
			src/loop/loop.c \
			src/movement/movement.c \
			src/movement/rotation.c \
			src/raycast/textures.c \

# Generar nombres de objetos en obj/ manteniendo subcarpetas
OBJS        := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

# Flags para Linux
MLX_FLAGS   := -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz
LDFLAGS     := $(LIBFT) $(MLX_FLAGS)

# -------------------------------
# COLORES
# -------------------------------

GREEN := \033[1;32m
WHITE := \033[1;37m
RESET := \033[0m

# -------------------------------
# OBJETIVO POR DEFECTO
# -------------------------------

all: $(LIBFT) $(MLX_LIB) $(NAME)

# -------------------------------
# EJECUTABLE
# -------------------------------

$(NAME): $(OBJS)
	@printf "$(WHITE)Compilando $(NAME)...$(RESET)\t\t"
	@$(CC) $(OBJS) $(LDFLAGS) -o $@
	@printf "$(GREEN)[✔]$(RESET)\n\n"

# -------------------------------
# OBJETOS
# -------------------------------

# Compila cualquier .c en obj/ respetando subcarpetas
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# -------------------------------
# LIBFT
# -------------------------------

$(LIBFT):
	@printf "\n$(WHITE)Compilando libft...$(RESET)\t\t"
	@$(MAKE) -C $(LIBFT_DIR) --silent > /dev/null
	@printf "$(GREEN)[✔]$(RESET)\n"

# -------------------------------
# MINILIBX
# -------------------------------

$(MLX_LIB):
	@printf "$(WHITE)Compilando MiniLibX...$(RESET)\t\t"
	@$(MAKE) -C $(MLX_DIR) > /dev/null 2>&1
	@printf "$(GREEN)[✔]$(RESET)\n"

# -------------------------------
# LIMPIEZA
# -------------------------------

clean:
	@printf "\n$(WHITE)[🧹] Limpiando objetos...$(RESET)\t"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean --silent > /dev/null
	@$(MAKE) -C $(MLX_DIR) clean --silent > /dev/null
	@printf "$(GREEN)[✔]$(RESET)\n"

fclean: clean
	@printf "$(WHITE)[🧼] Eliminando ejecutable...$(RESET)\t"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean --silent > /dev/null
	@$(MAKE) -C $(MLX_DIR) clean --silent > /dev/null
	@printf "$(GREEN)[✔]$(RESET)\n\n"

re: fclean all

.PHONY: all clean fclean re