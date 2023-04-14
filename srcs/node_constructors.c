/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_constructors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:54:23 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/14 17:12:50 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"


/*
** setup new pipe node
*/
t_cmd	*new_pipe(t_cmd *left, t_cmd *right)
{
	t_pipe	*new;

	new = (t_pipe *)malloc(sizeof(t_pipe));
	if (new == NULL)
		return (NULL);
	new->type = PIPE;
	new->left = (t_cmd *) left;
	new->right = (t_cmd *) right;
	return ((t_cmd *) new);
}

/*
** setup new line node
*/
t_cmd	*new_line(t_cmd *left, t_cmd *right)
{
	t_line	*new;

	new = (t_line *)malloc(sizeof(t_line));
	if (new == NULL)
		return (NULL);
	new->type = LINE;
	new->left = (t_cmd *) left;
	new->right = (t_cmd *) right;
	return ((t_cmd *) new);
}
