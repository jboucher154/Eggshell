#include "../includes/minishell.h"


static int	mini_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n > 0 && *s1 == *s2 && *s1 != '\0' && *s2 != '\0')
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	return (((unsigned char) *s1) - ((unsigned char) *s2));
}

static void echo_command(char **args)
{
	int new_line;

	new_line = 0;
	args++;
	if (!mini_strncmp("-n", *args, 2))
	{
		new_line = 1;
		args++;
	}
	while (*args != '\0')
	{
		printf("%s\n", *args);
		args++;
	}
	
	printf("%i\n", new_line);
}


static void pwd_command(void)
{
	char	*current_wd;

	current_wd = getcwd(NULL, 0);
	printf("%s\n", current_wd);
	free(current_wd);
}

static void cd_command(char	**args, char **enviro)
{
	//changing OLDPWD before CD
	char	*current_wd;
	char	*to;
	
	current_wd = getcwd(NULL, 0);
	printf("at the start the current_wd = %s	home = %s\n", current_wd, getenv("HOME"));
	if (args[1] == '\0')
		to = getenv("HOME");
	else
		to = args[1];
	chdir(to);
	current_wd = getcwd(NULL, 0);
	printf("after the current_wd = %s\n", current_wd);
	return ;
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

void print_enviroment(char **enviro)
{
	while (*enviro != '\0')
	{	
		printf("%s\n", *enviro);
		enviro++;
	}
}

static void eggshell(char **enviro)
{
	int status;
	char *line;
  	char *args[5];
	args[0] = "echo";
	args[1] = "-n";
	args[2] = "see";
	args[3] = "neg seg";
	args[4] = NULL;

	status = 0;
	print_enviroment(enviro);
	while (status != 1)
	{
		line = ft_gets();
		if (!mini_strncmp("pwd", line, 4) || !mini_strncmp("PWD", line, 4))
			pwd_command();
		if (!mini_strncmp("cd", line, 4))
			cd_command(args, enviro);
		if ((!mini_strncmp("echo", line, 4) || !mini_strncmp("ECHO", line, 4)))
			echo_command(args);
		if (!mini_strncmp("env", line, 4))
			print_enviroment(enviro);
		if (!mini_strncmp("EXIT", line, 4))
			status = 1;
		if (line)
			free(line);
	}
	exit (EXIT_SUCCESS);
}

//int argc, char **argv, char **envp
int main(int argc, char **argv, char **envp)
{
	char **enviro;
	
	enviro = envp;
	eggshell(enviro);
	exit(EXIT_SUCCESS);
}