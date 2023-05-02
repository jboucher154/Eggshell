/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:46:58 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/02 16:11:35 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_quote_info(t_quote_tracker *quote_info, int in_quote_value, \
char quote_type_value)
{
	quote_info->in_quote = in_quote_value;
	quote_info->quote_type = quote_type_value;
}

static int	yes_expand(t_quote_tracker *q_info, char *to_assess, int index)
{
	if (((q_info->in_quote == TRUE && q_info->quote_type != '\'') || \
	q_info->in_quote == FALSE) && (to_assess[(index) + 1] != '\0' || \
	(to_assess[(index) + 1] && \
	ft_strchr(WHITESPACE, to_assess[index + 1]) == NULL)))
	{
		return (TRUE);
	}
	return (FALSE);
}

//search for needed expansions
char	*check_for_expansions(t_eggcarton *prog_info, char *to_assess)
{
	int				i;
	t_quote_tracker	q_info;

	update_quote_info(&q_info, FALSE, UNSET);
	i = 0;
	if (ft_strchr(to_assess, '$') == NULL)
		return (to_assess);
	while (to_assess[i])
	{
		if (ft_strchr(QUOTES, to_assess[i]) && q_info.in_quote == FALSE)
			update_quote_info(&q_info, TRUE, to_assess[i]);
		else if (to_assess[i] == q_info.quote_type && \
		q_info.in_quote == TRUE)
			update_quote_info(&q_info, FALSE, UNSET);
		else if (to_assess[i] == '$' && yes_expand(&q_info, to_assess, i))
		{
			to_assess = expand_env_var(prog_info, to_assess, to_assess + i);
			if (to_assess == NULL)
				return (NULL);
			i = 0;
			continue ;
		}
		i++;
	}
	return (to_assess);
}
