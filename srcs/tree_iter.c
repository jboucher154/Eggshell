/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_iter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:50:12 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/24 16:46:42 by jebouche         ###   ########.fr       */
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
	prog_info->children[index]->args = cmd->args; //clean in child not cmd tree
	printf("ARRAY BEFORE EXPANSION\n");
	print_array(prog_info->children[index]->args); //test
	if (check_for_expansions(prog_info, prog_info->children[index]->args, 0) == ERROR)
		printf("%sEggShell🥚: expansion error%s\n", RED, KNRM); //continue??
	printf("ARRAY AFTER EXPANSION\n");
	print_array(prog_info->children[index]->args); //test
	prog_info->children[index]->path = get_path(prog_info, cmd->args[0]);
}

void	setup_redirection(t_redirection *redirection, t_eggcarton *prog_info, int index)
{
	int	fd;
	char *error;
	printf("FILENAME before expansion: %s\n", redirection->filename);//
	check_for_expansions(prog_info, &(redirection->filename), 1);
	printf("FILENAME after expansion: %s\n", redirection->filename);//
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
		int	read_index;
		int	write_index;
		
		write_index = index * 2 + 1;
		read_index = index - 1 * 2;
		if (index == 0)
		{
			prog_info->children[index]->pipe_in = STDIN_FILENO;
			prog_info->children[index]->pipe_out = prog_info->pipes[1];

		}
		else if (index == prog_info->cmd_count)
		{
			read_index = index - 1 * 2;
			prog_info->children[index]->pipe_in = prog_info->pipes[read_index];
			prog_info->children[index]->pipe_out = STDOUT_FILENO;	
		}
		else
		{
			prog_info->children[index]->pipe_in = prog_info->pipes[read_index];
			prog_info->children[index]->pipe_out = prog_info->pipes[write_index];
		}
		
		//do we need to check if there will be an out?
		// do these after we fork
		// dup2(prog_info->pipes[read_index], 0); //reroute pipe contents to stdin
		// dup2(prog_info->pipes[write_index], 1); //reroute stdout to outfile
		// close(prog_info->pipes[write_index]); //close write end of pipe
}

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
		//left
		tree_iterator((t_cmd *)((t_pipe *)cmd)->left, prog_info, index);
		//increment index
		(*index)++;
		//right
		tree_iterator((t_cmd *)((t_pipe *)cmd)->right, prog_info, index);
		//return (tree_iterator(left) == SUCCESS && tree_iterator(right) == SUCCESS)?
	}
	else if (cmd->type == REDIRECTION_CMD)
	{
		//assign to current index in children?
		setup_redirection((t_redirection *)cmd, prog_info, *index);
		tree_iterator((t_cmd *)((t_redirection *)cmd)->cmd, prog_info, index);
	}
	if (cmd->type == EXECUTABLE_CMD)
	{
		//setup the command stuffs to current index
		setup_child((t_executable_cmd *)cmd, prog_info, *index);
	}
	return (SUCCESS);//?
}