/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_hashtable.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 17:46:40 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/04 13:55:06 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hash.h"

/*
 * fill_joined fills the allocated joined string with the passed strings. 
 * If the third string is NULL, it will only fill the joined string with
 * the first two strings.
*/
static	void	fill_joined(char *joined, char const *s1, char const *s2, \
char const *s3)
{
	int		i;
	char	*temp;

	i = 0;
	temp = joined;
	while (s1[i] != '\0')
	{
			*temp++ = s1[i];
			i++;
	}
	i = 0;
	while (s2[i] != '\0')
	{
			*temp++ = s2[i];
			i++;
	}
	i = 0;
	if (s3)
	{
		while (s3[i] != '\0')
		{
			*temp++ = s3[i];
			i++;
		}
	}
}

/*
 * ft_strjoin_three joins three strings together. It returns the joined string
 * on success and NULL on failure. If the third string is NULL, it will join
 * only the first two strings.
 */
static char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3)
{
	char	*joined;
	size_t	len;

	if (!s1 || !s2)
		return (NULL);
	if (!s3)
		len = ft_strlen(s1) + ft_strlen(s2);
	else
		len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	joined = (char *) malloc(sizeof(char) * (len + 1));
	if (joined)
	{
		joined[len] = '\0';
		fill_joined(joined, s1, s2, s3);
	}
	return (joined);
}

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

char	**ht_export_to_array(t_hash_table *hash_table)
{
	char	**export_array;
	int		export_index;
	size_t	index;

	index = 0;
	export_index = 0;
	// export_array = (char **) malloc(sizeof(char *) * (hash_table->filled + 1));
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
