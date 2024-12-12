/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:05:16 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 15:15:28 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

int	ft_open(char *file, int oflags, int iflags)
{
	int	fd;

	fd = open(file, oflags, iflags);
	if (fd == -1)
	{
		perror(file);
		_exit2(EXIT_FAILURE);
	}
	return (fd);
}

pid_t	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	ft_pipe(int fds[2])
{
	if (pipe(fds) == -1)
	{
		perror("Pipe");
		exit(EXIT_FAILURE);
	}
}

void	ft_dup2(int old, int new)
{
	if (dup2(old, new) == -1)
	{
		perror("Dup");
		exit(EXIT_FAILURE);
	}
	return ;
}

void	ft_close(int fd)
{
	if (close(fd) == -1)
	{
		perror("Close");
		exit(EXIT_FAILURE);
	}
}
