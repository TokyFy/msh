/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:11:15 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 15:36:07 by sranaivo         ###   ########.fr       */
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

void	_false(void)
{
	char	**args;

	args = (char *[2]){"false", NULL};
	execvp("false", args);
}

int	ft_execvp(const char *__file, char *const __argv[])
{
	execvp(__file, __argv);
	ft_putstr_fd((char *)__file, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	_false();
	return (0);
}
