/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 10:44:20 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/03 15:20:10 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//line count will be okay once print statements gone
void	run_builtins(t_child *cmd, t_eggcarton *prog_info)
{
	if (!ft_strncmp("pwd", cmd->args[0], 3) || \
	!ft_strncmp("PWD", cmd->args[0], 3))
		pwd_command();
	if (!ft_strncmp("cd", cmd->args[0], 3))
		cd_command(cmd->args, prog_info);
	if (!ft_strncmp("export", cmd->args[0], 6))
		export_command(cmd->args, prog_info->environment);
	if (!ft_strncmp("unset", cmd->args[0], 5))
		unset_command(cmd->args, prog_info->environment);
	if ((!ft_strncmp("echo", cmd->args[0], 4) || \
	!ft_strncmp("ECHO", cmd->args[0], 4)))
		echo_command(cmd->args);
	if (!ft_strncmp("env", cmd->args[0], 3))
		print_enviroment(prog_info->environment);
	if (!ft_strncmp("exit", cmd->args[0], 4))
		exit_command(prog_info, cmd);
	if (cmd->pid == 0)
	{
		printf("I am a child and will exit now!\n");// leave for now
		exit(EXIT_SUCCESS);
	}
	printf("I'm not a child!\n");// leave for now
}

//may need to update env variable based on exit status of last child
void	wait_for_children(t_eggcarton *prog_info)
{
	int	exit_status;
	int	wexit;
	int	index;

	index = 0;
	while (index < prog_info->pipe_count + 1)
	{
		if (prog_info->children[index]->pid >= 0)
		{
			waitpid(prog_info->children[index]->pid, &exit_status, 0);
			wexit = WEXITSTATUS(exit_status);
			ht_update_value(prog_info->environment, "?", ft_itoa(wexit));
		}
		index++;
	}
	echoctl_switch(OFF);
}

int	should_run_in_parent(t_eggcarton *prog_info, int index)
{
	if (prog_info->children[index]->path && \
	prog_info->children[index]->path[0] == ':')
	{
		if (prog_info->cmd_count == 1 && \
		(!ft_strncmp("cd", prog_info->children[index]->args[0], 2) || \
		!ft_strncmp("exit", prog_info->children[index]->args[0], 4) || \
		!ft_strncmp("unset", prog_info->children[index]->args[0], 5)))
			return (TRUE);
		else if (!ft_strncmp("export", prog_info->children[index]->args[0], 6) \
		&& prog_info->children[index]->args[1] != NULL)
			return (TRUE);
		else
			return (FALSE);
	}
	return (FALSE);
}

void	do_commands(t_eggcarton *prog_info)
{
	int	index;

	index = 0;
	initialize_child_signals();
	while (index < prog_info->pipe_count + 1)
	{
		if (should_run_in_parent(prog_info, index) == TRUE)
		{
			prog_info->children[index]->pid = -1;
			run_builtins(prog_info->children[index], prog_info);
		}
		else
		{
			prog_info->children[index]->pid = fork();
			if (prog_info->children[index]->pid == 0)
			{
				echoctl_switch(ON);
				pipe_child(prog_info, index);
			}
		}
		index++;
	}
	close_pipes(prog_info->pipes, prog_info->pipe_count);
	wait_for_children(prog_info);
}

void	executer(t_cmd **cmd, t_eggcarton *prog_info)
{
	int	index;

	index = 0;
	if (create_pipes(prog_info) == ERROR)
	{
		print_error("pipe creation failed");
		return ;
	}
	if (create_child_array(prog_info) == ERROR)
		return ;
	tree_iterator(*cmd, prog_info, &index);
	clean_tree(*cmd);
	*cmd = NULL;
	do_commands(prog_info);
	free_children(prog_info->children);
}
