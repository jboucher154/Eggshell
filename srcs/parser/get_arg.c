/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_arg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:07:17 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 14:26:06 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * parse_quote takes in pointers to the quoteless string, the t_quote_tracker
 * struct, the string to assess and the current index in the string. It copies
 * the string to assess into the quoteless string, removing any quotes.
 */
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

/*
 * remove_quotes takes in a pointer to the string to assess. It removes any
 * quotes from the string and returns the new string.
 */
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
	free(expanded_string);
	return (quoteless);
}

/*
 * find_end takes in a pointer to the input string. It returns a pointer to the
 * end of the next argument in the input string.
 */
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

/*
 * get_arg takes in pointers to the input string, the t_eggcarton struct and an
 * int representing whether or not to expand the argument. It returns the next
 * argument in the input string with any expansions performed.
 */
char	*get_arg(char **parsed_string, t_eggcarton *prog_info, int expand)
{
	char	*arg;
	char	*end;

	if (!*parsed_string)
		return (NULL);
	end = find_end(parsed_string);
	arg = ft_substr(*parsed_string, 0, end - *parsed_string);
	if (expand == TRUE)
		arg = check_for_expansions(prog_info, arg);
	if (arg && arg[0] == '\0')
	{
		free(arg);
		arg = NULL;
	}
	else
		arg = remove_quotes(arg);
	(*parsed_string) += end - *parsed_string;
	return (arg);
}
