/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utilities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 15:52:52 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 15:58:13 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * print_error prints the passed error message. No blame is placed.
 */
int	print_error(char *error_msg)
{
	ft_putstr_fd("\033[1;31mEggShell🥚: ", 2);
	ft_putstr_fd(error_msg, 2);
	ft_putstr_fd("\x1B[0m\n", 2);
	return (FALSE);
}

/*
 * print_blame_error prints the passed error message, and blames the passed
 * string.
 */
int	print_blame_error(char *program, char *error_msg, char *to_blame)
{
	ft_putstr_fd("\033[1;31mEggShell🥚: ", 2);
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

/*
 * print_errno_error prints the error message associated with errno. No blame
 * is placed.
 */
int	print_errno_error(void)
{
	char	*error;

	error = strerror(errno);
	ft_putstr_fd("\033[1;31mEggShell🥚: ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\x1B[0m\n", 2);
	return (ERROR);
}

/*
 * print_errno_blame prints the error message associated with errno, and
 * blames the passed string.
 */
int	print_errno_blame(char *program, char *to_blame)
{
	char	*error;

	error = strerror(errno);
	ft_putstr_fd("\033[1;31mEggShell🥚: ", 2);
	if (program)
	{
		ft_putstr_fd(program, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(to_blame, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\x1B[0m\n", 2);
	return (ERROR);
}
