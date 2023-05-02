/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rehash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:03:01 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/02 16:03:05 by jebouche         ###   ########.fr       */
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

size_t	ht_rehash(t_hash_table *table)
{
	t_hash_item	**new;
	t_hash_item	*item;
	t_hash_item	*tmp;
	size_t		i;
	t_hash_item	**old_table;

	table->size = table->size * 2;
	new = (t_hash_item **)ft_calloc(table->size + 1, sizeof(t_hash_item *));
	new = (t_hash_item **)ft_calloc(table->size + 1, sizeof(t_hash_item *));
	if (!new)
		return (ERROR);
	old_table = table->table;
	table->table = new;
	i = 0;
	while (i < table->size / 2)//
	{
		item = old_table[i];
		while (item)
		{
			tmp = item;
			item = item->next;
			rehash_add(table, tmp);
		}
		i++;
	}
	free(old_table);
	table->table = new;
	return (SUCCESS);
}
