/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:49:36 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:31:55 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

static int	get_start(char const *s1, char const *set)
{
	int	i;
	int	start;

	i = 0;
	start = 0;
	while (s1[start] != '\0' && set[i] != '\0')
	{
		if (s1[start] == set[i])
		{
			start++;
			i = 0;
		}
		else
			i++;
	}
	if (start == (int)ft_strlen(s1))
		start = 0;
	return (start);
}

static int	get_end(char const *s1, char const *set)
{
	int	i;
	int	end_index;

	i = 0;
	end_index = ft_strlen(s1) - 1;
	while (end_index >= 0 && set[i] != '\0')
	{
		if (s1[end_index] == set[i])
		{
			end_index--;
			i = 0;
		}
		else
			i++;
	}
	return (end_index);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	int		start;
	int		end;
	char	*trimmed;

	if (!s1 || !set)
		return (NULL);
	start = get_start(s1, set);
	end = get_end(s1, set) + 1;
	if (end - start == 0)
	{
		trimmed = (char *) malloc(sizeof(char) * (1));
		if (trimmed)
			trimmed[0] = '\0';
	}
	else
		trimmed = ft_substr(s1, start, end - start);
	return (trimmed);
}
