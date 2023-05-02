#include "ft_hash.h"

/*
** This function allocates a new hash item
** It returns a pointer to the new hash item
** It returns NULL if an error occured
** Value passed must be an allocated pointer
*/
t_hash_item	*new_hash_item(const char *key, void *value)
{
	t_hash_item	*item;

	// if (!value) //I want null values added
	// 	return (NULL);
	item = (t_hash_item *)malloc(sizeof(t_hash_item));
	if (!item)
		return (NULL);
	item->key = ft_strdup(key);
	if (!item->key)
	{
		free(item);
		return (NULL);
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
	if (ht_get(table, key) != NULL)
	{
		return (ht_update_value(table, key, value));//integrated update into add
	}
	if (table->filled >= table->size / 2)
	{
		rehash_res = ht_rehash(table);
		if (rehash_res == ERROR)
			return (ERROR);
	}
	item = new_hash_item(key, value);
	if (!item)//
		printf("MALLOC ERROR IN HT_ADD!!!!!!\n");//
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
			if (item->value)
				free(item->value);
			free(item);
			table->filled--;
			return (SUCCESS);
		}
		item = item->next;
	}
	return (ERROR);
}
