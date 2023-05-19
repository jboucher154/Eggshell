/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:02:18 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 10:47:31 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hash.h"

//print hash table for enviroment
void	ht_print_env(t_hash_table *table)
{
	t_hash_item	*to_print;
	size_t		index;
	size_t		printed;

	index = 0;
	printed = 0;
	while (index < table->size && printed < table->filled)
	{
		to_print = table->table[index];
		while (to_print != NULL)
		{
			if (to_print->key[0] != '?' && to_print->value)
			{
				ft_putstr_fd(to_print->key, 1);
				ft_putchar_fd('=', 1);
				ft_putstr_fd((char *)to_print->value, 1);
				ft_putchar_fd('\n', 1);
			}
			to_print = to_print->next;
			printed++;
		}
		index++;
	}
}
				// printf("%s=%s\n", to_print->key, (char *)to_print->value);

static void	print_export_value(t_hash_item *to_print)
{
	ft_putstr_fd("=\"", 1);
	ft_putstr_fd((char *)to_print->value, 1);
	ft_putchar_fd('\"', 1);
}

void	ht_print_export(t_hash_table *table)
{
	t_hash_item	*to_print;
	size_t		index;
	size_t		printed;

	index = 0;
	printed = 0;
	while (index < table->size && printed < table->filled)
	{
		to_print = table->table[index];
		while (to_print != NULL)
		{
			if (to_print->key[0] != '?')
			{
				ft_putstr_fd("declare -x ", 1);
				ft_putstr_fd(to_print->key, 1);
				if (to_print->value != NULL)
					print_export_value(to_print);
				ft_putchar_fd('\n', 1);
			}
			to_print = to_print->next;
			printed++;
		}
		index++;
	}
}

				// printf("declare -x %s\n", to_print->key);
				// printf("declare -x %s=\"%s\"\n", to_print->key, \
				// (char *)to_print->value);