/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:48:10 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:30:48 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

static void	in_case_of_failure(char **to_free)
{
	int	i;

	i = 0;
	while (to_free[i])
	{
		free(to_free[i]);
		i++;
	}
	free(to_free);
}

static void	find_word(char const *s, char delim, int index, int *start_end)
{
	while (s[index] == delim && s[index] != '\0')
		index++;
	start_end[0] = index;
	while (s[index] != delim && s[index] != '\0')
		index++;
	start_end[1] = index;
}

static	int	get_size(char const *s, char c)
{
	int	size;
	int	i;

	size = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && s[i] != '\0')
			size++;
		while (s[i] != c && s[i] != '\0')
			i++;
		while (s[i] == c && s[i] != '\0')
			i++;
	}
	return (size);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		strt_end[2];
	int		k;

	if (!s)
		return (NULL);
	find_word(s, c, 0, strt_end);
	k = 0;
	split = (char **) malloc(sizeof(char *) * (get_size(s, c) + 1));
	if (split)
	{
		split[get_size(s, c)] = 0;
		while (k < get_size(s, c))
		{
			split[k] = ft_substr(s, strt_end[0], strt_end[1] - strt_end[0]);
			if (!split[k])
			{
				in_case_of_failure(split);
				return (NULL);
			}
			find_word(s, c, strt_end[1], strt_end);
			k++;
		}
	}
	return (split);
}
