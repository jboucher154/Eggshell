/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:48:37 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:31:09 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

static	void	fill_joined(char *joined, char const *s1, char const *s2)
{
	int		i;
	char	*temp;

	i = 0;
	temp = joined;
	while (s1[i] != '\0')
	{
			*temp++ = s1[i];
			i++;
	}
	i = 0;
	while (s2[i] != '\0')
	{
			*temp++ = s2[i];
			i++;
	}
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	int		len;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	joined = (char *) malloc(sizeof(char) * (len + 1));
	if (joined)
	{
		joined[len] = '\0';
		fill_joined(joined, s1, s2);
	}
	return (joined);
}
