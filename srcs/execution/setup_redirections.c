/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 15:43:15 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/04 16:01:23 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_filename(char *fname)
{
	if (!fname || !fname[0])
	{
		print_error("ambiguous redirect");
		return (ERROR);
	}
	else
		return (SUCCESS);
}

void	setup_redirection(t_redirection *redirection, t_eggcarton *prog_info, \
int index)
{
	int		fd;
	fd = OPEN_ERROR;
	if (redirection->token_id != REDIRECT_HERE && check_filename(redirection->filename) == ERROR)
		return ;
	if (redirection->token_id == REDIRECT_HERE) // THIS IS THE WRONG PLACE TO CALL THIS??!
	{
		heredoc_bultin(prog_info, redirection);
	}
	if (redirection->token_id == REDIRECT_IN)
	{
		fd = open(redirection->filename, O_RDONLY);
		close(prog_info->children[index]->redir_in);
		prog_info->children[index]->redir_in = fd;
	}
	else if (redirection->token_id == REDIRECT_OUT)
	{
		fd = open(redirection->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		close(prog_info->children[index]->redir_out);
		prog_info->children[index]->redir_out = fd;
	}
	else if (redirection->token_id == REDIRECT_OUT_APPEND)
	{
		fd = open(redirection->filename, O_WRONLY | O_CREAT, 0644);
		close(prog_info->children[index]->redir_out);
		prog_info->children[index]->redir_out = fd;
	}
	if (fd == OPEN_ERROR)
		print_errno_error();
}

void	process_redirections(t_redirection *redir, t_eggcarton *prog_info, \
int index)
{
	if (redir && (redir->cmd == NULL || redir->cmd->type == EXECUTABLE_CMD))
	{
		setup_redirection(redir, prog_info, index);
		return ;
	}
	else if (redir && redir->cmd->type == REDIRECTION_CMD)
	{
		process_redirections(((t_redirection *)redir->cmd), prog_info, index);
		setup_redirection(redir, prog_info, index);
	}
}
