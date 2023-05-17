/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:44:45 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/16 16:15:53 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_child	*new_child(void)
{
	t_child	*child;

	child = (t_child *) malloc(sizeof(t_child));
	if (!child)
		return (NULL);
	child->path = NULL;
	child->args = NULL;
	child->command_present = TRUE;
	child->redir_in = UNSET;
	child->redir_out = UNSET;
	// child->heredoc_pipe[0] = UNSET;
	// child->heredoc_pipe[1] = UNSET;
	child->heredoc_fd = UNSET;
	child->pipe_in = UNSET;
	child->pipe_out = UNSET;
	return (child);
}

void	free_children(t_child **children)
{
	int	index;

	index = 0;
	while (children[index])
	{
		close_redirections(children[index]->redir_in, \
		children[index]->redir_out);
		if (children[index]->heredoc_fd != UNSET)
			unlink(HEREDOC_TEMP);
		// close_redirections(children[index]->heredoc_pipe[0], \
		// children[index]->heredoc_pipe[1]);
		clean_str_array(children[index]->args);
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
	int		child_count;

	child_count = 1 + prog_info->pipe_count;
	children = (t_child **) malloc(sizeof(t_child *) * (child_count + 1));
	if (!children)
		return (print_error("malloc failed"));
	index = 0;
	children[prog_info->cmd_count] = NULL;
	while (index < prog_info->cmd_count)
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
