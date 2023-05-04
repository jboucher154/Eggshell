#include "minishell.h"
#include "ft_hash.h"

static int	only_digits(char *to_check)
{
	int	i;

	i = 0;
	while (ft_isdigit(to_check[i]) && to_check[i] != '\0')
		i++;
	if (to_check[i] != '\0')
		return (1);
	return (0);
}

void	clean_and_restore(t_eggcarton *prog_info)
{
	free_children(prog_info->children);
	ht_destroy(&(prog_info->environment));
	ht_destroy(&(prog_info->command_table));
	tcsetattr(STDIN_FILENO, TCSANOW, prog_info->saved_term);
}

void	exit_command(t_eggcarton *prog_info, t_child *cmd)
{
	int	exit_code;

	exit_code = 0;
	if (cmd->args[1] != '\0' && cmd->args[2] == '\0')
	{
		if (only_digits(cmd->args[1]))
			print_error("Exit: numeric argument required");
		else
			exit_code = ft_atoi(cmd->args[1]);
	}
	else if (cmd->args[2] != '\0')
	{
		print_error("Too many arguments");
		printf("exit🍳\n");
		return ;
	}
	else
		exit_code = ft_atoi(ht_get(prog_info->environment, "?"));
	if (exit_code == 0)
		printf("Exit🐥\n");
	else
		printf("Exit🍳\n");
	clean_and_restore(prog_info);
	exit (exit_code);
}
