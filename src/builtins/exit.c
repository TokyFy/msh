/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 09:48:04 by sranaivo          #+#    #+#             */
/*   Updated: 2024/12/13 08:03:49 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

int	is_valit_exit(char *exit)
{
	char	*str;

	exit = ft_strtrim(exit, " \t");
	str = exit;
	while (*str == '+' || *str == '-')
		str++;
	if (!str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	msh_exit(t_cmd *cmd)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (cmd->argv[1])
	{
		if (cmd->argv[2])
		{
			ft_putstr_fd("msh: exit: too many arguments\n", STDERR_FILENO);
			return (2);
		}
		if (!is_valit_exit(cmd->argv[1]))
		{
			ft_putstr_fd("msh: exit: numeric argument required\n",
				STDERR_FILENO);
			_exit2(2);
		}
		_exit2(ft_atoi(cmd->argv[1]));
	}
	_exit2(WEXITSTATUS(get_status()));
	return (0);
}
