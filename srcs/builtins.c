#include "minishell.h"
#include "ft_hash.h"
// #include <string.h>//strerror
// #include <errno.h>//

// we need to test error handling with quotes
void	echo_command(char **args)
{
	int		new_line;

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

void	pwd_command(void)
{
	char	*current_wd;

	current_wd = getcwd(NULL, 0);
	printf("%s\n", current_wd);
	free(current_wd);
}

void	cd_command(char	**args, t_hash_table *ht_env)
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

void	unset_command(char **args, t_hash_table *ht_env)
{
	//needs to handle multiple unsets in one call
	int	index;

	index = 1;
	while (args[index] != '\0')
	{	
		ht_remove(ht_env, args[index]);
		index++;
	}
}

void	export_command(char **args, t_hash_table *ht_env)
{
	//needs to handle multiple exports in one call

	int		index;
	char	*key;
	char	*value;

	index = 0;
	while (args[1][index] != '=')
		index++;
	key = ft_substr(args[1], 0, index);
		index++;
	value = ft_strdup(args[1] + index);
	ht_add(ht_env, key, value);
	free(key);
}

void	print_enviroment(t_hash_table	*ht_env)
{
	ht_print(ht_env);
}

void	clearing(void)
{
	printf("\033[H\033[J");
}