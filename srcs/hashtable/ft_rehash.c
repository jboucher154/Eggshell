/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rehash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:03:01 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 14:04:14 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hash.h"

/*
** This resizes the hash table when it is too full
** It returns SUCCESS if the table was resized
** It returns ERROR if an error occured
*/
static void	rehash_add(t_hash_table *table, t_hash_item *to_add)
{
	size_t		index;
	t_hash_item	*tmp;

	index = get_hash(to_add->key) % table->size;
	to_add->prev = NULL;
	to_add->next = NULL;
	tmp = table->table[index];
	if (tmp == NULL)
		table->table[index] = to_add;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = to_add;
		to_add->prev = tmp;
	}
}

/*
 * add_items_from_index takes in an index, a hash_table, and a dbl pointer to
 * a hash_item. It adds all the items from the old hash_table index to the new
 * hash_table.
 */
static void	add_items_from_index(int index, t_hash_table *table, \
t_hash_item **old_table)
{
	t_hash_item	*item;
	t_hash_item	*tmp;

	item = old_table[index];
	while (item)
	{
		tmp = item;
		item = item->next;
		rehash_add(table, tmp);
	}
}

/*
 * ht_rehash takes in a hash_table and resizes the hash_table to double the
 * previous size. All items from the old table are reassigned in the newtable
 * It returns SUCCESS if the table was resized and ERROR if an error
 * occured.
 */
size_t	ht_rehash(t_hash_table *table)
{
	t_hash_item	**new;
	size_t		i;
	t_hash_item	**old_table;

	table->size = table->size * 2;
	new = (t_hash_item **)ft_calloc(table->size + 1, sizeof(t_hash_item *));
	if (!new)
		return (ERROR);
	old_table = table->table;
	table->table = new;
	i = 0;
	while (i < table->size / 2)
	{
		add_items_from_index(i, table, old_table);
		i++;
	}
	free(old_table);
	table->table = new;
	return (SUCCESS);
}
