/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 10:35:22 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/21 13:49:11 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_ast.h"
#include "ft_hash.h"

/*get_paths takes in the environment variable array and returns an array of 
 *paths from the PATHS variable in the environment
 */
char	**get_paths(t_eggcarton *prog_info)
{
	int		i;
	char	*paths;
	char	**split_paths;

	i = 0;

	split_paths = NULL;
	paths = ht_get(prog_info->environment, "PATH");
	if (paths)
		split_paths = ft_split(paths, ':');
	return (split_paths);
}

/*find_correct_path takes in a program name and an array of paths from the 
 *environment and returns the path to the program if it exists in 
 *one of the paths, if it doesn't exist it will return NULL
 */
char	*find_correct_path(char *fname, char **paths)
{
	int		i;
	char	*path;
	char	*slash_fname;

	i = 0;
	slash_fname = ft_strjoin("/", fname);
	while (paths[i] && slash_fname)
	{
		path = ft_strjoin(paths[i], slash_fname);
		if (path)
		{
			if (access(path, X_OK) == 0)
			{
				free(slash_fname);
				return (path);
			}
			else
				free(path);
		}
		i++;
	}
	if (slash_fname)
		free(slash_fname);
	return (NULL);
}

char	*get_path(t_eggcarton *prog_info, char *fname)
{
	char	*path;
	char	**paths;

	path = ht_get(prog_info->command_table, fname);
	if (!path)
	{
		if (access(fname, X_OK) == 0)
			path = ft_strdup(fname);
		else
		{
			paths = get_paths(prog_info);
			path = find_correct_path(fname, paths);
			clean_str_array(paths);
		}
		if (path)
			ht_add(prog_info->command_table, fname, path);
		//free the paths
	}
	return (path); //might be null, just check for that later
}

//may need to destroy the table in event of error
int	initalize_command_table(t_eggcarton *prog_info)
{
	prog_info->command_table = ht_create(30);
	if (!prog_info->command_table)
		return (ERROR);
	if (ht_add(prog_info->command_table, "echo", ft_strdup(":")) == ERROR)
		return (ERROR);
	if (ht_add(prog_info->command_table, "cd", ft_strdup(":")) == ERROR)
		return (ERROR);
	if (ht_add(prog_info->command_table, "pwd", ft_strdup(":")) == ERROR)
		return (ERROR);
	if (ht_add(prog_info->command_table, "export", ft_strdup(":")) == ERROR)
		return (ERROR);
	if (ht_add(prog_info->command_table, "unset", ft_strdup(":")) == ERROR)
		return (ERROR);
	if (ht_add(prog_info->command_table, "env", ft_strdup(":")) == ERROR)
		return (ERROR);
	if (ht_add(prog_info->command_table, "exit", ft_strdup(":")) == ERROR)
		return (ERROR);
	return (SUCCESS);
}