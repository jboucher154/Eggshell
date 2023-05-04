/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_environment.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jebouche <jebouche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 14:37:52 by jebouche          #+#    #+#             */
/*   Updated: 2023/05/04 14:44:49 by jebouche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_environment_array(t_eggcarton *prog_info)
{
	char **new_enviornmnet;

	new_enviornmnet = ht_export_to_array(prog_info->environment);
	if (!new_enviornmnet)
	{
		print_error("malloc error: environment not updated");
		return ;
	}
	clean_str_array(prog_info->array_env);
	prog_info->array_env  = new_enviornmnet;
}