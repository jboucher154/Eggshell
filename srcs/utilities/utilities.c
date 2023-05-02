/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 13:20:14 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/02 15:47:30 by jebouche         ###   ########.fr       */
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

int	print_error(char *error_msg)
{
	ft_putstr_fd("\033[31mEggShell🥚: ", 2);
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
