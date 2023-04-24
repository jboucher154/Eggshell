#include "../includes/minishell.h"

/* Read a string, and return a pointer to it. */
static char *ft_gets (void)
{
	char *line_read;
	
	line_read = (char *) 0;
	if (line_read)
    {
      free(line_read);
      line_read = (char *) 0;
    }
	line_read = readline (PROMPT);
	if (line_read && *line_read)
   		add_history (line_read);
	return (line_read);
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

static int eggshell(t_eggcarton *prog_info)
{
	int		status;
	char	*line;
	t_cmd	*cmd_tree;
  	
	status = 0;
	cmd_tree = NULL;
	while (status != 1)
	{
		line = ft_gets();
		if (!ft_strncmp("EXIT", line, 4))
		{
			status = 1;
			//clean program function needed
		}
		else
		{
			//validate syntax
			cmd_tree = parser(line, prog_info);
			print_tree(cmd_tree, 0);
			if (cmd_tree)
				executer(cmd_tree, prog_info);
			reset_program(prog_info, &cmd_tree);
		}
		if (line)
			free(line);
	}
	return (0);
	// exit(EXIT_SUCCESS);
}

int	set_shell_level(t_eggcarton *prog_info)
{
	char	*level;
	char	*new_level;
	int		int_level;

	level = ht_get(prog_info->environment, "SHLVL");
	if (!level)
	{
		level = ft_itoa(1);
		if (!level)
			return (ERROR);
		if (ht_add(prog_info->environment, "SHLVL", level) == ERROR)
			return (ERROR);
		return (SUCCESS);
	}
	int_level = ft_atoi(level) + 1;
	new_level = ft_itoa(int_level);
	if (!new_level)
		return (ERROR);
	return (ht_update_value(prog_info->environment, "SHLVL", new_level));
}

int	initialize_eggcarton(t_eggcarton *prog_info, char **envp)
{
	prog_info->environment = ht_create(150);
	if (prog_info->environment == NULL)
		return (ERROR);
	initialize_env_table(prog_info->environment, envp);
	if (initalize_command_table(prog_info) == ERROR)
		return (EXIT_FAILURE); //clean exit
	prog_info->cmd_count = 0;
	prog_info->pipe_count = 0;
	prog_info->og_env = envp;
	prog_info->pipes = NULL;
	prog_info->pids = NULL;
	prog_info->children = NULL;
	if (set_shell_level(prog_info) == ERROR)
		printf("SHELL LEVEL NOT SET!");
	return (SUCCESS);
}

int main(int argc, char **argv, char **envp)
{
	t_eggcarton		prog_info;
	(void) argc;
	(void) argv;
	initialize_eggcarton(&prog_info, envp);
	eggshell(&prog_info);
	ht_destroy(&(prog_info.environment));
	ht_destroy(&(prog_info.command_table));
	return (0);
	// exit(EXIT_SUCCESS);
}