#include "../includes/minishell.h"

/* Iterate string and identify tokens, add to linked list, return head of list */
void tonkenizer(char *input_str)
{
	//t_list *head;
	printf("entered tokenizer\n");
	int		start;
	int		stop;
	char *cmd = '\0';
	char *arg = '\0';

	start = 0;
	stop = 0;
	//head = ft_lstnew(NULL);
	// if (!head)
	// 	return NULL;
	// iterate string
	//!ft_strchr(TOKENS, input_str[j]) ||

	while (input_str[start] != '\0')
	{
		while (input_str[stop] && input_str[stop] != ' ')
			stop++;
		if (ft_strncmp("echo", input_str + start, 4) == 0)
		{
			cmd = ft_substr(input_str, start, stop - start);
			arg = ft_strdup(input_str + stop);
		}
		start = ++stop;
	}
	printf("%s:	%s\n", cmd, arg);
	//when string == token or non-token?
}


/* Read a string, and return a pointer to it. */
static char *ft_gets (void)
{
	char *line_read;
	
	line_read = (char *) 0;
	if (line_read)
    {
      free (line_read);
      line_read = (char *) 0;
    }
	line_read = readline (PROMPT);
	if (line_read && *line_read)
   		add_history (line_read);
	return (line_read);
}

static void eggshell(void)
{
	int status;
	char *line;
  	//char **args; parsed tokens??

	status = 0;
	while (status != 1)
	{
		line = ft_gets();
		tonkenizer(line);
		if (!ft_strncmp("EXIT", line, 4))
			status = 1;
		if (line)
			free(line);
	}
	exit (EXIT_SUCCESS);
}

//int argc, char **argv, char **envp
int main(void)
{
	eggshell();
	exit(EXIT_SUCCESS);
}
