/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:44:45 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/21 15:44:55 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

t_child	*new_child(void)
{
	t_child	*child;

	child = (t_child *) malloc(sizeof(t_child));
	if (!child)
		return (NULL);
	child->path = NULL;
	child->args = NULL;
	child->redir_in = -1;
	child->redir_out = -1;
	child->pipe_in = -1;
	child->pipe_out = -1;
	child->fd_to_close = -1; //
	return (child);
}

// void clean_up_children(t_child **children)
// {
// 	int	index;
// 	int arg_index;

// 	index = 0;
// 	while(children[index] && children[index]->args)
// 	{
// 		arg_index = 0;
// 		while(children[index]->args[arg_index])
// 		{
// 			free(children[index]->args[arg_index]);
// 			arg_index++;
// 		}
// 		free(children[index]->args);
// 		index++;
// 	}
// 	free_children(children);
// }	

void	free_children(t_child **children)
{
	int	index;

	index = 0;
	while (children[index])
	{
		free(children[index]);
		index++;
	}
	free(children);
	children = NULL;
}

int	create_child_array(t_eggcarton *prog_info)
{
	t_child	**children;
	int		index;
	
	children = (t_child **) malloc(sizeof(t_child *) * (prog_info->cmd_count + 1));
	if (!children)
		return (ERROR);
	index = 0;
	children[prog_info->cmd_count] = NULL;
	while(index < prog_info->cmd_count)
	{
		children[index] = new_child();
		if (!children[index])
		{
			free_children(children);
			return (ERROR);
		}
		index++;
	}
	prog_info->children = children;
	return (SUCCESS);
}