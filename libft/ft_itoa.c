/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 17:50:58 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:28:42 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

static int	get_num_len(long int n)
{
	int	i;

	i = 0;
	if (n < 0)
	{
		n = n * -1;
		i++;
	}
	if (n < 10)
		i++;
	else
	{
		while (n > 0)
		{
			n = n / 10;
			i++;
		}
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char		*str_num;
	int			len;
	long int	num;

	num = n;
	len = get_num_len(num);
	str_num = (char *) malloc(sizeof(char) * (len + 1));
	if (str_num)
	{
		str_num[len--] = '\0';
		if (num == 0)
			str_num[0] = '0';
		else if (num < 0)
		{
			str_num[0] = '-';
			num *= -1;
		}
		while (num > 0)
		{
			str_num[len--] = (num % 10) + 48;
			num = num / 10;
		}
	}
	return (str_num);
}
