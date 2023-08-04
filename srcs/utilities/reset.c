/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 12:49:17 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 15:53:28 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * clean_str_array frees the memory allocated for a string array and its
 * individual elements. Iterates over each element, freeing the memory.
 * Finally, it frees the array itself and assigns NULL to the array pointer.
 */
void	clean_str_array(char **array)
{
	int	index;

	index = 0;
	if (!array)
		return ;
	while (array[index])
	{
		free(array[index]);
		index++;
	}
	free(array);
	array = NULL;
}	

/*
 * close_pipes closes the file descriptors of pipes used for inter-process
 * communication. It takes an int* pipes array and an int pipe_count as
 * arguments. It iterates over the pipes array and closes each file descriptor.
 */
void	close_pipes(int *pipes, int pipe_count)
{
	int	index;

	index = 0;
	if (pipe_count == 0)
		return ;
	while (index < pipe_count * 2)
	{
		close(pipes[index]);
		index++;
	}
}

/*
 * close_redirections closes the file descriptors used for input and output
 * redirection. It takes two int fd_in and fd_out as arguments. If the
 * file descriptors are set to a valid value (not UNSET), it closes them.
 */
void	close_redirections(int fd_in, int fd_out)
{
	if (fd_in != UNSET)
		close(fd_in);
	if (fd_out != UNSET)
		close(fd_out);
}

/*
 * clean_tree frees the memory allocated for a command tree. It takes a t_cmd*
 * cmd as an argument and recursively traverses the tree to free each node
 * and its associated memory. Depending on the type of the command, it frees
 * different structures such as t_pipe, t_redirection, or t_exec.
 */
void	clean_tree(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->type == PIPE_CMD)
	{
		clean_tree((t_cmd *)(((t_pipe *)(cmd))->left));
		clean_tree((t_cmd *)(((t_pipe *)(cmd))->right));
		free((t_pipe *)cmd);
	}
	else if (cmd->type == REDIRECTION_CMD)
	{
		clean_tree((t_cmd *)(((t_redirection *)cmd)->cmd));
		free(((t_redirection *) cmd)->filename);
		free((t_redirection *) cmd);
	}
	else if (cmd->type == EXECUTABLE_CMD)
		free((t_exec *) cmd);
}

/*
 * reset_program resets the state of the program in the t_eggcarton struct
 * after executing a command. It cleans up the command tree using clean_tree(),
 * closes any open pipes using close_pipes(), and resets the relevant variables
 * in the t_eggcarton struct to their initial state.
 */
void	reset_program(t_eggcarton *prog_info, t_cmd **cmd)
{
	if (*cmd)
	{
		clean_tree(*cmd);
		*cmd = NULL;
	}
	if (prog_info->pipes)
	{
		close_pipes(prog_info->pipes, prog_info->pipe_count);
		free(prog_info->pipes);
	}
	prog_info->cmd_count = 0;
	prog_info->pipe_count = 0;
	prog_info->pipes = NULL;
	prog_info->children = NULL;
}
