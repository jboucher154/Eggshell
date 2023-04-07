/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:28:31 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:33:17 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"

static t_legend	*new_legend(void)
{
	t_legend	*new;

	new = (t_legend *) malloc(sizeof(t_legend));
	if (new)
	{
		new->padding = 0;
		new->zero = 0;
		new->period[0] = 0;
		new->period[1] = 0;
		new->hash = 0;
		new->plus = 0;
		new->space = 0;
		new->dash[0] = 0;
		new->dash[1] = 0;
	}
	return (new);
}

static char	*get_legend(const char *str, unsigned int index, unsigned int *end)
{
	while (is_flag(str[index + (*end)]) || ft_isdigit(str[index + (*end)]))
		(*end)++;
	if (is_format_specifier(str[index + (*end)]))
		(*end)++;
	return (ft_substr(str, index, *end));
}

int	convert_print(const char *str, unsigned int i, va_list *lst, int **count)
{
	unsigned int	end;
	char			*s_leg;
	t_legend		*leg;

	end = 0;
	s_leg = get_legend(str, i, &end);
	leg = new_legend();
	if (leg)
	{
		fill_legend(s_leg, &leg);
		check_ignores(&leg);
		**count += handle_per_specifier(&leg, lst);
	}
	free(leg);
	free (s_leg);
	return (i + end);
}
