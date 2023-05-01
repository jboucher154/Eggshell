/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 10:44:20 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/01 19:17:44 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_builtins(t_child *cmd, t_eggcarton *prog_info)
{
		if (!ft_strncmp("pwd", cmd->args[0], 3) || !ft_strncmp("PWD", cmd->args[0], 3))
			pwd_command();
		if (!ft_strncmp("cd", cmd->args[0], 3))
			cd_command(cmd->args, prog_info->environment);
		if (!ft_strncmp("export", cmd->args[0], 6))
		{

			export_command(cmd->args, prog_info->environment);
		}
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
		if (cmd->pid == 0)
		{
			printf("I am a child and will exit now!\n");// leave for now
			exit(EXIT_SUCCESS);
		}
		printf("I'm not a child!\n");// leave for now
}

void	run_system_executable(t_executable_cmd *cmd, t_eggcarton *prog_info)
{
	int	ret;

	ret = execve(cmd->cmd_path, cmd->args, prog_info->og_env);
	if (ret == -1)
		exit_child("exeve failed: ", cmd->args[0], errno);
}


void	wait_for_children(t_eggcarton *prog_info)
{
	int	exit_status;
	int	index;
	
	index = 0;
	while (index < prog_info->pipe_count + 1)
	{
		if (prog_info->children[index]->pid >= 0)
			waitpid(prog_info->children[index]->pid , &exit_status, 0);
		index++;
	}
	//may need to update env variable based on exit status of last child
}

// void	wait_for_children(t_eggcarton *prog_info)
// {
// 	int	exit_status;
// 	int	index;
	
// 	index = 0;
// 	while (index < prog_info->pipe_count + 1)
// 	{
// 		if (prog_info->pids[index] >= 0)
// 			waitpid(prog_info->pids[index], &exit_status, 0);
// 		index++;
// 	}
// 	//may need to update env variable based on exit status of last child
// }


void	exit_child(char *error_msg, char *arg, int exit_code)
{
	ft_putstr_fd("\033[31mEggShell🥚: ", STDERR_FILENO);
	ft_putstr_fd(error_msg, STDERR_FILENO);
	ft_putendl_fd(arg, STDERR_FILENO);
	ft_putstr_fd("\x1B[0m", STDERR_FILENO);
	exit(exit_code);
}

/*bail_on_child is called by the child process if the command is not found.
 * It will exit with an error code.
 */
static void	bail_on_child(char *cmd)
{
	if (cmd == NULL)
		exit_child("command not found: ", " ", CMD_ERROR);
	else
		exit_child("command not found: ", cmd, CMD_ERROR);
}

/* pipe_child is called by child processes. It will dup2 the read and write
 * file descriptors to the correct file descriptors, close the file descriptors
 * that are not needed, and then execve the command. If the command is not
 * found, it will exit with an error code.
 */
void	pipe_child(t_eggcarton *prog_info, int index)
{
	if (prog_info->pipe_count > 0)
	{
		if (prog_info->children[index]->pipe_in != UNSET)
		{
			dup2(prog_info->children[index]->pipe_in, STDIN_FILENO); // in  == read
		}
		if (prog_info->children[index]->pipe_out != UNSET)
		{
			dup2(prog_info->children[index]->pipe_out, STDOUT_FILENO); // out == write
		}
	}
	close_pipes(prog_info->pipes, prog_info->pipe_count);
//then redirection dups
	if (prog_info->children[index]->redir_in != UNSET)
	{
		if (prog_info->children[index]->redir_in == OPEN_ERROR)
			write(STDIN_FILENO, "\0", 1);
		else
			dup2(prog_info->children[index]->redir_in, STDIN_FILENO);
	}
	if (prog_info->children[index]->redir_out != UNSET)
	{
		if (prog_info->children[index]->redir_in == OPEN_ERROR)
			ft_putstr_fd("OUTFILE OPEN ERROR, investigate proper course\n", 2);
		else
			dup2(prog_info->children[index]->redir_out, STDOUT_FILENO);
	}
//close all the fds
	close_redirections(prog_info->children[index]->redir_in, prog_info->children[index]->redir_out);
	if (prog_info->children[index]->command_present == FALSE) //may want to move this up more before the redirections are duped
		exit(0); //run redirection commands and exit before command checking and execution
	if (prog_info->children[index]->path == NULL)
		bail_on_child(prog_info->children[index]->args[0]);
	else if (prog_info->children[index]->redir_in == OPEN_ERROR || \
		prog_info->children[index]->redir_out == OPEN_ERROR) //targeted to redir in, should this apply to redir out as well?
		exit(1);//
	if (prog_info->children[index]->path[0] == ':')
		run_builtins(prog_info->children[index], prog_info);
	else
		execve(prog_info->children[index]->path, prog_info->children[index]->args, prog_info->og_env);
	exit_child("exeve failed: ", prog_info->children[index]->args[0], errno);
}

int	should_run_in_parent(t_eggcarton *prog_info, int index)
{
	if (prog_info->cmd_count == 1 && \
	(!ft_strncmp("cd", prog_info->children[index]->args[0], 2) || \
	!ft_strncmp("exit", prog_info->children[index]->args[0], 4) || \
	!ft_strncmp("unset", prog_info->children[index]->args[0], 5)))
		return (TRUE);
	else if (!ft_strncmp("export", prog_info->children[index]->args[0], 6) && \
		prog_info->children[index]->args[1] != NULL)
		return (TRUE);
	return (FALSE);
}

void	do_commands(t_eggcarton *prog_info)
{
	int		index;
	int 	current_pid;

	index = 0;
	current_pid = 0;
	print_children(prog_info->children);//
	while (index < prog_info->pipe_count + 1)
	{
		if (prog_info->children[index]->path && prog_info->children[index]->path[0] == ':')
		{
			if (should_run_in_parent(prog_info, index) == TRUE)
			{
				printf("I WILL RUN IN PARENT\n");//
				prog_info->children[index]->pid = -1;
				run_builtins(prog_info->children[index], prog_info);
			}
			else
			{
				prog_info->children[index]->pid = fork(); //child pid == 0
				if (prog_info->children[index]->pid == 0)
				{
					pipe_child(prog_info, index);//
					print_error("COMMAND DOES NOT EXIST, OR SOMETHING ELSE WAS WRONG");
					exit(1);//
				}
				else
					prog_info->children[index]->pid = current_pid;
			}
		}
		else
		{
			current_pid = fork(); //child pid == 0
			if (current_pid == 0)
			{
				pipe_child(prog_info, index);//
				print_error("COMMAND DOES NOT EXIST, OR SOMETHING ELSE WAS WRONG");
				exit(1);//
			}
			else
				prog_info->children[index]->pid = current_pid;
		}
		index++;
	}
	close_pipes(prog_info->pipes, prog_info->pipe_count);
	wait_for_children(prog_info);
}

// void	do_commands(t_eggcarton *prog_info)
// {
// 	int		index;
// 	int 	current_pid;

// 	index = 0;
// 	current_pid = 0;
// 	print_children(prog_info->children);//
// 	while (index < prog_info->pipe_count + 1)
// 	{
// 		// if built in, should it be a child process or not? else if, else etc. if PID = 0, is child and exit. if not 0.....return to next process
// 		if (prog_info->children[index]->path && prog_info->children[index]->path[0] == ':')
// 		{
// 			run_builtins(prog_info->children[index], prog_info);
// 			prog_info->pids[index] = -1;
// 		}
// 		else
// 		{

// 			current_pid = fork(); //child pid == 0
// 			if (current_pid == 0)
// 			{
// 				pipe_child(prog_info, index);//
// 				print_error("COMMAND DOES NOT EXIST, OR SOMETHING ELSE WAS WRONG");
// 				exit(1);//
// 			}
// 			else
// 				prog_info->pids[index] = current_pid;
// 		}
// 		index++;
// 	}
// 	close_pipes(prog_info->pipes, prog_info->pipe_count);
// 	wait_for_children(prog_info);
// }

void	executer(t_cmd *cmd, t_eggcarton *prog_info)
{
	int	index;

	index = 0;
	// print_tree(cmd, 0);//
	if (create_pipes(prog_info) == ERROR)
	{
		print_error("pipe creation failed");
		return ;
	}
	prog_info->pids = malloc(sizeof(int) * prog_info->cmd_count);
	if (!prog_info->pids)
	{	
		print_error("malloc failed");
		return ;
	}
	if (create_child_array(prog_info) == ERROR)
		print_error("ERROR");//printed the error in the array creation...
	tree_iterator(cmd, prog_info, &index);
	// print_children(prog_info->children);//
	do_commands(prog_info);
	free_children(prog_info->children);
}