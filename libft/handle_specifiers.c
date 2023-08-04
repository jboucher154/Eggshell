/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_specifiers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 15:14:26 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:33:39 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"

int	handle_per_specifier(t_legend **legend, va_list *list)
{
	if ((*legend)->specifier == '%')
		return (print_percent(legend));
	if ((*legend)->specifier == 'c')
		return (print_char(legend, list));
	if ((*legend)->specifier == 's')
		return (print_string(legend, list));
	if ((*legend)->specifier == 'd' || (*legend)->specifier == 'i')
		return (print_int_dec(legend, list));
	if ((*legend)->specifier == 'u')
		return (print_unsigned(legend, list));
	if ((*legend)->specifier == 'x' || (*legend)->specifier == 'X')
		return (print_hex(legend, list));
	if ((*legend)->specifier == 'p')
	{
		(*legend)->hash = 1;
		return (print_hex(legend, list));
	}
	return (0);
}

int	is_format_specifier(char c)
{
	if (c == 'c' || c == 's' || c == 'd' || c == 'i' || c == 'u' || c == 'x' \
	|| c == 'X' || c == 'p' || c == '%')
		return (1);
	else
		return (0);
}
