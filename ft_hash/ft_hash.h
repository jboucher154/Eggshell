/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:44:51 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/07 17:20:22 by jebouche         ###   ########.fr       */
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
t_hash_table;

#endif