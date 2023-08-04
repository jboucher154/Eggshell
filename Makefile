NAME := minishell

LIB = libft/libft.a

CC := CC

CFLAGS := -Wall -Wextra -Werror -I includes

DIR_DUP     = mkdir -p $(@D)

INCS := includes/minishell.h includes/ft_hash.h includes/libft.h

SRC_DIR := srcs

OBJ_DIR := objs

SRCS := main.c \
		eggshell.c \
		builtins/builtins.c builtins/env_builtins.c builtins/exit_builtin.c builtins/heredoc_builtin.c builtins/export.c\
		parser/executable_parse.c parser/node_constructors.c parser/parser.c parser/redirection_parse.c parser/tokenize.c parser/expansions.c parser/get_arg.c parser/expand_env_variable.c \
		execution/executer.c execution/pathfinding.c execution/tree_iter.c execution/execute_child.c execution/setup_redirections.c \
		hashtable/ft_hash.c hashtable/ft_hash_add_remove.c hashtable/ft_rehash.c hashtable/ft_hash_print.c hashtable/ft_hash_destroy.c hashtable/ft_export_hashtable.c \
		utilities/utilities.c utilities/reset.c utilities/error_utilities.c utilities/builtins_utilities.c utilities/utilities_environment.c \
		children/children.c \
		setup/validate_syntax.c setup/validate_redirection.c setup/signal.c setup/heredoc_signals.c setup/initialize.c \
		
SRCS := $(SRCS:%=$(SRC_DIR)/%)

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

READLINE := -lreadline -L ~/.brew/opt/readline/lib -I ~/.brew/opt/readline/include

all:  $(NAME)

$(NAME): $(OBJS) $(LIB)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIB) $(READLINE)
	@echo "\033[0;92m* $(NAME) was created *\033[0m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCS)
	$(DIR_DUP)
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