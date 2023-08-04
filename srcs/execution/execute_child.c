/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 15:06:28 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 13:24:38 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* exit_child is called by bail_onchild() to print the formatted error.
*/
void	exit_child(char *error_msg, char *arg, int exit_code)
{
	ft_putstr_fd("\033[1;31mEggShell🥚: ", STDERR_FILENO);
	ft_putstr_fd(error_msg, STDERR_FILENO);
	ft_putendl_fd(arg, STDERR_FILENO);
	ft_putstr_fd("\x1B[0m", STDERR_FILENO);
	exit(exit_code);
}

/* bail_on_child is called by the child process if the command is not found.
 * It will exit with an error code and print an error message to stderr.
 */
static void	bail_on_child(char *cmd)
{
	if (cmd && cmd[0] == '.')
	{
		if (access(cmd, F_OK) != 0)
			exit_child("no such file or directory: ", cmd, CMD_ERROR);
		else if (access(cmd, X_OK) != 0)
			exit_child("Permission denied: ", cmd, 126);
	}
	if (cmd == NULL)
		exit_child("command not found: ", " ", CMD_ERROR);
	else
		exit_child("command not found: ", cmd, CMD_ERROR);
}

static void	dup_pipes(t_eggcarton *prog_info, int index)
{
	if (prog_info->pipe_count > 0)
	{
		if (prog_info->children[index]->pipe_in != UNSET)
		{
			dup2(prog_info->children[index]->pipe_in, STDIN_FILENO);
		}
		if (prog_info->children[index]->pipe_out != UNSET)
		{
			dup2(prog_info->children[index]->pipe_out, STDOUT_FILENO);
		}
	}
	close_pipes(prog_info->pipes, prog_info->pipe_count);
}

/*
*  dup_redirections is called by the child process in the execution phase.
*  It will dup2 the appropriate file descriptors for redirections and close 
*  them after duping. If a file descriptor had an open error, it will write
*  a null byte to the file descriptor.
*/
static void	dup_redirections(t_eggcarton *prog_info, int index)
{
	if (prog_info->children[index]->redir_in != UNSET)
	{
		if (prog_info->children[index]->redir_in == OPEN_ERROR)
			write(STDIN_FILENO, "\0", 1);
		else
			dup2(prog_info->children[index]->redir_in, STDIN_FILENO);
	}
	if (prog_info->children[index]->redir_out != UNSET)
	{
		if (prog_info->children[index]->redir_in == OPEN_ERROR)
			write(STDIN_FILENO, "\0", 1);
		else
			dup2(prog_info->children[index]->redir_out, STDOUT_FILENO);
	}
	close_redirections(prog_info->children[index]->redir_in, \
	prog_info->children[index]->redir_out);
}

/* 
*  pipe_child is called by child processes in the execution phase. 
*  It will dup2 the appropriate file descriptors for pipes and redirectsion
*  and run the command. If the command is a builtin, it will run the builtin
*  and exit. If the command is not a builtin, it will execve the command.
*  If the command is not found or a fd had an open error, it will exit with 
*  an error code. 
*/
void	pipe_child(t_eggcarton *prog_info, int index)
{
	dup_pipes(prog_info, index);
	dup_redirections(prog_info, index);
	if (prog_info->children[index]->command_present == FALSE)
		exit(0);
	if (prog_info->children[index]->path == NULL)
		bail_on_child(prog_info->children[index]->args[0]);
	else if (prog_info->children[index]->redir_in == OPEN_ERROR || \
		prog_info->children[index]->redir_out == OPEN_ERROR)
		exit(1);
	if (prog_info->children[index]->path[0] == ':')
		run_builtins(prog_info->children[index], prog_info);
	else
		execve(prog_info->children[index]->path, \
		prog_info->children[index]->args, prog_info->array_env);
	print_errno_error();
	exit(EXIT_FAILURE);
}
