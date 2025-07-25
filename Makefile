CC = cc

CFLAGS = -Werror -Wextra -Wall
LDFLAGS = -lreadline

INCLUDES = -Iincludes -I./lib/ft_printf/includes -I./lib/libft/includes

LIBPRINTF = lib/ft_printf/libftprintf.a

LIBFT = lib/libft/libft.a

SRC_DIR = srcs

OBJ_DIR = objs

SRCS = $(shell find $(SRC_DIR) -name '*.c')

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

DEPENDS = $(OBJS:.o=.d)

NAME = minishell

-include $(DEPENDS)

all: $(NAME) 

$(NAME): $(OBJS) $(LIBPRINTF) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBPRINTF) $(LIBFT) $(LDFLAGS) -o $(NAME)

VPATH = $(shell find $(SRC_DIR) -type d)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBPRINTF):
	$(MAKE) -C lib/ft_printf

$(LIBFT):
	$(MAKE) -C lib/libft

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C lib/ft_printf clean
	$(MAKE) -C lib/libft clean

.PHONY: fclean
fclean: clean
	rm -rf $(NAME)
	$(MAKE) -C lib/ft_printf fclean
	$(MAKE) -C lib/libft fclean

re: fclean all

.PHONY: all
