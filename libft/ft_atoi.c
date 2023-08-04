/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:45:46 by jebouche          #+#    #+#             */
/*   Updated: 2022/11/08 13:33:33 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	overflowed(int sign)
{
	if (sign > 0)
		return (-1);
	else if (sign < 0)
		return (0);
	else
		return (1);
}

int	ft_atoi(const char *str)
{
	int			i;
	long int	sum;
	int			sign;

	i = 0;
	sum = 0;
	sign = 1;
	if (str[i] > 57 || !str[i])
		return (0);
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		sum = sum * 10 + (str[i] - 48);
		i++;
	}
	if (sum < 0)
		return (overflowed(sign));
	return (sum * sign);
}
