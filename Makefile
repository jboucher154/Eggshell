### COLOURS ###
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_END=\033[0m

### SET UP ###
CC = cc
CFLAGS = -Wall -Werror -Wall -I$I

RM = /bin/rm -f
RMDIR = /bin/rmdir -p

LIBFT = libft/libft.a

S = srcs
O = objs
I = includes

FILES = main.c builtins.c env_builtins.c executable_parse.c executer.c node_constructors.c parser.c \
		pathfinding.c redirection_parse.c tokenize.c utilities.c \
		ft_hash.c ft_hash_add_remove.c ft_rehash.c ft_hash_print.c reset.c expansions.c children.c tree_iter.c print_tree.c validate_syntax.c \
		signal.c initialize.c eggshell.c execute_file.c


HEADER = minishell.h libft.h ft_hash.h
HEADER := $(addprefix $I/,$(HEADER))

SRCS := $(foreach FILE,$(FILES),$(shell find $S -type f -name '$(FILE)')) # get a list of all source code files
O_DIRS = $(dir $(OBJS)) # convert source code paths to object code paths
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) # get a list of all object code directories

READLINE = -lreadline -L ~/.brew/opt/readline/lib -I ~/.brew/opt/readline/include

NAME = minishell

### RULES ###
all: $(NAME)

print: #print the list of source code files
	@echo $(SRCS)  

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(READLINE) -Llibft -lft -o $(NAME)
	@echo "$(COLOUR_GREEN) $(NAME) created$(COLOUR_END)"

$O:
	@mkdir -p $@ $(O_DIRS)

$O/%.o: $S/%.c $(HEADER) | $O
	@$(CC) $(CFLAGS) -c $< -o $@

### LIBFT

libft: $(LIBFT)

$(LIBFT):
	@$(MAKE) -C libft
	@echo "$(COLOUR_GREEN) $(LIBFT) created$(COLOUR_END)"

### CLEANING

clean:
	@cd libft && $(MAKE) clean
	@echo "$(COLOUR_RED) $(LIBFT) removed$(COLOUR_END)"
	@if [ -d $O ]; then $(RM) -rf $(O_DIRS) $O; fi

fclean : clean
	@cd libft && $(MAKE) fclean

re: fclean $(NAME)

.PHONY: all clean fclean re