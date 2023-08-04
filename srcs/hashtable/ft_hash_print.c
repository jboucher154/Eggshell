/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:02:18 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 13:59:40 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hash.h"

/*
 * ht_print_env takes in a hash_table and prints the key value pairs in the
 * hash_table to stdout. Keys with a "?" as the first character or NULL
 * values are not printed.
 */
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

/*
 * print_export_value takes in a hash_item and prints the value of the hash_item
 * to stdout formated for POSIX standard.
 */
static void	print_export_value(t_hash_item *to_print)
{
	ft_putstr_fd("=\"", 1);
	ft_putstr_fd((char *)to_print->value, 1);
	ft_putchar_fd('\"', 1);
}

/*
 * ht_print_export takes in a hash_table and prints the key value pairs in the
 * hash_table to stdout formated for POSIX standard. Keys with a "?" as the
 * first character are not printed.
 */
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
