/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:47:22 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:30:02 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	if (n < 1)
		return (0);
	while (n - 1 > 0 && *str1 == *str2)
	{
		str1++;
		str2++;
		n--;
	}
	return (((unsigned char) *str1) - ((unsigned char) *str2));
}
