/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:43:42 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 14:02:03 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hash.h"
#include "libft.h"

/*
 * This function is used to get the hash of a string.
 * It is used to get the index of the hash table.
 * It is based on the djb2 algorithm.
 */
size_t	get_hash(const char *str)
{
	size_t	hash;
	size_t	i;

	hash = 5381;
	i = 0;
	while (str[i])
	{
		hash = ((hash << 5) + hash) + str[i];
		i++;
	}
	return (hash);
}

/*
 * This function intialize a new hash table
 * It takes the size of the hash table as a parameter
 * It returns a pointer to the new hash table
 * It returns NULL if an error occured
 * It allocates 16 slots for the hash table and 1 for the NULL pointer
 */
t_hash_table	*ht_create(int size)
{
	t_hash_table	*table;
	int				i;

	table = ft_calloc(1, sizeof(t_hash_table));
	if (!table)
		return (NULL);
	table->table = ft_calloc((size + 1), sizeof(t_hash_item *));
	if (!table->table)
	{
		free(table);
		return (NULL);
	}
	i = 0;
	while (i <= size)
		table->table[i++] = NULL;
	table->size = size;
	table->filled = 0;
	return (table);
}

/*
 * ht_internal_get is used to get an item from the hash table and verify
 * that an item with that key exists.
*/
t_hash_item	*ht_internal_get(t_hash_table *table, const char *key)
{
	size_t		index;
	t_hash_item	*item;

	index = get_hash(key) % table->size;
	item = table->table[index];
	while (item)
	{
		if (ft_strncmp(item->key, key, ft_strlen(item->key)) == 0)
			return (item);
		item = item->next;
	}
	return (NULL);
}

/*
** This function is used to get an item from the hash table
** It returns a pointer to the item value if it was found
** It returns NULL if the item was not found and if the value is NULL
*/
void	*ht_get(t_hash_table *table, const char *key)
{
	t_hash_item	*item;

	item = ht_internal_get(table, key);
	if (item)
		return (item->value);
	return (NULL);
}

/*
 * ht_update_value is used to update the value of an item in the hash table.
 * It takes in a hash_table, a key, and a new_value. It returns SUCCESS if
 * the value was updated and ERROR if the value was not updated.
 */
size_t	ht_update_value(t_hash_table *table, const char *key, void *new_value)
{
	size_t		index;
	t_hash_item	*item;

	index = get_hash(key) % table->size;
	item = table->table[index];
	while (item)
	{
		if (ft_strncmp(item->key, key, ft_strlen(item->key)) == 0)
		{
			if (item->value)
				free(item->value);
			item->value = new_value;
			return (SUCCESS);
		}
		item = item->next;
	}
	return (ERROR);
}
