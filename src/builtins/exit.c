/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 09:48:04 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/26 11:06:56 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

int	msh_exit(t_cmd *cmd)
{
	int		exit_code;
	t_list	**env;

	exit_code = 0;
	env = static_env(NULL);
	free_env(*env);
	if ((cmd->argv)[1])
		exit_code = ft_atoi((cmd->argv)[1]);
	free_ast((void *)cmd);
	exit(exit_code);
}
