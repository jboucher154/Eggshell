/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   characters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 15:17:45 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:33:12 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"

int	print_char(t_legend **legend, va_list *list)
{
	int	padding;
	int	count;

	count = 0;
	padding = (*legend)->padding;
	if ((*legend)->dash[0] == 1)
	{
		padding = (*legend)->dash[1];
		ft_putchar_fd(va_arg(*list, int), 1);
		count += print_flag_char(' ', padding - 1) + 1;
	}
	else
	{
		if (padding > 0)
			count = print_flag_char(' ', padding - 1);
		ft_putchar_fd(va_arg(*list, int), 1);
		count++;
	}
	return (count);
}

static int	with_dash(t_legend **leg, int len, char *str, int *i)
{
	int	count;

	count = 0;
	(*leg)->dash[1] -= len;
	while (*i < len)
	{
		ft_putchar_fd(str[*i], 1);
		count++;
		(*i)++;
	}
	if ((*leg)->dash[1] >= 1)
		count += print_flag_char(' ', (*leg)->dash[1]);
	return (count);
}

int	print_string(t_legend **legend, va_list *list)
{
	char	*str;
	int		len;
	int		count;
	int		i;

	count = 0;
	i = 0;
	str = va_arg(*list, char *);
	if (!str)
		str = "(null)";
	len = ft_strlen(str);
	if (len > (*legend)->period[1] && (*legend)->period[0] == 1)
		len = (*legend)->period[1];
	if ((*legend)->dash[0])
		count += with_dash(legend, len, str, &i);
	else
	{
		count += print_flag_char(' ', (*legend)->padding - len);
		while (i < len)
		{
			ft_putchar_fd(str[i++], 1);
			count++;
		}
	}
	return (count);
}
