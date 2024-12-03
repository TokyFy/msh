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

#include "libft.h"
#include <msh.h>
#include <unistd.h>

int is_valit_exit(char *exit)
{
	char *str = exit;

	while(*str == '+' || *str == '-')
	{
		str++;
	}

	if(!str)
		return 0;

	while(*str)
	{
		if(!ft_isdigit(*str))
			return 0;
		str++;
	}
	return 1;
}

int	msh_exit(t_cmd *cmd)
{
	ft_putstr_fd("exit\n" , STDERR_FILENO);
	if(cmd->argv[1])
	{
		if(cmd->argv[2])
			return 1;
		if(!is_valit_exit(cmd->argv[1]))
			_exit2(2);
		_exit2(ft_atoi(cmd->argv[1]));
	}
	_exit2(WEXITSTATUS(get_status()));
	return 0;
}
