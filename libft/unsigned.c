/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsigned.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 14:27:08 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:34:18 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"

static void	put_unsigned(unsigned int num)
{
	if (num > 9)
	{
		put_unsigned(num / 10);
		put_unsigned(num % 10);
	}
	else
	{
		ft_putchar_fd(num + 48, 1);
	}
}

static int	print_pad_n_un(t_legend **leg, int len, unsigned int n, int z_pad)
{
	int		sp_pad;
	int		count;

	count = 0;
	if ((*leg)->padding > 0 && (*leg)->zero == 0)
	{
		if (n == 0 && (*leg)->period[0] == 1 && (*leg)->period[1] == 0)
			sp_pad = (*leg)->padding - z_pad;
		else
			sp_pad = (*leg)->padding - (len + z_pad);
		count += print_flag_char(' ', sp_pad);
	}
	count += print_flag_char('0', (z_pad));
	if (!(n == 0 && (*leg)->period[0] == 1 && (*leg)->period[1] == 0))
	{
		put_unsigned(n);
		count += len;
	}
	sp_pad = (*leg)->dash[1] - (count);
	if ((*leg)->dash[0] == 1 && (*leg)->dash[1] > 0)
		count += print_flag_char(' ', sp_pad);
	return (count);
}

int	print_unsigned(t_legend **legend, va_list *list)
{
	int				count;
	unsigned int	num;
	int				z_pad;
	int				len;

	num = va_arg(*list, unsigned int);
	len = get_num_len_u(num, 10);
	z_pad = get_zpad(legend, len);
	count = print_pad_n_un(legend, len, num, z_pad);
	return (count);
}
