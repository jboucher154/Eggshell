/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 13:20:14 by jebouche          #+#    #+#             */
/*   Updated: 2023/04/26 17:59:04 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	move_pointer_past_ws(char **str_to_move)
{
	if (!str_to_move || !*str_to_move || **str_to_move == '\0')
		return ;
	while (**str_to_move && ft_strchr(WHITESPACE, **str_to_move))
		(*str_to_move)++;// not usre if this needs to be checked
}