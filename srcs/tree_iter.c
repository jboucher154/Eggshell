/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_iter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:50:12 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/25 19:08:42 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

void	setup_child(t_executable_cmd *cmd, t_eggcarton *prog_info, int index)
{
	if (prog_info->pipe_count != 0)
	{
		setup_pipes(prog_info, index);
	}
	prog_info->children[index]->args = cmd->args;
	if (check_for_expansions(prog_info, prog_info->children[index]->args, 0) == ERROR)
		printf("%sEggShell🥚: expansion error%s\n", RED, KNRM); //continue??
	prog_info->children[index]->path = get_path(prog_info, cmd->args[0]);
}

void	setup_redirection(t_redirection *redirection, t_eggcarton *prog_info, int index)
{
	int	fd;
	char *error;
	check_for_expansions(prog_info, &(redirection->filename), 1);
	fd = -1;
	if (redirection->token_id == REDIRECT_IN)
	{
		fd = open(redirection->filename, O_RDONLY);
		close(prog_info->children[index]->redir_in);
		prog_info->children[index]->redir_in = fd;
	}
	else if (redirection->token_id == REDIRECT_OUT)
	{
		fd = open(redirection->filename, O_WRONLY|O_CREAT|O_TRUNC);
		close(prog_info->children[index]->redir_out);
		prog_info->children[index]->redir_out = fd;
	}
	else if (redirection->token_id == REDIRECT_OUT_APPEND)
	{
		fd = open(redirection->filename, O_WRONLY|O_CREAT);
		close(prog_info->children[index]->redir_out);
		prog_info->children[index]->redir_out = fd;
	}
	if (fd == -1) 
	{
		error = strerror(errno);
		printf("%sEggShell🥚:%s%s\n", RED, error, KNRM);
	}
}

void	setup_pipes(t_eggcarton *prog_info, int index)
{
	int	read_index;  // in  == read
	int	write_index; //out  == write
	
	write_index = (index * 2) + 1;
	read_index = (index - 1) * 2;
	printf("READ INDEX: %i   WRITE INDEX: %i \n", read_index, write_index);
	if (index == 0)
	{
		prog_info->children[index]->pipe_in = UNSET; // or unset? STDIN
		prog_info->children[index]->pipe_out = prog_info->pipes[write_index];
	}
	else if (index == prog_info->cmd_count - 1)
	{
		prog_info->children[index]->pipe_in = prog_info->pipes[read_index];
		prog_info->children[index]->pipe_out = UNSET; // STDOUT?	
	}
	else
	{
		prog_info->children[index]->pipe_in = prog_info->pipes[read_index];
		prog_info->children[index]->pipe_out = prog_info->pipes[write_index];
	}
	printf("CHILD %i: in/read: %i out/write: %i\n", index, prog_info->children[index]->pipe_in, prog_info->children[index]->pipe_out); //
}

	//do we need to check if there will be an out?
	// do these after we fork
	// dup2(prog_info->pipes[read_index], 0); //reroute pipe contents to stdin
	// dup2(prog_info->pipes[write_index], 1); //reroute stdout to outfile
	// close(prog_info->pipes[write_index]); //close write end of pipe

int	create_pipes(t_eggcarton *prog_info)
{
	char *error;
	int	count;
	int	pipe_ret;

	prog_info->pipes = malloc(sizeof (int) * (prog_info->pipe_count * 2));
	if (!prog_info->pipes)
	{	
		printf("%sEggShell🥚: malloc failed%s\n", RED, KNRM);
		return (ERROR);
	}
	pipe_ret = 0;
	count = 0;
	while (count < prog_info->pipe_count && pipe_ret != -1)
	{
		printf("made pipe!\n");//
		pipe_ret = pipe(prog_info->pipes + (count * 2));
		count++;
	}
	if (pipe_ret == -1)
	{
		error = strerror(errno);
		printf("%sEggShell🥚:%s%s\n", RED, error, KNRM);
	}
	return (SUCCESS);
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
		setup_redirection((t_redirection *)cmd, prog_info, *index);
		tree_iterator((t_cmd *)((t_redirection *)cmd)->cmd, prog_info, index);
	}
	else if (cmd->type == EXECUTABLE_CMD)
	{
		setup_child((t_executable_cmd *)cmd, prog_info, *index);
	}
	return (SUCCESS);
}