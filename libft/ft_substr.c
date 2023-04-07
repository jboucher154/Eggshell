/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:05:49 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:32:02 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

static	int	get_length(int strlen, int start, int len)
{
	int	remainder;

	remainder = strlen - start;
	if (remainder - len < 0)
		return (remainder);
	return (len);
}

static void	increment(int *i, unsigned int *start, int *size)
{
	*i = *i + 1;
	*start = *start + 1;
	*size = *size - 1;
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_string;
	int		i;
	int		size;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		size = 0;
	else
		size = get_length(ft_strlen(s), start, len);
	sub_string = (char *) malloc(sizeof(char) * (size + 1));
	i = 0;
	if (sub_string)
	{
		sub_string[size] = '\0';
		while (size > 0)
		{
			if (s[start] == '\0')
				sub_string[i] = '\0';
			sub_string[i] = s[start];
			increment(&i, &start, &size);
		}
	}
	return (sub_string);
}
