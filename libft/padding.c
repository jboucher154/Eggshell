/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 10:55:48 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:34:05 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"

static int	specifier_specific(t_legend **leg)
{
	if ((*leg)->specifier == 'd' || (*leg)->specifier == 'i')
	{
		if (((*leg)->plus == 1 || (*leg)->space == 1)
			&& (*leg)->zero == 1)
			return (-1);
	}
	else if ((*leg)->specifier == 'x' || (*leg)->specifier == 'X' \
		|| (*leg)->specifier == 'p')
	{
		if ((*leg)->hash == 1 && (*leg)->zero == 1)
			return (-2);
	}
	return (0);
}

int	get_zpad(t_legend **legend, int len)
{
	int	z_pad;

	if ((*legend)->period[0] == 0 && (*legend)->zero == 0)
		return (0);
	z_pad = (*legend)-> padding;
	if ((*legend)->period[0] == 1)
		z_pad = (*legend)->period[1];
	z_pad += specifier_specific(legend);
	if (z_pad < len)
		z_pad = 0;
	else
		z_pad -= len;
	return (z_pad);
}
