/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gnl_join.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 09:29:17 by jebouche          #+#    #+#             */
/*   Updated: 2023/02/06 09:32:38 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	void	fill_joined(char *joined, char *s1, char *s2)
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

char	*ft_gnl_join(char *s1, char *s2)
{
	char	*joined;
	int		len;

	if (!s1 || !s2)
		return (NULL);
	if (s1[0] == '\0')
		len = ft_strlen(s2);
	else
		len = ft_strlen(s1) + ft_strlen(s2);
	joined = (char *) malloc(sizeof(char) * (len + 1));
	if (joined)
	{
		joined[len] = '\0';
		fill_joined(joined, s1, s2);
	}
	free (s1);
	free (s2);
	return (joined);
}
