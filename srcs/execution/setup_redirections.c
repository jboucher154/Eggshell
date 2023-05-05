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

int	open_file(t_redirection *redirection)
{
	int	fd;

	fd = UNSET;
	if (redirection->token_id == REDIRECT_IN)
	{
		fd = open(redirection->filename, O_RDONLY);
	}
	else if (redirection->token_id == REDIRECT_OUT)
	{
		fd = open(redirection->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (redirection->token_id == REDIRECT_OUT_APPEND)
	{
		fd = open(redirection->filename, O_WRONLY | O_CREAT, 0644);
	}
	if (fd == OPEN_ERROR)
		print_errno_error();
	return (fd);
}

void	close_file(t_eggcarton *prog_info, int index, int redir_type)
{
	if (redir_type == REDIRECT_IN)
	{
		close(prog_info->children[index]->redir_in);
	}
	else if (redir_type == REDIRECT_OUT || redir_type == REDIRECT_OUT_APPEND)
	{
		close(prog_info->children[index]->redir_out);
	}
}


void	setup_redirection(t_redirection *redirection, t_eggcarton *prog_info, \
int index)
{
	int		fd;
	fd = OPEN_ERROR;
	if (redirection->token_id != REDIRECT_HERE && check_filename(redirection->filename) == ERROR)
		return ;
	if (redirection->token_id == REDIRECT_HERE)
	{
		heredoc_bultin(prog_info, redirection, index);
		if (prog_info->children[index]->heredoc_pipe[0] != UNSET)
		{
			close(prog_info->children[index]->redir_in);
			prog_info->children[index]->redir_in = prog_info->children[index]->heredoc_pipe[0];
		}
		return ;
	}
	fd = open_file(redirection);
	close_file(prog_info, index, redirection->token_id);
	if (redirection->token_id == REDIRECT_IN)
		prog_info->children[index]->redir_in = fd;
	else if (redirection->token_id == REDIRECT_OUT)
		prog_info->children[index]->redir_out = fd;
	else if (redirection->token_id == REDIRECT_OUT_APPEND)
		prog_info->children[index]->redir_out = fd;
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
