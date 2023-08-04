/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:48:20 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:30:54 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

char	*ft_strchr(const char *s, int c)
{
	char			*temp;
	unsigned char	new_c;

	temp = (char *)s;
	new_c = c;
	while (*temp != new_c && s != NULL)
	{
		temp++;
		if (*temp == '\0' && new_c != '\0')
			return (NULL);
	}
	return (temp);
}
