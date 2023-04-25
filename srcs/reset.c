/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 12:49:17 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/25 10:01:42 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

void clean_str_array(char **array)
{
	int	index;

	index = 0;
	if (!array)
		return ;
	while(array[index])
	{
		free(array[index]);
		index++;
	}
	free(array);
}	

void close_pipes(int *pipes, int pipe_count)
{
	int index;
	
	index = 0;
	if (pipe_count == 0)
		return ;
	while(pipes[index] < pipe_count)
	{
		close(pipes[index]);
		index++;
	}
}

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
		// printf("REDIRECTION CMD CLEANED:\n");//
	}
	else if (cmd->type == EXECUTABLE_CMD)
	{
		clean_str_array(((t_executable_cmd *)cmd)->args);
		free((t_executable_cmd *)cmd);	
		// printf("EXECUTABLE CMD CLEANED\n");//
	}
}

void reset_program(t_eggcarton *prog_info, t_cmd **cmd)
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
	if (prog_info->pids)
		free(prog_info->pids);
	prog_info->cmd_count = 0;
	prog_info->pipe_count = 0;
	prog_info->pipes = NULL;
	prog_info->pids = NULL;
	prog_info->children = NULL;
}