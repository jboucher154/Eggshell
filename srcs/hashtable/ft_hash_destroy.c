/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_destroy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 16:01:58 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/02 16:02:12 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hash.h"

static void	free_hashtable(t_hash_table **hash_table)
{
	free((*hash_table)->table);
	(*hash_table)->table = NULL;
	free(*hash_table);
	(*hash_table) = NULL;
}

/*
** This function is used destroy the hash table
** It takes a dbl pointer to the hash table as a parameter
** It frees all the memory used by the hash table
** It sets the pointer to the hash table to NULL
*/
void	ht_destroy(t_hash_table **hash_table)
{
	size_t			i;
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
			if (tmp->value)
				free(tmp->value);
			free(tmp);
			tmp = NULL;
		}
		i++;
	}
	free_hashtable(hash_table);
}
