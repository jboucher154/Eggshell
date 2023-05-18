/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_constructors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:54:23 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/18 14:50:07 by jebouche         ###   ########.fr       */
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

t_executable_cmd	*new_executable_cmd(void)
{
	t_executable_cmd	*new;
	int					index;

	new = (t_executable_cmd *) malloc(sizeof(t_executable_cmd));
	if (!new)
		return (NULL);
	new->type = EXECUTABLE_CMD;
	new->cmd_path = NULL;
	new->args = (char **) malloc(sizeof(char *) * 11);
	if (!new->args)
	{
		free(new);
		return (NULL);
	}
	index = 0;
	while (index < 11)
	{
		new->args[index] = NULL;
		index++;
	}
	new->arg_count = 0;
	return (new);
}

t_cmd	*new_redirection(t_cmd *cmd, char **file_start, char token_id, \
t_eggcarton *prog_info)
{
	t_redirection	*new;

	new = (t_redirection *)malloc(sizeof(t_redirection));
	if (new == NULL)
		return (NULL);
	new->type = REDIRECTION_CMD;
	new->cmd = (t_cmd *) cmd;
	new->expand_variable = TRUE;
	if (ft_strchr(QUOTES, **file_start))
		new->expand_variable = FALSE;
	if (token_id == REDIRECT_HERE)
		new->filename = get_arg(file_start, prog_info, FALSE);
	else
	{
		new->filename = get_arg(file_start, prog_info, TRUE);
		if (!new->filename || !new->filename[0])
		{
			print_error("ambiguous redirect");
		}
	}
	move_pointer_past_ws(file_start);
	new->token_id = token_id;
	if (token_id == REDIRECT_IN)
		new->fd = STDIN_FILENO;
	else if (token_id == REDIRECT_OUT)
		new->fd = STDOUT_FILENO;
	else if (token_id == REDIRECT_OUT_APPEND)
		new->fd = STDOUT_FILENO;
	return ((t_cmd *) new);
}
