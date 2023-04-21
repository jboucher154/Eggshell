/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 10:44:20 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/21 15:48:13 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"

void	exit_child(char *error_msg, char *arg, int exit_code)
{
	printf("%sEggShell🥚: %s %s%s\n", RED, error_msg, arg, KNRM);
	exit(exit_code);
}

void	run_builtins(t_executable_cmd *cmd, t_eggcarton *prog_info)
{
		if (!ft_strncmp("pwd", cmd->args[0], 3) || !ft_strncmp("PWD", cmd->args[0], 3))
			pwd_command();
		if (!ft_strncmp("cd", cmd->args[0], 3))
			cd_command(cmd->args, prog_info->environment);
		if (!ft_strncmp("export", cmd->args[0], 6))
			export_command(cmd->args, prog_info->environment);
		if (!ft_strncmp("unset", cmd->args[0], 5))
			unset_command(cmd->args, prog_info->environment);
		if ((!ft_strncmp("echo", cmd->args[0], 4) || !ft_strncmp("ECHO", cmd->args[0], 4)))
			echo_command(cmd->args);
		if (!ft_strncmp("env", cmd->args[0], 3))
			print_enviroment(prog_info->environment);
		if (!ft_strncmp("CLEAR", cmd->args[0], 5))
			clearing();
		if (!ft_strncmp("EXIT", cmd->args[0], 4))
			exit(EXIT_SUCCESS);
}

void	run_system_executable(t_executable_cmd *cmd, t_eggcarton *prog_info)
{
	int	ret;

	ret = execve(cmd->cmd_path, cmd->args, prog_info->og_env);
	if (ret == -1)
		exit_child("exeve failed: ", cmd->args[0], errno);

}

void do_command(t_executable_cmd *cmd, t_eggcarton *prog_info)
{
	if (cmd->cmd_path[0] == ':')
		run_builtins(cmd, prog_info);
	else
		run_system_executable(cmd, prog_info);
		printf("system executables run here");
}


void	executer(t_cmd *cmd, t_eggcarton *prog_info)
{
	int	index;

	index = 0;
//	prog_info->pipes = create_pipes(prog_info);
	printf("EGGCARTON info: cmd count: %i, pipe count: %i\n", prog_info->cmd_count, prog_info->pipe_count);
	if (create_pipes(prog_info) == ERROR)
	{
		printf("%sEggShell🥚: pipe creation failed%s\n", RED, KNRM);
		return ;
	}
	prog_info->pids = malloc(sizeof(int) * prog_info->cmd_count); //ft_calloc?
	if (!prog_info->pids)
	{	
		printf("%sEggShell🥚: malloc failed%s\n", RED, KNRM);
		return ;
	}
	//make the array for commands
	if (create_child_array(prog_info) == ERROR)
		printf("ERROR\n");
		//cry
	printf("WE did it! %i\n", cmd->type);

	//iterate the tree and populate commands, open file descriptors
	tree_iterator(cmd, prog_info, &index);
	print_children(prog_info->children);
	//iterate over array of commands, forking and sending to child processes
	//parent process waits for all PIDs saved in array
	free_children(prog_info->children);
}