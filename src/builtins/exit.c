/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 09:48:04 by sranaivo          #+#    #+#             */
/*   Updated: 2024/12/13 08:35:10 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

int	is_valit_exit(char *exit)
{
	char		*str;
	long double	return_exit;

	if (!*exit)
		return (0);
	exit = ft_strtrim(exit, " \t");
	return_exit = ft_atold(exit);
	if (return_exit > LLONG_MAX || return_exit < LLONG_MIN)
		return (0);
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
	char	**arg;

	arg = cmd->argv;
	ft_putstr_fd("exit\n", STDERR_FILENO);
	arg++;
	if (cmd->argv[1])
	{
		while (*arg)
		{
			if (!is_valit_exit(*arg++))
			{
				ft_putstr_fd("msh: exit: numeric argument required\n",
					STDERR_FILENO);
				_exit2(2);
			}
		}
		if (cmd->argv[2])
		{
			ft_putstr_fd("msh: exit: too many arguments\n", STDERR_FILENO);
			return (2);
		}
		_exit2(ft_atoi(cmd->argv[1]));
	}
	_exit2(WEXITSTATUS(get_status()));
	return (0);
}
