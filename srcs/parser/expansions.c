/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:46:58 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 14:23:00 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * update_quote_info takes in a pointer to a t_quote_tracker struct, an int
 * representing whether or not the current character is in a quote, and a char
 * representing the type of quote. It updates the t_quote_tracker struct with
 * the new information.
 */
static void	update_quote_info(t_quote_tracker *quote_info, int in_quote_value, \
char quote_type_value)
{
	quote_info->in_quote = in_quote_value;
	quote_info->quote_type = quote_type_value;
}

/*
 * yes_expand takes in a pointer to a t_quote_tracker struct, a pointer to the
 * string to assess, and an int representing the current index in the string.
 * It returns TRUE expansion criteria is met and FALSE if it is not. 
 */
static int	yes_expand(t_quote_tracker *q_info, char *to_assess, int index)
{
	if (((q_info->in_quote == TRUE && q_info->quote_type != '\'') || \
		q_info->in_quote == FALSE))
	{
		if (to_assess[(index) + 1] == '\0' || (to_assess[(index) + 1] && \
			(ft_strchr(WHITESPACE, to_assess[index + 1]) || \
			ft_strchr(QUOTES, to_assess[index + 1]))))
			return (FALSE);
		else
			return (TRUE);
	}
	return (FALSE);
}

/*
 * expand_env_var takes in pointers to the t_eggcarton struct, the string to
 * assess, the current index in the string, and a pointer to the end of the
 * string. It expands any environment variable and returns the new string.
 */
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
			to_assess = expand_env_var(prog_info, to_assess, to_assess + i, &i);
			if (to_assess == NULL)
				return (NULL);
			continue ;
		}
		i++;
	}
	return (to_assess);
}
