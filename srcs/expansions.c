/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 15:46:58 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/02 13:25:26 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy_from_value(char *new_str, int *new_index, char *value)
{
	int	value_index;

	value_index = 0;
	while (value && value[value_index])
	{
		new_str[*new_index] = value[value_index];
		value_index++;
		(*new_index)++;
	}
}

void	insert_new_value(char *str, char *new_str, char *variable, char *value)
{
	int		new_index;
	int		old_index;

	new_index = 0;
	old_index = 0;
	while (str[old_index])
	{
		if (str[old_index] == '$')
		{
			copy_from_value(new_str, &new_index, value);
			old_index += ft_strlen(variable);
			break ;
		}
		else
			new_str[new_index++] = str[old_index++];
	}
	while (str[old_index])
	{
		new_str[new_index++] = str[old_index++];
	}
	new_str[new_index] = '\0';
}

int	find_end_of_var(char *varaible_start)
{
	int	index;

	index = 1;
	while (varaible_start[index] == '_' || ft_isalpha(varaible_start[index]) || \
	ft_isdigit(varaible_start[index]))
		index++;
	return (index);
}

static size_t	new_str_len(char *str, char *variable, char *value)
{
	if (value == NULL)
		return (ft_strlen(str) - ft_strlen(variable));
	else
		return (ft_strlen(str) - ft_strlen(variable) + ft_strlen(value));
}

//exands the variable
char	*expand_env_var(t_eggcarton *prog_info, char *str, char *variable_start)
{
	char	*variable;
	char	*value;
	char	*new_str;

	if (variable_start == NULL)
		return (str);
	variable = ft_substr(variable_start, 0, find_end_of_var(variable_start));
	if (variable == NULL)
	{
		print_error("malloc failed during expansion");
		return (str);
	}
	value = ht_get(prog_info->environment, variable + 1);
	new_str = (char *) malloc(sizeof(char) * \
	(new_str_len(str, variable, value) + 1));
	if (!new_str)
	{
		free(variable);
		print_error("variable expansion failed");
		return (str);
	}
	insert_new_value(str, new_str, variable, value);
	free(variable);
	free(str);
	return (new_str);
}

static void	update_quote_info(t_quote_tracker *quote_info, int in_quote_value, \
char quote_type_value)
{
	quote_info->in_quote = in_quote_value;
	quote_info->quote_type = quote_type_value;
}

static int yes_expand(t_quote_tracker *q_info, char *to_assess, int index)
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
