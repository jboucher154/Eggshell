/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hexadecimal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:58:43 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:33:45 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"

static void	hex_to_caps(char **str)
{
	int	i;

	i = 0;
	while ((*str)[i] != '\0')
	{
		if (ft_isalpha((*str)[i]))
		{
			(*str)[i] = ft_toupper((*str)[i]);
		}
		i++;
	}
}

static int	print_pad_n_string(t_legend **leg, int l, char *h_str, int z_pad)
{
	int		sp_pad;
	int		count;

	count = 0;
	if ((*leg)->padding > 0 && (*leg)->zero == 0)
	{
		if (!(ft_strncmp(h_str, "0", l)) && (*leg)->period[0] == 1 \
			&& (*leg)->period[1] == 0)
			sp_pad = (*leg)->padding - z_pad;
		else
			sp_pad = (*leg)->padding - (((*leg)->hash * 2) + l + z_pad);
		count += print_flag_char(' ', sp_pad);
	}
	if ((*leg)->hash == 1)
		count += apply_hex_prefix((*leg)->specifier);
	count += print_flag_char('0', (z_pad));
	if (!(ft_strncmp(h_str, "0", l) == 0 && (*leg)->period[0] == 1 \
		&& (*leg)->period[1] == 0))
	{
		ft_putstr_fd(h_str, 1);
		count += l;
	}
	if ((*leg)->dash[0] == 1 && (*leg)->dash[1] > 0)
		count += print_flag_char(' ', (*leg)->dash[1] - (count));
	return (count);
}

int	print_hex(t_legend **legend, va_list *list)
{
	int		count;
	char	*hex_str;
	int		len;
	int		z_pad;

	count = 0;
	if ((*legend)->specifier == 'p')
		hex_str = ft_itoa_ubase(va_arg(*list, unsigned long), 16);
	else
		hex_str = ft_itoa_ubase(va_arg(*list, unsigned int), 16);
	if ((*legend)->specifier == 'X')
		hex_to_caps(&hex_str);
	len = ft_strlen(hex_str);
	z_pad = get_zpad(legend, len);
	if (ft_strncmp(hex_str, "0", len) == 0 && (*legend)->specifier != 'p')
		(*legend)->hash = 0;
	count += print_pad_n_string(legend, len, hex_str, z_pad);
	free (hex_str);
	return (count);
}
