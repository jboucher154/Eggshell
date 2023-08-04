/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_base.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:35:13 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:33:51 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"

int	get_num_len_u(unsigned long n, int base)
{
	unsigned long	i;
	unsigned long	ub;

	i = 0;
	if (base > 0)
		ub = (unsigned long) base;
	else
		return (0);
	if (n < ub)
		return (1);
	else
	{
		while (n >= ub)
		{
			n = n / ub;
			i++;
		}
	}
	return (i + 1);
}

char	*ft_itoa_ubase(unsigned long num, int base)
{
	char		*str_num;
	int			len;
	int			to_add;

	len = get_num_len_u(num, base);
	str_num = (char *) malloc(sizeof(char) * (len + 1));
	if (str_num)
	{
		str_num[len--] = '\0';
		if (num == 0)
			str_num[0] = '0';
		while (num > 0)
		{
			to_add = (num % base);
			if (to_add <= 9)
				str_num[len--] = to_add + 48;
			else
				str_num[len--] = to_add + 87;
			num = num / base;
		}
	}
	return (str_num);
}
