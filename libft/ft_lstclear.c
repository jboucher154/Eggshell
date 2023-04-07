/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 12:40:21 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:29:11 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*temp;

	if (!lst || !(*lst))
		return ;
	temp = *lst;
	while (temp)
	{
		(*lst) = (*lst)->next;
		ft_lstdelone(temp, del);
		temp = *lst;
	}
}
