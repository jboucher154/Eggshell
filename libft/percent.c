/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   percent.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 16:05:02 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:34:11 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"

static int	print_pad_n_per(t_legend **legend, int z_pad)
{
	int	sp_pad;
	int	count;

	count = 0;
	if ((*legend)->padding > 0 && (*legend)->zero == 0)
	{
		sp_pad = (*legend)->padding - (1 + z_pad);
		count += print_flag_char(' ', sp_pad);
	}
	count += print_flag_char('0', z_pad);
	ft_putchar_fd('%', 1);
	count++;
	sp_pad = (*legend)->dash[1] - (count);
	if ((*legend)->dash[0] == 1 && (*legend)->dash[1] > 0)
		count += print_flag_char(' ', sp_pad);
	return (count);
}

int	print_percent(t_legend **legend)
{
	int	count;
	int	z_pad;

	z_pad = get_zpad(legend, 1);
	count = print_pad_n_per(legend, z_pad);
	return (count);
}
