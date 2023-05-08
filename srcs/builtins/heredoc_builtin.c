#include "minishell.h"

// static char	*heredoc_get(void)
// {
// 	char	*line_read;

// 	line_read = (char *) 0;
// 	if (line_read)
// 	{
// 		free(line_read);
// 		line_read = (char *) 0;
// 	}
// 	line_read = readline(">");
// 	return (line_read);
// }

// static void	pass_to_child(t_eggcarton *prog_info, char *input, int index)
// {	
// 	// if (pipe(prog_info->children[index]->heredoc_pipe) == -1)
// 	// {
// 	// 	print_error("pipe creation failed");
// 	// 	prog_info->children[index]->heredoc_pipe[1] = UNSET;
// 	// 	prog_info->children[index]->heredoc_pipe[0] = UNSET;
// 	// 	return ;
// 	// }
// 	// (void) input;
// 	write(prog_info->children[index]->heredoc_pipe[1], input, ft_strlen(input));
// 	close(prog_info->children[index]->heredoc_pipe[1]);
// }

// void	heredoc_bultin(t_eggcarton *prog_info, t_redirection *redirection, int index)
// {    
//     char	*line_read;
// 	char	*input = NULL;
// 	char 	*joined;
// 	int		pid;
// 	int		exit_status;

// 	initialize_heredoc_signals();
// 	exit_status = 0;
// 	if (pipe(prog_info->children[index]->heredoc_pipe) == -1)
// 	{
// 		print_error("pipe creation failed");
// 		prog_info->children[index]->heredoc_pipe[1] = UNSET;
// 		prog_info->children[index]->heredoc_pipe[0] = UNSET;
// 		return ;
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 		return ;
// 	if (pid == 0)
// 	{
// 		input = ft_calloc(1, sizeof(char));
// 		line_read = heredoc_get();
// 		while (line_read != NULL)
// 		{

// 			if (line_read && line_read[0] && !ft_strncmp(redirection->filename, line_read, ft_strlen(line_read)))
// 			{
// 				break ;
// 			}
// 			else if (line_read)
// 			{
// 				joined = ft_strjoin_three(input, line_read, "\n");
// 				free(input);
// 				input = joined;
// 			}
// 			line_read = heredoc_get();
// 		}
// 		if (input)
// 		{
// 			if (redirection->expand_variable == TRUE)
// 				input = check_for_expansions(prog_info, input);
// 			pass_to_child(prog_info, input, index);
// 			free(input);
// 		}
// 		exit(0); //some code here that we can check with the parent to bail if needed
// 	}
// 		waitpid(pid, &exit_status, 0);
// 		printf("heredoc finished\n");
// }


static char	*heredoc_get(void)
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

static void	pass_to_child(t_eggcarton *prog_info, char *input, int index)
{	
	if (pipe(prog_info->children[index]->heredoc_pipe) == -1)
	{
		print_error("pipe creation failed");
		prog_info->children[index]->heredoc_pipe[1] = UNSET;
		prog_info->children[index]->heredoc_pipe[0] = UNSET;
		return ;
	}
	(void) input;
	write(prog_info->children[index]->heredoc_pipe[1], input, ft_strlen(input));
	close(prog_info->children[index]->heredoc_pipe[1]);
}

void	heredoc_bultin(t_eggcarton *prog_info, t_redirection *redirection, int index)
{    
    char	*line_read;
	char	*input = NULL;
	char 	*joined;

	initialize_heredoc_signals();
	(void) prog_info->pipe_count;
	(void) index;
	input = ft_calloc(1, sizeof(char));
	line_read = heredoc_get();
	while (line_read != NULL)
	{

		if (line_read && line_read[0] && !ft_strncmp(redirection->filename, line_read, ft_strlen(line_read)))
		{
			break ;
		}
		else if (line_read)
		{
			joined = ft_strjoin_three(input, line_read, "\n");
			free(input);
			input = joined;
		}
		line_read = heredoc_get();
	}
	if (input)
	{
		if (redirection->expand_variable == TRUE)
			input = check_for_expansions(prog_info, input);
		pass_to_child(prog_info, input, index);
		free(input);
	}
}