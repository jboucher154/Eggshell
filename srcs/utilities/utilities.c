/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 13:20:14 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/17 15:42:32 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	move_pointer_past_ws(char **str_to_move)
{
	if (!str_to_move || !*str_to_move || **str_to_move == '\0')
		return ;
	while (**str_to_move && ft_strchr(WHITESPACE, **str_to_move))
		(*str_to_move)++;
}

void	move_pointer_backwards_ws(char **str_to_move, char *dont_go_past)
{
	if (!str_to_move || !*str_to_move || **str_to_move == '\0' || !dont_go_past \
		|| dont_go_past == *str_to_move)
		return ;
	while (*str_to_move > dont_go_past && ft_strchr(WHITESPACE, **str_to_move))
		(*str_to_move)--;
}

int	print_error(char *error_msg)
{
	ft_putstr_fd("\033[31mEggShell🥚: ", 2);
	ft_putstr_fd(error_msg, 2);
	ft_putstr_fd("\x1B[0m\n", 2);
	return (FALSE);
}

int	print_blame_error(char *program, char *error_msg, char *to_blame)
{
	ft_putstr_fd("\033[31mEggShell🥚: ", 2);
	if (program)
	{
		ft_putstr_fd(program, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(to_blame, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error_msg, 2);
	ft_putstr_fd("\x1B[0m\n", 2);
	return (FALSE);
}

int	print_errno_error(void)
{
	char	*error;

	error = strerror(errno);
	printf("%sEggShell🥚:%s%s\n", RED, error, KNRM);
	return (ERROR);
}

void	close_redirections(int fd_in, int fd_out)
{
	if (fd_in != UNSET)
		close(fd_in);
	if (fd_out != UNSET)
		close(fd_out);
}

//OFF = printing of ctrl characters off
void	echoctl_switch(int toggle)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	if (toggle == OFF)
	{
		term.c_lflag &= ~(ECHOCTL);
	}
	else
	{
		term.c_lflag |= ECHOCTL;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}


/*
 * fill_joined fills the allocated joined string with the passed strings. 
 * If the third string is NULL, it will only fill the joined string with
 * the first two strings.
*/
static	void	fill_joined(char *joined, char const *s1, char const *s2, \
char const *s3)
{
	int		i;
	char	*temp;

	i = 0;
	temp = joined;
	while (s1[i] != '\0')
	{
			*temp++ = s1[i];
			i++;
	}
	i = 0;
	while (s2[i] != '\0')
	{
			*temp++ = s2[i];
			i++;
	}
	i = 0;
	if (s3)
	{
		while (s3[i] != '\0')
		{
			*temp++ = s3[i];
			i++;
		}
	}
}

/*
 * ft_strjoin_three joins three strings together. It returns the joined string
 * on success and NULL on failure. If the third string is NULL, it will join
 * only the first two strings.
 */
char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3)
{
	char	*joined;
	size_t	len;

	if (!s1 || !s2)
		return (NULL);
	if (!s3)
		len = ft_strlen(s1) + ft_strlen(s2);
	else
		len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	joined = (char *) malloc(sizeof(char) * (len + 1));
	if (joined)
	{
		joined[len] = '\0';
		fill_joined(joined, s1, s2, s3);
	}
	return (joined);
}

void	ft_free_two(char *to_free1, char *to_free2)
{
	if (to_free1)
	{
		free(to_free1);
		to_free1 = NULL;
	}
	if (to_free2)
	{
		free(to_free2);
		to_free2 = NULL;
	}
}
