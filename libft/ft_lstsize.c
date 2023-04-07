/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 12:36:41 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:29:52 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

int	ft_lstsize(t_list *lst)
{
	int		len;
	t_list	*temp;

	len = 0;
	if (!lst)
		return (len);
	temp = lst;
	while (temp)
	{
		temp = temp->next;
		len++;
	}
	return (len);
}
