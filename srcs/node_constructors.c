/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_constructors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:54:23 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/26 17:59:51 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** setup new pipe node
*/
t_cmd	*new_pipe(t_cmd *left, t_cmd *right)
{
	t_pipe	*new;

	new = (t_pipe *)malloc(sizeof(t_pipe));
	if (new == NULL)
		return (NULL);
	new->type = PIPE_CMD;
	new->left = (t_cmd *) left;
	new->right = (t_cmd *) right;
	return ((t_cmd *) new);
}
