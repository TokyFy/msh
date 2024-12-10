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

#include <msh.h>

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
	if(WIFSIGNALED(status))
	{
		status = 128 + WEXITSTATUS(status);
	}
	args[1] = ft_itoa(WEXITSTATUS(status));
	args[2] = NULL;
	execve(args[0], (char **)args, NULL);
}
