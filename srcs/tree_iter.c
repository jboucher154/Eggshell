/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_iter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:50:12 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/27 15:49:06 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	setup_child(t_executable_cmd *cmd, t_eggcarton *prog_info, int index)
{
	if (cmd->arg_count == 0)
	{
		prog_info->children[index]->command_present = FALSE;
		printf("assigned FALSE\n");//
		return ;
	}
	if (prog_info->pipe_count != 0)
	{
		setup_pipes(prog_info, index);
	}
	prog_info->children[index]->args = cmd->args;
	prog_info->children[index]->path = get_path(prog_info, cmd->args[0]);
}

void	setup_redirection(t_redirection *redirection, t_eggcarton *prog_info, int index)
{
	int		fd;
	char	*error;
	
	fd = OPEN_ERROR;
	if (redirection->token_id == REDIRECT_IN)
	{
		fd = open(redirection->filename, O_RDONLY);
		close(prog_info->children[index]->redir_in);
		prog_info->children[index]->redir_in = fd;
	}
	else if (redirection->token_id == REDIRECT_OUT)
	{
		fd = open(redirection->filename, O_WRONLY|O_CREAT|O_TRUNC, 0644);
		close(prog_info->children[index]->redir_out);
		prog_info->children[index]->redir_out = fd;
	}
	else if (redirection->token_id == REDIRECT_OUT_APPEND)
	{
		fd = open(redirection->filename, O_WRONLY|O_CREAT, 0644);
		close(prog_info->children[index]->redir_out);
		prog_info->children[index]->redir_out = fd;
	}
	if (fd == OPEN_ERROR) 
	{
		error = strerror(errno);
		printf("%sEggShell🥚:%s%s\n", RED, error, KNRM);
	}
}

void	setup_pipes(t_eggcarton *prog_info, int index)
{
	int	read_index; 
	int	write_index; 
	
	write_index = (index * 2) + 1;
	read_index = (index - 1) * 2;
	// printf("READ INDEX: %i   WRITE INDEX: %i \n", read_index, write_index);
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
	char *error;
	int	count;
	int	pipe_ret;

	prog_info->pipes = malloc(sizeof (int) * (prog_info->pipe_count * 2));
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

void	process_redirections(t_redirection *redir, t_eggcarton *prog_info, int index)
{
	if (redir && (redir->cmd == NULL|| redir->cmd->type == EXECUTABLE_CMD)) //added check for null next
	{
		setup_redirection(redir, prog_info, index);
		return ;
	}
	else if (redir && redir->cmd->type == REDIRECTION_CMD)
	{
		process_redirections(((t_redirection *)redir->cmd), prog_info, index);
		setup_redirection(redir, prog_info, index);
	}
}

int	tree_iterator(t_cmd *cmd, t_eggcarton *prog_info, int *index)//index for all the ararys
{
	if (*index == prog_info->cmd_count)
		return (SUCCESS);
	if (cmd->type == PIPE_CMD)
	{
		tree_iterator((t_cmd *)((t_pipe *)cmd)->left, prog_info, index);
		(*index)++;
		tree_iterator((t_cmd *)((t_pipe *)cmd)->right, prog_info, index);
		//return (tree_iterator(left) == SUCCESS && tree_iterator(right) == SUCCESS)?
	}
	else if (cmd->type == REDIRECTION_CMD)
	{
		// setup_redirection((t_redirection *)cmd, prog_info, *index);
		process_redirections((t_redirection *)cmd, prog_info, *index);
		while (cmd && cmd->type == REDIRECTION_CMD) //added check for null
			cmd = ((t_redirection *)cmd)->cmd;
		if (cmd)
			tree_iterator(cmd, prog_info, index);
		else
		{
			prog_info->children[*index]->command_present = FALSE;
			printf("assigned FALSE\n");//
		}
	}
	else if (cmd->type == EXECUTABLE_CMD)
	{
		setup_child((t_executable_cmd *)cmd, prog_info, *index);
	}
	return (SUCCESS);
}