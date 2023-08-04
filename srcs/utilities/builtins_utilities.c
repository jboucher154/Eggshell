/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 15:11:03 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 14:40:16 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_heredoc_name(int index)
{
	char	*int_str;
	char	*fname;

	int_str = ft_itoa(index);
	fname = ft_strjoin(HEREDOC_TEMP, int_str);
	free(int_str);
	return (fname);
}

/*
*  is_valid_var_name() checks if the key is a valid variable name according to
*  the POSIX standard. It returns TRUE if the key is valid, and FALSE if it is
*  not. It does not allow access to the '?' key.
*/
int	is_valid_var_name(char *key)
{
	size_t	index;

	index = 0;
	if (ft_isdigit(key[0]) || !key[0])
		return (FALSE);
	while (key[index] == '_' || ft_isalpha(key[index]) || \
	ft_isdigit(key[index]))
		index++;
	if (index == ft_strlen(key))
		return (TRUE);
	else
		return (FALSE);
}
