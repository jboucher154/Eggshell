/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:46:11 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:28:24 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*reserved;

	if (count == 0 || size == 0)
		return (ft_calloc(1, 1));
	if (count * size % size != 0 || count * size % count != 0)
		return (NULL);
	reserved = malloc(size * count);
	if (reserved)
		ft_bzero(reserved, count * size);
	return (reserved);
}
