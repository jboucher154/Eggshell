/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   legend.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 16:53:58 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:33:56 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"

static int	check_num(const char *str, int *index)
{
	int	sum;

	sum = 0;
	if (ft_isdigit(str[*index]))
	{
		while (ft_isdigit(str[*index]))
		{
			if (sum == 0)
				sum = str[*index] - 48;
			else
				sum = (sum * 10) + str[*index] - 48;
			(*index)++;
		}
	}
	return (sum);
}

static int	is_plus_hex_spc(char c, t_legend ***leg, int	*i)
{
	if (c == '#' || c == '+' || c == ' ')
	{
		if (c == '#')
			(**leg)->hash = 1;
		if (c == '+')
			(**leg)->plus = 1;
		if (c == ' ')
			(**leg)->space = 1;
		(*i)++;
		return (1);
	}
	return (0);
}

static void	eval_zero(t_legend ***leg, const char *str, int *i)
{
	if (str[*i] == '0' && *i == 0)
	{
		(**leg)->zero = 1;
		(*i)++;
	}
	else if (str[*i] == '0')
	{
		if (((**leg)->hash == 1 || (**leg)->plus == 1 || (**leg)->space == 1)
			&& (!(**leg)->dash[0] && !(**leg)->period[0]))
			(**leg)->zero = 1;
			(*i)++;
	}
}

static void	eval_check_dash(t_legend ***leg, const char *str, int *i)
{
	if (str[*i] == '.')
	{
		(**leg)->period[0] = 1;
		(*i)++;
		if (str[*i] == '#' || str[*i] == '+' || str[*i] == ' ')
			is_plus_hex_spc(str[*i], leg, i);
		if (ft_isdigit(str[*i]))
			(**leg)->period[1] = check_num(str, i);
	}
	else if (str[*i] == '-')
	{
		(**leg)->dash[0] = 1;
		(*i)++;
		if (str[*i] == '#' || str[*i] == '+' || str[*i] == ' ')
			is_plus_hex_spc(str[*i], leg, i);
		if (ft_isdigit(str[*i]))
			(**leg)->dash[1] = check_num(str, i);
	}
}

int	fill_legend(const char *str_leg, t_legend **leg)
{
	int	i;

	i = 0;
	if (!str_leg || !(*leg))
		return (0);
	while (str_leg[i] != '\0')
	{
		is_plus_hex_spc(str_leg[i], &leg, &i);
		if (str_leg[i] == '0')
			eval_zero(&leg, str_leg, &i);
		if (ft_isdigit(str_leg[i]) && (!(*leg)->dash[0] && !(*leg)->period[0]))
			(*leg)->padding = check_num(str_leg, &i);
		if (str_leg[i] == '.' || str_leg[i] == '-')
			eval_check_dash(&leg, str_leg, &i);
		if (is_format_specifier(str_leg[i]))
		{
			(*leg)->specifier = str_leg[i];
			break ;
		}
	}
	return (1);
}
