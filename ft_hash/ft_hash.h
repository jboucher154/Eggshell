/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:44:51 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/11 16:56:33 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HASH_H
# define FT_HASH_H

# include "libft.h"

#define	SUCCESS	0
#define	ERROR	1

typedef struct	s_hash_item
{
	char				*key;
	void				*value;
	struct	s_hash_item	*next;
	struct	s_hash_item	*prev;
}				t_hash_item;

typedef struct	s_hash_table
{
	struct s_hash_item	**table;
	size_t				size;
	size_t				filled;
}				t_hash_table;

size_t			get_hash(const char *str);
t_hash_table	*ht_create(int size);
void 			ht_destroy(t_hash_table **table);
t_hash_item		*new_hash_item(const char *key, void *value);
int				ht_add(t_hash_table *table, const char *key, void *value);
int				ht_remove(t_hash_table *table, const char *key);
size_t 			ht_rehash(t_hash_table *table);
void			*ht_get(t_hash_table *table, const char *key);

#endif