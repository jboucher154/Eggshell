#include "minishell.h"
#include "ft_hash.h"

static int	only_digits(char *to_check)
{
	int	i;

	i = 0;
	if (to_check[0] == '-' && ft_isdigit(to_check[1]))
		i++;
	while (ft_isdigit(to_check[i]) && to_check[i] != '\0')
		i++;
	if (to_check[i] != '\0')
		return (FALSE);
	return (TRUE);
}

void	clean_and_restore(t_eggcarton *prog_info)
{
	if (prog_info->children )
		free_children(prog_info->children);
	ht_destroy(&(prog_info->environment));
	ht_destroy(&(prog_info->command_table));
	clean_str_array(prog_info->array_env);
	if (prog_info->pipes)
		free(prog_info->pipes);
	tcsetattr(STDIN_FILENO, TCSANOW, prog_info->saved_term);
}

void	exit_command(t_eggcarton *prog_info, t_child *cmd)
{
	unsigned char	exit_code;

	exit_code = 0;
	if (cmd->args[1] != '\0' && cmd->args[2] == '\0')
	{
		if (!only_digits(cmd->args[1]))
		{
			print_error("Exit: numeric argument required");
			exit_code = 255;
		}
		else
			exit_code = ft_atoi(cmd->args[1]);
	}
	else if (cmd->args[2] != '\0')
	{
		print_error("Too many arguments");
		ht_update_value(prog_info->environment, "?", ft_strdup("1"));
		return ;
	}
	else
		exit_code = ft_atoi(ht_get(prog_info->environment, "?"));
	if (exit_code == 0)
		printf("Exit🐥\n");
	if (exit_code != 0)
	{
		printf("Exit🍳\n");
		printf("%d\n", exit_code);
	}
	clean_and_restore(prog_info);
	// while (1)
	// 	 ;
	exit (exit_code);
}
