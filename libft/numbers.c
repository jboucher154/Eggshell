/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numbers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:12:14 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:34:00 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"

static int	print_sign(int plus, long int num)
{
	int	count;

	count = 0;
	if (plus == 1 && num >= 0)
		count += print_flag_char('+', 1);
	else if (num < 0)
	{
		count += print_flag_char('-', 1);
	}
	return (count);
}

static int	print_if_non_zero(t_legend **leg, long int num, int len)
{
	if (!(num == 0 && (*leg)->period[0] == 1 && (*leg)->period[1] == 0))
	{
		ft_putnbr_nosign(num, 1);
		return (len);
	}
	return (0);
}

static int	print_pad_n_num(t_legend **leg, int len, long int num, int z_pad)
{
	int		sp_pad;
	int		count;
	int		flag_holder;

	flag_holder = 0;
	count = 0;
	if ((*leg)->plus == 1 || (*leg)->space == 1 || num < 0)
		flag_holder = 1;
	if ((*leg)->padding > 0 && (*leg)->zero == 0)
	{
		if (num == 0 && (*leg)->period[0] == 1 && (*leg)->period[1] == 0)
			sp_pad = (*leg)->padding - (flag_holder + z_pad);
		else
		sp_pad = (*leg)->padding - (flag_holder + len + z_pad);
		count += print_flag_char(' ', sp_pad);
	}
	if ((*leg)->space == 1 && num >= 0)
		count += print_flag_char(' ', 1);
	count += print_sign((*leg)->plus, num);
	count += print_flag_char('0', (z_pad));
	count += print_if_non_zero(leg, num, len);
	sp_pad = (*leg)->dash[1] - (count);
	if ((*leg)->dash[0] == 1 && (*leg)->dash[1] > 0)
		count += print_flag_char(' ', sp_pad);
	return (count);
}

int	print_int_dec(t_legend **legend, va_list *list)
{
	int			count;
	long int	num;
	int			z_pad;
	int			len;

	num = (long int) va_arg(*list, int);
	if (num < 0)
		len = get_num_len_long(num * -1);
	else
		len = get_num_len_long(num);
	z_pad = get_zpad(legend, len);
	if ((*legend)->zero == 1 && num < 0)
		z_pad--;
	count = print_pad_n_num(legend, len, num, z_pad);
	return (count);
}
