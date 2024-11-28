/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:11:15 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 15:52:44 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <msh.h>
#include <stdio.h>
#include <string.h>

void	perrorexit(const char *error)
{
	perror(error);
	exit(1);
}

void	ft_waitpid(pid_t pid, int *status, int op)
{
	if (waitpid(pid, status, op) == -1)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
}

void	_exit2(int status)
{
	const char	*args[3];
	args[0] = shell_path(NULL);
	args[1] = ft_itoa(status);
	args[2] = NULL;
	execvp(args[0], (char**)args);
}

int	ft_execvp(const char *__file, char *const __argv[])
{
	execvp(__file, __argv);
	ft_putstr_fd((char *)__file, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	_exit2(127);
	return (0);
}
