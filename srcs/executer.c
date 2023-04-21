/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 10:44:20 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/21 11:49:06 by jebouche         ###   ########.fr       */
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
	int ret;

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

int	create_pipes(t_eggcarton *prog_info)
{
	char *error;
	int	count;
	int	pipe_ret;

	prog_info->pipes = malloc(sizeof (int) * (prog_info->pipe_count * 2));
	if (!prog_info->pipes)
	{	
		printf("%sEggShell🥚: malloc failed%s\n", RED, KNRM);
		return (ERROR);
	}
	pipe_ret = 0;
	count = 0;
	while (count < prog_info->pipe_count && pipe_ret != -1)
	{
		pipe_ret = pipe(prog_info->pipes + (count * 2));
		count++;
	}
	if (pipe_ret == -1)
	{
		error = strerror(errno);
		printf("%sEggShell🥚:%s%s\n", RED, error, KNRM);
	}
	return (SUCCESS);
}

t_child	*new_child(void)
{
	t_child	*child;

	child = (t_child *) malloc(sizeof(t_child));
	if (!child)
		return (NULL);
	child->path = NULL;
	child->args = NULL;
	child->redir_in = -1;
	child->redir_out = -1;
	child->pipe_in = -1;
	child->pipe_out = -1;
	child->fd_to_close = -1; //
	return (child);
}

void clean_up_children(t_child **children)
{
	int	index;
	int arg_index;

	index = 0;
	while(children[index] && children[index]->args)
	{
		arg_index = 0;
		while(children[index]->args[arg_index])
		{
			free(children[index]->args[arg_index]);
			arg_index++;
		}
		free(children[index]->args);
		index++;
	}
	free_children(children);
}	
void	free_children(t_child **children)
{
	int	index;

	index = 0;
	while (children[index])
	{
		free(children[index]);
		index++;
	}
	free(children);
	children = NULL;
}

int	create_child_array(t_eggcarton *prog_info)
{
	t_child	**children;
	int		index;
	
	children = (t_child **) malloc(sizeof(t_child *) * (prog_info->cmd_count + 1));
	if (!children)
		return (ERROR);
	index = 0;
	children[prog_info->cmd_count] = NULL;
	while(index < prog_info->cmd_count)
	{
		children[index] = new_child();
		if (!children[index])
		{
			free_children(children);
			return (ERROR);
		}
		index++;
	}
	prog_info->children = children;
	return (SUCCESS);
}

void	setup_redirection(t_redirection *redirection, t_eggcarton *prog_info, int index)
{
	int	fd;
	char *error;
	check_for_expansions(prog_info, &(redirection->filename));
	fd = -1;
	if (redirection->token_id == REDIRECT_IN)
	{
		fd = open(redirection->filename, O_RDONLY);
		prog_info->children[index]->redir_in = fd;
	}
	else if (redirection->token_id == REDIRECT_OUT)
	{
		fd = open(redirection->filename, O_WRONLY|O_CREAT|O_TRUNC);
		prog_info->children[index]->redir_out = fd;
	}
	else if (redirection->token_id == REDIRECT_OUT_APPEND)
	{
		fd = open(redirection->filename, O_WRONLY|O_CREAT);
		prog_info->children[index]->redir_out = fd;
	}
	if (fd == -1) 
	{
		error = strerror(errno);
		printf("%sEggShell🥚:%s%s\n", RED, error, KNRM);
	}
}

void	setup_pipes(t_eggcarton *prog_info, int index)
{
		int	read_index;
		int	write_index;
		
		write_index = index * 2 + 1;
		read_index = index - 1 * 2;
		if (index == 0)
		{
			prog_info->children[index]->pipe_in = STDIN_FILENO;
			prog_info->children[index]->pipe_out = prog_info->pipes[1];

		}
		else if (index == prog_info->cmd_count)
		{
			read_index = index - 1 * 2;
			prog_info->children[index]->pipe_in = prog_info->pipes[read_index];
			prog_info->children[index]->pipe_out = STDOUT_FILENO;	
		}
		else
		{
			prog_info->children[index]->pipe_in = prog_info->pipes[read_index];
			prog_info->children[index]->pipe_out = prog_info->pipes[write_index];
		}
		
		//do we need to check if there will be an out?
		// do these after we fork
		// dup2(prog_info->pipes[read_index], 0); //reroute pipe contents to stdin
		// dup2(prog_info->pipes[write_index], 1); //reroute stdout to outfile
		// close(prog_info->pipes[write_index]); //close write end of pipe
}

char	*quote_cut(char *to_check_str)
{
	char *new;
	
	new = ft_substr(to_check_str, 1, ft_strlen(to_check_str) - 1);
	if (!new)
	{
		printf("%sEggShell🥚: malloc failed%s\n", RED, KNRM);
		return (NULL);
	}
	return (new);
}

void	insert_new_value(char *str, char *new_str, char *variable, char *value)
{
	int		value_index;
	int		new_index;
	int		old_index;

	value_index = 0;
	new_index = 0;
	old_index = 0;
	while (str[old_index])// should this be new string ?
	{
		if (str[old_index] == '$') // should tigger copying of variable value to the new string
		{
			while (value && value[value_index])
			{
				new_str[new_index] = value[value_index];
				value_index++;
				new_index++;
			}
			old_index += ft_strlen(variable);
			break ;
		}
		else
			new_str[new_index++] = str[old_index++];
	}
	printf("CHECKING old str index content: %c\n", str[old_index]);//
	while (str[old_index])//should pick up after the variable name, finnish copying
	{
		new_str[new_index++] = str[old_index++];
	}
	new_str[new_index] = '\0';
	printf("CHECKING STRING AFTER VALUE INSERT: %s\n", new_str);//
}

 //exands the variable
char	*expand_env_var(t_eggcarton *prog_info, char *str, char *variable_start)
{
	char	*variable;
	char	*value;
	char	*new_str;

	printf("CHECKING STRING: %s\n", str);//
	if (variable_start == NULL)
		return (str);
	variable =  ft_substr(variable_start, 0, find_end_word(variable_start));
	value  = ht_get(prog_info->environment, variable + 1);
	new_str = (char *) malloc(sizeof(char) * (ft_strlen(str) - ft_strlen(variable) + ft_strlen(value) + 1));
	if (!new_str)
	{
		free (variable);
		printf("%sEggShell🥚: variable expansion failed%s\n", RED, KNRM);
		return (str); //return the origional string incase of failure
	}
	printf("STR: %s , VARIABLE: %s, VALUE: %s\n", str, variable, value);
	insert_new_value(str, new_str, variable, value);
	printf("CHECKING STRING AFTER VALUE INSERT: %s\n", new_str);//
	free (variable);
	free (str);
	return (expand_env_var(prog_info, new_str, ft_strchr(new_str, '$')));
	//iterate and copy str & value
	//return str
	//check if $present if '$VAR' '"$VAR"' do not expand

}

//search for needed expansions
int	check_for_expansions(t_eggcarton *prog_info, char **to_check_arr)
{
	int		index;
	char	*variable_start;
	char	*temp;
	//to_check = "file name, single string" OR arguments;
	index = 0;
	printf("CHECKING ARRAY: \n");//
	print_array(to_check_arr);//
	while (to_check_arr[index])
	{
		variable_start = ft_strchr(to_check_arr[index], '$');
		printf("VARIABLE START ASSESSED\n");//
		if (variable_start && to_check_arr[index][0] != '\'')
		{
			to_check_arr[index] = expand_env_var(prog_info, to_check_arr[index], variable_start);
			if (to_check_arr[index] == NULL)
				return (ERROR); //
		}
		if (ft_strchr(QUOTES, to_check_arr[index][0]))
		{
			temp = quote_cut(to_check_arr[index]);//
			free(to_check_arr[index]);
			to_check_arr[index] = temp; //shoudl we bail here if the malloc failed?
		}
		index++;
	}
	return (SUCCESS);
}

void	setup_child(t_executable_cmd *cmd, t_eggcarton *prog_info, int index)
{
	if (prog_info->pipe_count != 0)
	{
		setup_pipes(prog_info, index);
	}
	prog_info->children[index]->args = cmd->args; //clean in child not cmd tree
	printf("ARRAY BEFORE EXPANSION\n");
	print_array(prog_info->children[index]->args); //test
	if (check_for_expansions(prog_info, prog_info->children[index]->args) == ERROR)
		printf("%sEggShell🥚: expansion error%s\n", RED, KNRM); //continue??
	printf("ARRAY AFTER EXPANSION\n");
	print_array(prog_info->children[index]->args); //test
	prog_info->children[index]->path = get_path(prog_info, cmd->args[0]);
}

int	tree_iterator(t_cmd *cmd, t_eggcarton *prog_info, int *index)//index for all the ararys
{
	if (*index == prog_info->cmd_count)
		return (SUCCESS);
	if (cmd->type == PIPE_CMD)
	{
		//left
		tree_iterator((t_cmd *)((t_pipe *)cmd)->left, prog_info, index);
		//increment index
		(*index)++;
		//right
		tree_iterator((t_cmd *)((t_pipe *)cmd)->right, prog_info, index);
		//return (tree_iterator(left) == SUCCESS && tree_iterator(right) == SUCCESS)?
	}
	else if (cmd->type == REDIRECTION_CMD)
	{
		//assign to current index in children?
		setup_redirection((t_redirection *)cmd, prog_info, *index);
		tree_iterator((t_cmd *)((t_redirection *)cmd)->cmd, prog_info, index);
	}
	if (cmd->type == EXECUTABLE_CMD)
	{
		//setup the command stuffs to current index
		setup_child((t_executable_cmd *)cmd, prog_info, *index);
	}
	return (SUCCESS);//?
}

void	executer(t_cmd *cmd, t_eggcarton *prog_info)
{
	int *pids;
	int index;

	index = 0;
//	prog_info->pipes = create_pipes(prog_info);
	printf("EGGCARTON info: cmd count: %i, pipe count: %i\n", prog_info->cmd_count, prog_info->pipe_count);
	if (create_pipes(prog_info) == ERROR)
	{
		printf("%sEggShell🥚: pipe creation failed%s\n", RED, KNRM);
		return ;
	}
	pids = malloc(sizeof(int) * prog_info->cmd_count); //ft_calloc?
	if (!pids)
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
	clean_up_children(prog_info->children);
}








// void	executer(t_cmd *cmd, t_eggcarton *prog_info, int depth)
// {
	
// 	if (cmd->type == LINE)
// 	{
// 		printf("LINE found, error: %i\n", depth);
// 	}
// 	else if (cmd->type == REDIRECTION_CMD)
// 	{
// 		do_redirection((t_redirection *)cmd, prog_info); //
// 		executer((t_cmd *)(((t_redirection *)cmd)->cmd), prog_info, depth + 1);
// 		printf("REDIRECTION found, depth: %i\n", depth);
// 	}
// 	else if (cmd->type == PIPE_CMD)
// 	{
// 		setup_pipe(((t_pipe *)cmd), prog_info);//
// 		executer((t_cmd *)(((t_pipe *)(cmd))->left), prog_info, depth + 1);
// 		executer((t_cmd *)(((t_pipe *)(cmd))->right), prog_info, depth + 1);
// 		printf("PIPE found, depth: %i\n", depth);
// 	}
// 	else if (cmd->type == EXECUTABLE_CMD)
// 	{
// 		do_command((t_executable_cmd *)cmd, prog_info);//
// 		printf("EXECUTABLE CMD found: %s, depth: %i\n", ((t_executable_cmd *)cmd)->args[0] ,depth);
// 		print_array(((t_executable_cmd *)cmd)->args);
// 	}
// 	else
// 	{
// 		printf("SOMETHING IS WRONG, depth: %i\n", depth);
// 	}
// }