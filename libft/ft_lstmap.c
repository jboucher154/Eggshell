/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 12:42:03 by jebouche          #+#    #+#             */
/*   Updated: 2023/01/16 14:29:41 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*new_lst;

	if (!lst || !f || !del)
		return (NULL);
	head = NULL;
	new_lst = ft_lstnew(f(lst->data));
	if (!new_lst)
		return (NULL);
	ft_lstadd_back(&head, new_lst);
	lst = lst->next;
	while (lst)
	{
		new_lst = ft_lstnew(f(lst->data));
		if (!new_lst)
		{
			ft_lstclear(&head, del);
			return (NULL);
		}
		ft_lstadd_back(&head, new_lst);
		lst = lst->next;
	}
	return (head);
}
