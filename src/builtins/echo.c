/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 10:58:01 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/22 14:05:05 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

static int	has_n_option(char *arg)
{
	char	*tmp;
	int		i;

	tmp = handle_quote(arg);
	if (!tmp || tmp[0] != '-')
	{
		free(tmp);
		return (0);
	}
	i = 1;
	while (tmp[i] == 'n')
		i++;
	if (tmp[i] == '\0')
	{
		free(tmp);
		return (1);
	}
	free(tmp);
	return (0);
}

int	echo(t_cmd *cmd)
{
	int	option;
	int	i;

	option = 0;
	i = 1;
	while ((cmd->argv)[i] && has_n_option((cmd->argv)[i]))
	{
		option = 1;
		i++;
	}
	expand_main_loop(*(static_env(NULL)), &(cmd->argv));
	while ((cmd->argv)[i])
	{
		ft_putstr_fd((cmd->argv)[i], 1);
		if ((cmd->argv)[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!option)
		ft_putchar_fd('\n', 1);
	return (0);
}
