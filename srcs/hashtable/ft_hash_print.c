/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:02:18 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/02 18:43:52 by jebouche         ###   ########.fr       */
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
				printf("%s=%s\n", to_print->key, (char *)to_print->value);
			to_print = to_print->next;
			printed++;
		}
		index++;
	}
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
				if (to_print->value == NULL)
					printf("declare -x %s\n", to_print->key);
				else
					printf("declare -x %s=\"%s\"\n", to_print->key, \
					(char *)to_print->value);
			}
			to_print = to_print->next;
			printed++;
		}
		index++;
	}
}
