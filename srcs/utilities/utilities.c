/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 13:20:14 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/19 15:56:04 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * move_pointer_past_ws moves the pointer past any whitespace characters
 * in the string that is passed.
 */
void	move_pointer_past_ws(char **str_to_move)
{
	if (!str_to_move || !*str_to_move || **str_to_move == '\0')
		return ;
	while (**str_to_move && ft_strchr(WHITESPACE, **str_to_move))
		(*str_to_move)++;
}

/*
 * move_pointer_backwards_ws moves the pointer backwards past any whitespace
 * characters in the string that is passed.
 */
void	move_pointer_backwards_ws(char **str_to_move, char *dont_go_past)
{
	if (!str_to_move || !*str_to_move || **str_to_move == '\0' || !dont_go_past \
		|| dont_go_past == *str_to_move)
		return ;
	while (*str_to_move > dont_go_past && ft_strchr(WHITESPACE, **str_to_move))
		(*str_to_move)--;
}

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
char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3)
{
	char	*joined;
	size_t	len;

	if (!s1 || !s2)
		return (NULL);
	if (!s3)
		len = ft_strlen(s1) + ft_strlen(s2);
	else
		len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	joined = ft_calloc((len + 1), sizeof(char));
	if (joined)
	{
		joined[len] = '\0';
		fill_joined(joined, s1, s2, s3);
	}
	return (joined);
}

/*
 * ft_free_two frees two strings and sets them to NULL
 */
void	ft_free_two(char *to_free1, char *to_free2)
{
	if (to_free1)
	{
		free(to_free1);
		to_free1 = NULL;
	}
	if (to_free2)
	{
		free(to_free2);
		to_free2 = NULL;
	}
}
