/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_iter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:50:12 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/18 19:23:22 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_child(t_executable_cmd *cmd, t_eggcarton *prog_info, int index)
{
	if (cmd->arg_count == 0)
	{
		prog_info->children[index]->command_present = FALSE;
		return ;
	}
	if (prog_info->pipe_count != 0)
	{
		setup_pipes(prog_info, index);
	}
	prog_info->children[index]->args = cmd->args;
	prog_info->children[index]->path = get_path(prog_info, cmd->args[0]);
}

void	setup_pipes(t_eggcarton *prog_info, int index)
{
	int	read_index;
	int	write_index;

	write_index = (index * 2) + 1;
	read_index = (index - 1) * 2;
	if (index == 0)
	{
		prog_info->children[index]->pipe_in = UNSET;
		prog_info->children[index]->pipe_out = prog_info->pipes[write_index];
	}
	else if (index == prog_info->cmd_count - 1)
	{
		prog_info->children[index]->pipe_in = prog_info->pipes[read_index];
		prog_info->children[index]->pipe_out = UNSET;
	}
	else
	{
		prog_info->children[index]->pipe_in = prog_info->pipes[read_index];
		prog_info->children[index]->pipe_out = prog_info->pipes[write_index];
	}
}

int	create_pipes(t_eggcarton *prog_info)
{
	char	*error;
	int		count;
	int		pipe_ret;

	prog_info->pipes = ft_calloc((prog_info->pipe_count * 2), sizeof(int));
	if (!prog_info->pipes)
	{	
		print_error("malloc failed");
		return (ERROR);
	}
	pipe_ret = 0;
	count = 0;
	while (count < prog_info->pipe_count && pipe_ret != -1)
	{
		pipe_ret = pipe(prog_info->pipes + (count * 2));
		count++;
	}
	if (pipe_ret == -1)
	{
		error = strerror(errno);
		print_error(error);
	}
	return (SUCCESS);
}

int	tree_iterator(t_cmd *cmd, t_eggcarton *prog_info, int *index)
{
	if (*index == prog_info->cmd_count)
		return (SUCCESS);
	if (cmd->type == PIPE_CMD)
	{
		tree_iterator((t_cmd *)((t_pipe *)cmd)->left, prog_info, index);
		(*index)++;
		tree_iterator((t_cmd *)((t_pipe *)cmd)->right, prog_info, index);
	}
	else if (cmd->type == REDIRECTION_CMD)
	{
		process_redirections((t_redirection *)cmd, prog_info, *index);
		while (cmd && cmd->type == REDIRECTION_CMD)
			cmd = ((t_redirection *)cmd)->cmd;
		if (cmd)
			tree_iterator(cmd, prog_info, index);
		else
		{
			prog_info->children[*index]->command_present = FALSE;
		}
	}
	else if (cmd->type == EXECUTABLE_CMD)
	{
		setup_child((t_executable_cmd *)cmd, prog_info, *index);
	}
	return (SUCCESS);
}
