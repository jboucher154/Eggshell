/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:00:46 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:33:22 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"

int	apply_hex_prefix(char s)
{
	if (s == 'X')
	{
		ft_putchar_fd('0', 1);
		ft_putchar_fd('X', 1);
	}
	else
	{
		ft_putchar_fd('0', 1);
		ft_putchar_fd('x', 1);
	}
	return (2);
}

int	print_flag_char(char c, int repeat)
{
	int	i;

	i = 0;
	while (i < repeat)
	{
		ft_putchar_fd(c, 1);
		i++;
	}
	if (repeat < 0)
		return (0);
	return (repeat);
}

void	check_ignores(t_legend **legend)
{
	if ((*legend)->specifier == '%')
	{
		(*legend)->space = 0;
		(*legend)->plus = 0;
		(*legend)->hash = 0;
		(*legend)->period[0] = 0;
		return ;
	}
	if ((*legend)->dash[0] == 1 || (*legend)->period[0] == 1)
		(*legend)->zero = 0;
	if ((*legend)->dash[0] == 1)
	{
		if ((*legend)->dash[1] == 0)
		{
			(*legend)->dash[1] = (*legend)->padding;
			(*legend)->padding = 0;
		}
		else
			(*legend)->padding = 0;
	}
	if ((*legend)->plus == 1)
		(*legend)->space = 0;
}

int	is_flag(char c)
{
	if (c == '-' || c == '0' || c == '.' || c == '#' || c == ' ' || c == '+')
		return (1);
	else
		return (0);
}
