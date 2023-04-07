/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:48:27 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:31:03 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

char	*ft_strdup(const char *s1)
{
	char	*copy;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(s1);
	copy = (char *) malloc(len + 1);
	if (copy)
	{
		copy[len] = '\0';
		while (i <= len)
		{
			copy[i] = s1[i];
			i++;
		}	
	}
	return (copy);
}
