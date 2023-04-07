/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:49:20 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:32:13 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

static void	was_found(size_t *tracked, const char *needle)
{
	size_t	checked;

	checked = tracked[0] + ft_strlen(needle);
	tracked[0] = checked;
	tracked[1] = 1;
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*temp;
	size_t	tracked[2];

	temp = (char *) haystack;
	tracked[0] = -1;
	tracked[1] = 0;
	if (!*needle)
		return ((char *) haystack);
	if (len == 0)
		return (NULL);
	while (*temp && ++tracked[0] <= len)
	{
		if (*temp == *needle)
		{
			if (ft_strncmp(temp, needle, ft_strlen(needle)) == 0)
			{
				was_found(tracked, needle);
				break ;
			}
		}
		temp++;
	}
	if (tracked[0] > len || !tracked[1])
		return (NULL);
	return (temp);
}
