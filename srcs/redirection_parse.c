/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parse.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 14:42:17 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/27 10:13:15 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_filename(char *file_start)  //can probably turn this into a utility that finds end of 'word' and returns malloc'd copy
{
	char	*filename;
	int		len;

	len  = 0;
	while (!ft_strchr(WHITESPACE, file_start[len]))
		len++;
	filename = ft_substr(file_start, 0, len);
	return (filename);

}

t_cmd	*new_redirection(t_cmd *cmd, char **file_start, char token_id) //should the index be modified by pointer here?
{
	t_redirection	*new;

	new = (t_redirection *)malloc(sizeof(t_redirection));
	if (new == NULL)
		return (NULL);
	new->type = REDIRECTION_CMD;
	new->cmd = (t_cmd *) cmd;
	new->filename = find_filename(*file_start);
	new->token_id = token_id;
	(*file_start) += ft_strlen(new->filename);
	move_pointer_past_ws(file_start);
	if (token_id == REDIRECT_IN)
		new->fd = STDIN_FILENO;
	else if (token_id == REDIRECT_OUT)
		new->fd = STDOUT_FILENO;
	else if (token_id == REDIRECT_OUT_APPEND)
		new->fd = STDOUT_FILENO;
	return ((t_cmd *) new);
}

t_cmd	*handle_redirection(t_cmd *cmd, char **parsed_string, char *end)
{
	t_cmd	*new_redir;
	int	token_id;

	new_redir = cmd;
	move_pointer_past_ws(parsed_string);
	while (*parsed_string < end && (**parsed_string == '<' || **parsed_string == '>')) //check if there is a redirection coming
	{
		token_id = move_to_token(parsed_string, end); //move to redirection
		if (token_id == REDIRECT_OUT_APPEND)
			(*parsed_string) += 2;
		else
			(*parsed_string)++;
		move_pointer_past_ws(parsed_string);
		if (identify_token(*parsed_string) != ALPHA)
			print_error("Error");
		new_redir = new_redirection(new_redir, parsed_string, token_id);
		move_pointer_past_ws(parsed_string);
	}
	return (new_redir);
}