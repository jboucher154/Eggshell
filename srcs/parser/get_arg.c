/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_arg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:07:17 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/03 09:53:49 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_quote(char *quoteless, t_quote_tracker *quote_info, \
char *expanded_string, int index)
{
	if (expanded_string[index] && quote_info->in_quote == FALSE && \
		ft_strchr(QUOTES, expanded_string[index]))
	{
		quote_info->in_quote = TRUE;
		quote_info->quote_type = expanded_string[index];
	}
	else if (expanded_string[index] && quote_info->in_quote == TRUE && \
		ft_strchr(&(quote_info->quote_type), expanded_string[index]) == NULL)
	{
		quoteless[quote_info->quote_index] = expanded_string[index];
		(quote_info->quote_index)++;
	}
	else if (quote_info->in_quote == TRUE && \
		ft_strchr(&(quote_info->quote_type), expanded_string[index]))
	{
		quote_info->in_quote = FALSE;
		quote_info->quote_type = UNSET;
	}
	else
	{
		quoteless[quote_info->quote_index] = expanded_string[index];
		(quote_info->quote_index)++;
	}
}

char	*remove_quotes(char *expanded_string)
{
	char			*quoteless;
	int				index;
	t_quote_tracker	quote_info;

	index = 0;
	quoteless = ft_calloc((ft_strlen(expanded_string) + 1), sizeof(char));
	quote_info.quote_type = UNSET;
	quote_info.quote_index = 0;
	quote_info.in_quote = FALSE;
	while (expanded_string[index])
	{
		parse_quote(quoteless, &quote_info, expanded_string, index);
		index++;
	}
	return (quoteless);
}

char	*find_end(char **parsed_string)
{
	int		in_quote;
	char	*end;
	char	quote_type;

	in_quote = FALSE;
	end = *parsed_string;
	quote_type = UNSET;
	while (end != NULL)
	{
		if (end == NULL || (ft_strchr(WHITESPACE, *end) && in_quote == FALSE) \
		|| (ft_strchr("<>|", *end) && in_quote == FALSE))
			return (end);
		else if (ft_strchr(QUOTES, *end) && in_quote == FALSE)
		{
			in_quote = TRUE;
			quote_type = *end;
		}
		else if (ft_strchr(&quote_type, *end) && in_quote == TRUE)
		{
			in_quote = FALSE;
		}
		end++;
	}
	return (end);
}	

//returns allocated char * that is stored in arg array
char	*get_arg(char **parsed_string, t_eggcarton *prog_info)
{
	char	*arg;
	char	*end;

	if (!*parsed_string)
		return (NULL);
	end = find_end(parsed_string);
	arg = ft_substr(*parsed_string, 0, end - *parsed_string);
	arg = check_for_expansions(prog_info, arg);
	arg = remove_quotes(arg);
	(*parsed_string) += end - *parsed_string;
	return (arg);
}
