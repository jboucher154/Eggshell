/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 10:44:20 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 13:39:23 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
*  run_builtins() will direct the builtin command to the correct function.
*  If the command was run in a child process, the exit status will be used 
* 	to exit(). Otherwise, the exit status is updated in the '?' environment var.
*/
void	run_builtins(t_child *cmd, t_eggcarton *prog_info)
{
	int	exit_status;

	exit_status = 0;
	if (!ft_strncmp("pwd", cmd->args[0], 3) || \
	!ft_strncmp("PWD", cmd->args[0], 3))
		exit_status = pwd_command();
	if (!ft_strncmp("cd", cmd->args[0], 3))
		exit_status = cd_command(cmd->args, prog_info);
	if (!ft_strncmp("export", cmd->args[0], 6))
		exit_status = export_command(cmd->args, prog_info);
	if (!ft_strncmp("unset", cmd->args[0], 5))
		exit_status = unset_command(cmd->args, prog_info);
	if ((!ft_strncmp("echo", cmd->args[0], 4) || \
	!ft_strncmp("ECHO", cmd->args[0], 4)))
		exit_status = echo_command(cmd->args);
	if (!ft_strncmp("env", cmd->args[0], 3))
		exit_status = print_enviroment(prog_info->environment);
	if (!ft_strncmp("exit", cmd->args[0], 4))
		exit_command(prog_info, cmd);
	if (cmd->pid == 0)
		exit(exit_status);
	else
		ht_update_value(prog_info->environment, "?", ft_itoa(exit_status));
}

/*
*  wait_for_children() will wait for all child processes to finish and update
* the '?' environment variable with the exit status of the child processes.
* After all child processes have finished, echoctl_switch() is called 
* to restore the echo terminal settings.
*/
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

/*
*  should_run_in_parent() will return TRUE if the builtin command should be 
*  run in the parent process and FALSE if it should be run in a child process.
*/
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

/*
*  do_commands() determines if the command should be run in the parent process or
*  in a child process. The PIDs of the child processes are stored in the t_child
*  struct. Terminal echo settings are turned on for the child processes.
*  The parent closes all pipes and waits for all child processes to finish.
*/
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

/*
*  executer() is called by eggshell(). It creates the 
*  pipes and an array of child processes. The children are setup iwth the 
*  tree_iterator(). The command tree is freed after iterated. If a process 
*  ended by a signal, commands do not execute. All children are freed 
*  after execution.
*/
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
	if (prog_info->should_execute == TRUE)
		do_commands(prog_info);
	prog_info->should_execute = TRUE;
	free_children(prog_info->children);
}
