/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 11:48:44 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:31:26 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_end;
	size_t	i;
	size_t	src_len;
	size_t	dst_len;

	src_len = ft_strlen(src);
	if (dstsize == 0)
		return (src_len);
	dst_len = ft_strlen(dst);
	dst_end = dst_len;
	if (dst_end == dstsize)
		return (dst_end + src_len);
	i = 0;
	while (dst_end < dstsize - 1 && src[i] != '\0')
	{
		dst[dst_end] = src[i];
		i++;
		dst_end++;
	}
	if (dst_len < dstsize)
		dst[dst_end] = '\0';
	if (dst_len > dstsize)
		return (dstsize + src_len);
	return (dst_len + src_len);
}
