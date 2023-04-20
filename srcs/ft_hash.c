/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:43:42 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/20 14:57:31 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hash.h"
#include "../includes/libft.h"
#include "stdio.h"

/*
** These functions have not been tested yet
** They are not used in eggshell yet
** They are here for future use
*/

/*
** This function is used to get the hash of a string.
** It is used to get the index of the hash table.
** It is based on the djb2 algorithm.
** https://en.wikipedia.org/wiki/Djb2
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
** This function intialize a new hash table
** It takes the size of the hash table as a parameter
** It returns a pointer to the new hash table
** It returns NULL if an error occured
** It allocates 16 slots for the hash table and 1 for the NULL pointer
*/
t_hash_table	*ht_create(int size)
{
	t_hash_table	*table;
	int				i;

	table = (t_hash_table *)malloc(sizeof(t_hash_table));
	if (!table)
		return (NULL);
	table->table = (t_hash_item **)malloc(sizeof(t_hash_item *) * (size + 1));
	if (!table->table)
	{
		free(table);
		return (NULL);
	}
	i = 0;
	while(i <= size)
		table->table[i++] = NULL;
	table->size = size;
	table->filled = 0;
	return (table);
}

/*
** This function is used destroy the hash table
** It takes a dbl pointer to the hash table as a parameter
** It frees all the memory used by the hash table
** It sets the pointer to the hash table to NULL
*/
void	ht_destroy(t_hash_table **hash_table)
{
	size_t	i;
	t_hash_item		**tmp_tbl;
	t_hash_item		*item;
	t_hash_item		*tmp;

	i = 0;
	tmp_tbl = (*hash_table)->table;
	while (tmp_tbl && (*hash_table)->size > 0 && i < (*hash_table)->size)
	{
		item = tmp_tbl[i];
		while (item)
		{
			tmp = item;
			item = item->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			tmp = NULL;
		}
		i++;
	}
	free((*hash_table)->table);
	(*hash_table)->table = NULL;
	free(*hash_table);
	(*hash_table) = NULL;
}

/*
** This function allocates a new hash item
** It returns a pointer to the new hash item
** It returns NULL if an error occured
** Value passed must be an allocated pointer
*/
t_hash_item	*new_hash_item(const char *key, void *value)
{
	t_hash_item	*item;
	
	if (!value)
		return (NULL);
	item = (t_hash_item *)malloc(sizeof(t_hash_item));
	if (!item)
		return NULL;
	item->key = ft_strdup(key);
	if (!item->key)
	{
		free(item);
		return NULL;
	}
	item->value = value;
	item->next = NULL;
	item->prev = NULL;
	return (item);
}

/*
** This function is used to add a new item to the hash table
** It returns SUCCESS if the item was added
** It returns ERROR if an error occured
*/
int	ht_add(t_hash_table *table, const char *key, void *value)
{
	t_hash_item	*item;
	size_t		index;
	t_hash_item	*tmp;
	int			rehash_res;

	rehash_res = 0;
	if (table->filled >= table->size / 2)
	{
		rehash_res = ht_rehash(table);
		if (rehash_res == ERROR)
			return (ERROR);
	}
	item = new_hash_item(key, value);
	if (!item)
		return (ERROR);
	index = get_hash(key) % table->size;
	tmp = table->table[index];
	if (tmp)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = item;
		item->prev = tmp;
	}
	else
		table->table[index] = item;
	table->filled++;
	return (SUCCESS);
}

/*
** This function is used to remove an item from the hash table
** It returns SUCCESS if the item was removed
** It returns ERROR if an error occured
//should it return the value of the item ?
*/
int	ht_remove(t_hash_table *table, const char *key)
{
	size_t		index;
	t_hash_item	*item;

	index = get_hash(key) % table->size;
	item = table->table[index];
	while (item)
	{
		if (ft_strncmp(item->key, key, ft_strlen(item->key)) == 0)
		{
			if (item->prev)
				item->prev->next = item->next;
			if (item->next)
				item->next->prev = item->prev;
			if (item == table->table[index])
				table->table[index] = item->next;
			free(item->key);
			free(item->value);
			free(item);
			table->filled--;
			return (SUCCESS);
		}
		item = item->next;
	}
	return (ERROR);
}

/*
** This resizes the hash table when it is too full
** It returns SUCCESS if the table was resized
** It returns ERROR if an error occured
*/
void	rehash_add(t_hash_table *table , t_hash_item *to_add)
{
	size_t		index;
	t_hash_item	*tmp;
	// int			rehash_res;

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
	t_hash_item	**new_table;
	t_hash_item	*item;
	t_hash_item	*tmp;
	size_t		i;
	t_hash_item **old_table;

	table->size = table->size * 2;
	new_table = (t_hash_item **)ft_calloc(table->size + 1, sizeof(t_hash_item *));
	if (!new_table)
		return (ERROR);
	old_table = table->table;
	table->table = new_table;
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
	table->table = new_table;
	return (SUCCESS);
}
			// tmp = item;
			// item = item->next;
			// tmp->next = NULL;
			// tmp->prev = NULL;
			// ht_add(table, tmp->key, tmp->value);
			// free(tmp->key);
			// // free(tmp->value);
			// free(tmp);
			// tmp = NULL;

/*
** This function is used to get an item from the hash table
** It returns a pointer to the item value if it was found
** It returns NULL if the item was not found
*/
void	*ht_get(t_hash_table *table, const char *key)
{
	size_t		index;
	t_hash_item	*item;

	index = get_hash(key) % table->size;
	item = table->table[index];
	while (item)
	{
		if (ft_strncmp(item->key, key, ft_strlen(item->key)) == 0)
			return (item->value);
		item = item->next;
	}
	return (NULL);
}

//update value
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

//print hash table
void	ht_print(t_hash_table *table)
{
	t_hash_item	*to_print;
	size_t		index;
	size_t		printed;
	
	index = 0;
	printed = 0;
	while (index < table->size && printed < table->filled)
	{
		// printf("print index: %i\n", index);
		to_print = table->table[index];
		while (to_print != NULL)
		{
			printf("%s=%s\n", to_print->key, (char *)to_print->value);
			to_print = to_print->next;
			printed++; //may not be needed
		}
		index++;
	}
}