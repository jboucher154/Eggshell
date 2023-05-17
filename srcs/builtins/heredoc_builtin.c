/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 15:11:03 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/17 15:25:58 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_getline(void)
{
	char	*line_read;

	line_read = (char *) 0;
	if (line_read)
	{
		free(line_read);
		line_read = (char *) 0;
	}
	line_read = readline(">");
	return (line_read);
}

static char	*gather_here_doc(t_redirection *redirection)
{
	char	*line_read;
	char	*input;
	char	*joined;

	input = ft_calloc(1, sizeof(char));
	line_read = heredoc_getline();
	while (line_read != NULL)
	{
		if (line_read && line_read[0] && !ft_strncmp(redirection->filename, \
		line_read, ft_strlen(line_read)))
			break ;
		else if (line_read)
		{
			joined = ft_strjoin_three(input, line_read, "\n");
			free(input);
			free(line_read);
			input = joined;
		}
		if (!input)
			break ;//
		line_read = heredoc_getline();
	}
	return (input);
}

static void	run_here_child(t_eggcarton *prog_info, t_redirection *redirection, \
int index)
{
	char	*input;

	close_pipes(prog_info->pipes, prog_info->pipe_count);
	close_redirections(prog_info->children[index]->redir_in, \
	prog_info->children[index]->redir_out);
	input = gather_here_doc(redirection);
	if (input)
	{
		if (redirection->expand_variable == TRUE)
			input = check_for_expansions(prog_info, input);
		write(prog_info->children[index]->heredoc_fd, input, ft_strlen(input));
		free(input);
	}
	close(prog_info->children[index]->heredoc_fd);
	exit (0);
}

void	heredoc_builtin(t_eggcarton *prog_info, t_redirection *redirection, \
int index)
{
	int		pid;
	int		exit_status;

	exit_status = 0;
	prog_info->children[index]->heredoc_fd = open(HEREDOC_TEMP, \
	O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (prog_info->children[index]->heredoc_fd == -1)
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		initialize_heredoc_signals();
		run_here_child(prog_info, redirection, index);
	}
	close(prog_info->children[index]->heredoc_fd);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &exit_status, 0);
	if (WEXITSTATUS(exit_status) > 0)
	{
		prog_info->should_execute = FALSE;
		ht_update_value(prog_info->environment, "?", ft_strdup("1"));
	}
}
