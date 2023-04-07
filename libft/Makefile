# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/25 15:56:42 by jebouche          #+#    #+#              #
#    Updated: 2023/02/06 17:09:29 by jebouche         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCS =  ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
		ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_toupper.c ft_tolower.c \
		ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_memmove.c \
		ft_strlcpy.c ft_strnstr.c ft_strlcat.c ft_strdup.c ft_calloc.c ft_atoi.c \
		ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c \
		ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		ft_lstsize.c ft_lstnew.c ft_lstmap.c ft_lstlast.c ft_lstiter.c ft_lstdelone.c \
		ft_lstclear.c ft_lstadd_front.c ft_lstadd_back.c ft_printf.c conversion.c \
		flags.c handle_specifiers.c legend.c characters.c hexadecimal.c itoa_base.c \
		numbers.c padding.c percent.c unsigned.c num_utils.c get_next_line.c ft_gnl_join.c
		
OBJS = $(SRCS:.c=.o)

DEPS = ./includes/libft.h ./includes/ft_printf.h

CC = cc

CFLAGS = -Wall -Werror -Wextra -Iincludes

all: $(NAME)

$(NAME): $(OBJS)
		@ar rcs $(NAME) $(OBJS)
		
%.o: %.c $(DEPS)
		@$(CC) $(CFLAGS) -c $< -o $@

clean:
		@rm -f $(OBJS)

fclean: clean
		@rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all