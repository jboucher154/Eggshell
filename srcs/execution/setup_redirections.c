/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 15:43:15 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/16 16:13:27 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_filename(char *fname, t_eggcarton *prog_info)
{
	if (!fname || !fname[0])
	{
		print_error("ambiguous redirect");
		ht_update_value(prog_info->environment, "?", ft_itoa(1));
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
		fd = open(redirection->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	if (fd == OPEN_ERROR)
		print_errno_error();
	return (fd);
}

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

void	setup_redirection(t_redirection *redirection, t_eggcarton *prog_info, \
int index)
{
	int		fd;
	fd = OPEN_ERROR;
	if (redirection->token_id != REDIRECT_HERE && check_filename(redirection->filename, prog_info) == ERROR)
		return ;
	if (redirection->token_id == REDIRECT_HERE)
	{
		heredoc_builtin(prog_info, redirection, index);
		prog_info->children[index]->heredoc_fd = open(HEREDOC_TEMP, O_RDONLY); //error if open -1?
		close(prog_info->children[index]->redir_in);
		prog_info->children[index]->redir_in = prog_info->children[index]->heredoc_fd;
		// close(prog_info->children[index]->heredoc_pipe[1]);// added this in to try and fix hanging process...
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
		//add check here if redirection should be setup
		setup_redirection(redir, prog_info, index);
		return ;
	}
	else if (redir && redir->cmd->type == REDIRECTION_CMD)
	{
		process_redirections(((t_redirection *)redir->cmd), prog_info, index);
		//add check here if redirection should be setup
		setup_redirection(redir, prog_info, index);
	}
}
