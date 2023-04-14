/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parse.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 14:42:17 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/14 15:43:21 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

char	*find_filename(char *file_start)  //can probably turn this into a utility that finds end of 'word' and returns malloc'd copy
{
	char	*filename;
	int		len;

	len  = 0;
	while (!ft_strchr(WHITESPACE, file_start[len])) //should this be checked against tokens too?
		len++;
	filename = ft_substr(file_start, 0, len); //+1 to len? no it should point to whitespace so that should be okay
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
	(*file_start) += ft_strlen(new->filename);
	return ((t_cmd *) new);
}

t_cmd	*handle_redirection(t_cmd *cmd, char **parsed_string, char *end)
{
	t_cmd	*new_redir;
	int	token_id;

	new_redir = cmd;
	while (*parsed_string < end && peek_next_token(*parsed_string, "<>")) //check if there is a redirection coming
	{
		token_id = move_to_token(parsed_string, end); //move to redirection
		if (token_id == REDIRECT_OUT_APPEND)
			(*parsed_string) += 2;
		else
			(*parsed_string)++;
		while (ft_strchr(WHITESPACE, **parsed_string)) //move past redir and skip any whitespace
			(*parsed_string)++;
		if (identify_token(*parsed_string) != ALPHA)
		{
			printf("ERROR!!!!!!!\n");// print error and exit this process!!
		}
		new_redir = new_redirection(new_redir, parsed_string, token_id);
	}
	return (new_redir);
}