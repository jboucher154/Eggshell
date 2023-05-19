/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_hashtable.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 17:46:40 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 13:55:00 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hash.h"

/*
 * add_joined_key_values takes in a hash_item and adds the key and value to the
 * export_array and a char* that is the key, "=", and value joined together.
 * Keys with a "?" as the first character or NULL values
 * are not added to the export_array.
 */
static void	add_joined_key_values(t_hash_item *to_join, int *export_index, \
char **export_array)
{
	while (to_join)
	{
		if (to_join->key[0] != '?' && to_join->value != NULL)
		{
			export_array[*export_index] = ft_strjoin_three(to_join->key, \
			"=", to_join->value);
			if (!export_array[*export_index])
			{
				while (*export_index <= 0)
				{
					(*export_index)--;
					free(export_array[*export_index]);
					return ;
				}
			}
			(*export_index)++;
		}
		to_join = to_join->next;
	}
}

/*
 * ht_export_to_array takes in a hash_table and returns an array of strings
 * that are the key, "=", and value joined together. The array is NULL
 * terminated.
 */
char	**ht_export_to_array(t_hash_table *hash_table)
{
	char	**export_array;
	int		export_index;
	size_t	index;

	index = 0;
	export_index = 0;
	export_array = ft_calloc((hash_table->filled + 1), sizeof(char *));
	if (!export_array)
		return (NULL);
	export_array[hash_table->filled] = NULL;
	while (index < hash_table->size)
	{
		if (hash_table->table[index])
		{
			add_joined_key_values(hash_table->table[index], &export_index, \
			export_array);
			if (export_index == 0)
			{
				free(export_array);
				return (NULL);
			}
		}
		index++;
	}
	return (export_array);
}
