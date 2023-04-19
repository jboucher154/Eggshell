#include "../includes/minishell.h"
#include "../ft_hash/ft_hash.h"
#include <string.h>//strerror
#include <errno.h>//

// we need to test error handling with quotes
static void echo_command(char **args)
{
	int new_line;

	new_line = 0;
	args++;
	if (!ft_strncmp("-n", *args, 2))
	{
		new_line = 1;
		args++;
	}
	while (*args != '\0')
	{
		printf("%s ", *args);
		args++;
	}
	if (new_line != 1)
		printf("\n");
}

static void pwd_command(void)
{
	char	*current_wd;

	current_wd = getcwd(NULL, 0);
	printf("%s\n", current_wd);
	free(current_wd);
}

static void cd_command(char	**args, t_hash_table *ht_env)
{
	//changing OLDPWD before CD
	char	*current_wd;
	char	*to;
	char	*error;

	current_wd = getcwd(NULL, 0);
	ht_update_value(ht_env, "OLDPWD", (void *) current_wd);
	
	if (args[1] == '\0')
		to = getenv("HOME");
	else
		to = args[1];
	if (chdir(to) == -1)
	{
		error = strerror(errno);
		printf("%sEggShell🥚: %s: %s%s\n", RED, args[1], error, KNRM);
	}
	current_wd = getcwd(NULL, 0);
	ht_update_value(ht_env, "PWD", (void *) current_wd);
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

void unset_command(char **args, t_hash_table *ht_env)
{
	//needs to handle multiple unsets in one call
	int index;

	index = 1;
	while (args[index] != '\0')
	{	
		ht_remove(ht_env, args[index]);
		index++;
	}
}

void export_command(char **args, t_hash_table *ht_env)
{
	//needs to handle multiple exports in one call

	int index;
	char *key;
	char *value;

	index = 0;
	while (args[1][index] != '=')
		index++;
	key = ft_substr(args[1], 0, index);
		index++;
	value = ft_strdup(args[1] + index);
	ht_add(ht_env, key, value);
	free(key);
}

void print_enviroment(t_hash_table	*ht_env)
{
	ht_print(ht_env);
}

void clearing(void)
{
	printf("\033[H\033[J");
}

static void eggshell(t_hash_table	*ht_env)
{
	int status;
	char *line;
  	char *args[5];
	args[0] = "export";
	args[1] = "32THINGS=stuff";
	args[2] = "THINGS";
	args[3] = "neg seg";
	args[4] = NULL;

	status = 0;
	while (status != 1)
	{
		line = ft_gets();
		if (!ft_strncmp("pwd", line, 4) || !ft_strncmp("PWD", line, 4))
			pwd_command();
		if (!ft_strncmp("cd", line, 3))
			cd_command(args, ht_env);
		if (!ft_strncmp("export", line, 6))
			export_command(args, ht_env);
		if (!ft_strncmp("unset", line, 5))
			unset_command(args, ht_env);
		if ((!ft_strncmp("echo", line, 5) || !ft_strncmp("ECHO", line, 4)))
			echo_command(args);
		if (!ft_strncmp("env", line, 4))
			print_enviroment(ht_env);
		if (!ft_strncmp("CLEAR", line, 5))
			clearing();
		if (!ft_strncmp("EXIT", line, 4))
			status = 1;
		if (line)
			free(line);
	}
	exit (EXIT_SUCCESS);
}

static void initialize_env_table(t_hash_table *ht_env, char **envp)
{
	int index;
	int out_index;
	char *key;
	char *value;
	
	index = 0;
	out_index = 0;
	while (envp[out_index])	
	{
		while (envp[out_index][index] != '=')
			index++;
		key = ft_substr(envp[out_index], 0, index);
		index++;
		value = ft_strdup(envp[out_index] + index);
		ht_add(ht_env, key, value);
		free(key);
		index = 0;
		out_index++;
	}	
}

int main(int argc, char **argv, char **envp)
{
	t_hash_table	*ht_env;

	ht_env = ht_create(150);
	initialize_env_table(ht_env, envp);
	eggshell(ht_env);
	exit(EXIT_SUCCESS);
}