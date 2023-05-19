/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 15:11:03 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 14:40:16 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
*  heredoc_getline() reads a line from stdin and returns it as a string using
*  the readline library. It returns NULL if there is an error or if the user
*/
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

/*
*  gather_here_doc() reads lines from stdin until it finds a line that matches
*  the delimiter given or until the user preses Ctl-D or Ctl-C . It returns the 
*  lines read as a string.
*/
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
		line_read, ft_strlen(redirection->filename) + 1))
			break ;
		else if (line_read)
		{
			joined = ft_strjoin_three(input, line_read, "\n");
			free(input);
			free(line_read);
			input = joined;
		}
		if (!input)
			break ;
		line_read = heredoc_getline();
	}
	return (input);
}

/*
*  run_here_child() is the child process of the heredoc builtin. It closes the
*  pipes and redirections that are not needed, gathers the input from stdin,
*  checks for expansions, and writes the input to the temporary file.
*/
static void	run_here_child(t_eggcarton *prog_info, t_redirection *redirection, \
int index, int fd)
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
		write(fd, input, ft_strlen(input));
		free(input);
	}
	close(fd);
	exit (0);
}

/*
*  heredoc_parent() is the parent process of the heredoc builtin. It closes the
*  temporary file, ignores the SIGINT signal, waits for the child process to
*  finish, and updates the exit status of the program if an error occured.
*/
void	heredoc_parent(t_eggcarton *prog_info, int fd, int pid, int exit_status)
{
	close(fd);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &exit_status, 0);
	if (WEXITSTATUS(exit_status) > 0)
	{
		prog_info->should_execute = FALSE;
		ht_update_value(prog_info->environment, "?", ft_strdup("1"));
	}
}

/*
*  heredoc_builtin() is the redirection function that handles '<<'. It takes a 
*  t_eggcarton pointer, a t_redirection pointer, and an int index as arguments.
*  It creates a temporary file based on the index, forks a child process, and 
*  runs the child process.
*/
void	heredoc_builtin(t_eggcarton *prog_info, t_redirection *redirection, \
int index)
{
	int		pid;
	int		exit_status;
	int		fd;

	exit_status = 0;
	prog_info->children[index]->here_doc = get_heredoc_name(index);
	if (!prog_info->children[index]->here_doc)
	{
		print_errno_error();
		return ;
	}
	fd = open(prog_info->children[index]->here_doc, \
	O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		initialize_heredoc_signals();
		run_here_child(prog_info, redirection, index, fd);
	}
	heredoc_parent(prog_info, fd, pid, exit_status);
}
