NAME := minishell

LIB = libft/libft.a

CC := CC

CFLAGS := -Wall -Wextra -Werror -I includes -g #-fsanitize=address

INCS := includes/minishell.h
#need to add handling for compling from srcs subdirs
SRC_DIR := srcs

OBJ_DIR := objs

SRCS := main.c builtins.c executable_parse.c executer.c node_constructors.c parser.c \
		pathfinding.c redirection_parse.c  tokenize.c utilities.c \
		ft_hash.c reset.c expansions.c children.c tree_iter.c print_tree.c validate_syntax.c

SRCS := $(SRCS:%=$(SRC_DIR)/%)

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

READLINE := -lreadline -L ~/.brew/opt/readline/lib -I ~/.brew/opt/readline/include #from Lucas

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJS) $(LIB)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIB) $(READLINE)
	@echo "\033[0;92m* $(NAME) was created *\033[0m"

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): 
	$(MAKE) -C libft

clean:
	@rm -f $(OBJS)
	@rm -rf $(OBJ_DIR)
	$(MAKE) clean -C libft
	@echo "\033[0;91m* $(NAME) .o files and obj directory were deleted *\033[0m"

fclean: clean
	@rm -f $(NAME)
	$(MAKE) fclean -C libft
	@echo "\033[0;91m* $(NAME) was deleted *\033[0m"

re: fclean all
	
.PHONY: all clean fclean re
.SILENT:
