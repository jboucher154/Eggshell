/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 11:28:46 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/26 17:59:30 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

///THIS FILE FOR TESTING ONLY NOT FOR FINAL PROJECT!!!

void	print_array(char **array)
{
	int	index;

	index = 0;
	while (array && array[index])
	{
		printf("ARG: %s ", array[index]);
		index++;
	}
	printf("\n");
}

//if we want a temp variable here it could be a union....
void	print_tree(t_cmd *cmd, int	depth)
{
	if (cmd->type == REDIRECTION_CMD)
	{
		print_tree((t_cmd *)(((t_redirection *)cmd)->cmd), depth + 1);
		printf("REDIRECTION found, depth: %i\n", depth);
		printf("FILENAME from redir: %s\n", ((t_redirection *)cmd)->filename);
	}
	else if (cmd->type == PIPE_CMD)
	{
		print_tree((t_cmd *)(((t_pipe *)(cmd))->left), depth + 1);
		print_tree((t_cmd *)(((t_pipe *)(cmd))->right), depth + 1);
		printf("PIPE found, depth: %i\n", depth);
	}
	else if (cmd->type == EXECUTABLE_CMD)
	{
		printf("EXECUTABLE CMD found: %s, depth: %i\n", ((t_executable_cmd *)cmd)->args[0] ,depth);
		print_array(((t_executable_cmd *)cmd)->args);
	}
	else
	{
		printf("SOMETHING IS WRONG, depth: %i\n", depth);
	}
}

void	print_children(t_child **children)
{
	int		i;

	i = 0;
	while (children[i] != NULL)
	{
		printf("CHILD %i: \n redir in: %i\n redir out: %i\n pipe in: %i\n pipe out%i\n", i, \
		children[i]->redir_in, children[i]->redir_out, children[i]->pipe_in, children[i]->pipe_out);
		// if (children[i]->path)
		printf("PATH: %s\n", children[i]->path);

		print_array(children[i]->args);
		// printf("PIPE CHECK for child %i: in: %i out: %i\n", i, children[i]->pipe_in,children[i]->pipe_out);
		printf("-------------------------------------------------------------\n");
		i++;
	}
}