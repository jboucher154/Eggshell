/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_constructors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:54:23 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 14:28:35 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 *  new_pipe takes in pointers to the left and right commands. It creates a new
 *  pipe struct and returns a pointer to it. If malloc fails, it returns NULL.
 */
t_cmd	*new_pipe(t_cmd *left, t_cmd *right)
{
	t_pipe	*new;

	new = ft_calloc(1, sizeof(t_pipe));
	if (new == NULL)
		return (NULL);
	new->type = PIPE_CMD;
	new->left = (t_cmd *) left;
	new->right = (t_cmd *) right;
	return ((t_cmd *) new);
}

/*
 *  new_executable_cmd creates a new executable command struct and returns a
 *  pointer to it. If malloc fails, it returns NULL.
 */
t_exec	*new_executable_cmd(void)
{
	t_exec	*new;
	int		index;

	new = ft_calloc(1, sizeof(t_exec));
	if (!new)
		return (NULL);
	new->type = EXECUTABLE_CMD;
	new->cmd_path = NULL;
	new->args = ft_calloc(11, sizeof(char *));
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

/*
 * set_fd takes in a pointer to a redirection struct and a char representing
 * the token id. It sets the fd in the redirection struct based on the token id.
 */
static void	set_fd(t_redirection *new, char token_id)
{
	if (token_id == REDIRECT_IN)
		new->fd = STDIN_FILENO;
	else if (token_id == REDIRECT_OUT)
		new->fd = STDOUT_FILENO;
	else if (token_id == REDIRECT_OUT_APPEND)
		new->fd = STDOUT_FILENO;
}

/*
 * new_redirection takes in pointers to the current command, the start of the
 * file name, a char representing the token id, and a pointer to the
 * t_eggcarton struct. It creates a new redirection struct and returns a pointer
 * to it. If malloc fails, it returns NULL.
 */
t_cmd	*new_redirection(t_cmd *cmd, char **file_start, char token_id, \
t_eggcarton *prog_info)
{
	t_redirection	*new;

	new = ft_calloc(1, sizeof(t_redirection));
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
			print_error("ambiguous redirect");
	}
	move_pointer_past_ws(file_start);
	new->token_id = token_id;
	set_fd(new, token_id);
	return ((t_cmd *) new);
}
