/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 15:43:15 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 13:44:59 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * open_file opens a file and returns the file descriptor.
 * If the file cannot be opened, it prints an error message.
 */
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
		fd = open(redirection->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	if (fd == OPEN_ERROR && redirection->filename && redirection->filename[0])
		print_errno_blame(NULL, redirection->filename);
	return (fd);
}

/*
 * close_file closes a file descriptor if it is not UNSET.
 */
void	close_file(t_eggcarton *prog_info, int index, int redir_type)
{
	if (redir_type == REDIRECT_IN)
	{
		if (prog_info->children[index]->redir_in != UNSET)
			close(prog_info->children[index]->redir_in);
	}
	else if (redir_type == REDIRECT_OUT || redir_type == REDIRECT_OUT_APPEND)
	{
		if (prog_info->children[index]->redir_out != UNSET)
			close(prog_info->children[index]->redir_out);
	}
}

/*
 * setup_redirection sets up the redirections for a child process.
 * If the redirection is a heredoc, it calls the heredoc_builtin function.
 * Otherwise, it opens the file and closes the file descriptor. The opened
 * file descriptor is then assigned to the child process.
 */
void	setup_redirection(t_redirection *redirection, t_eggcarton *prog_info, \
int index)
{
	int		fd;

	fd = OPEN_ERROR;
	if (redirection->token_id == REDIRECT_HERE)
	{
		heredoc_builtin(prog_info, redirection, index);
		fd = open(prog_info->children[index]->here_doc, O_RDONLY);
		if (fd == OPEN_ERROR)
			print_errno_blame(NULL, prog_info->children[index]->here_doc);
		close(prog_info->children[index]->redir_in);
		prog_info->children[index]->redir_in = fd;
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

/*
 * process_redirections iterates n redirections recursively. If the file
 * cannot be opened, it returns and stops processing redirections.
 */
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
		if (prog_info->children[index]->redir_in == OPEN_ERROR || \
			prog_info->children[index]->redir_out == OPEN_ERROR)
			return ;
		setup_redirection(redir, prog_info, index);
	}
}
