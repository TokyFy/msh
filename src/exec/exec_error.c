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
#include <sys/stat.h>
#include <unistd.h>

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
	execve(args[0], (char **)args , NULL);
}
/*
int	ft_execvp(const char *__file, char *const __argv[])
{
	execvp(__file, __argv);
	ft_putstr_fd((char *)__file, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	_exit2(127);
	return (0);
}
*/

int	file_exists(const char *path)
{
	struct stat	buffer;

	if (stat(path, &buffer) == 0)
		return (1);
	return (0);
}

char	*path_join(char *path1, char *path2)
{
	char	*temp;
	char	*path;

	temp = ft_strjoin(path1, "/");
	path = ft_strjoin(temp, path2);
	free(temp);
	return (path);
}

char	*find_exec(const char *exec, char **path)
{
	char	*exec_path;

	exec_path = NULL;
	while (*path)
	{
		exec_path = path_join(*path, (char *)exec);
		if (file_exists(exec_path))
		{
			return (exec_path);
		}
		free(exec_path);
		path++;
	}
	return (NULL);
}

int	ft_execvp(const char *__file, char * __argv[])
{
	char	*path;
	char	**paths;
	char	*exec_path;

	exec_path = NULL;
	if(!ft_strchr(__file, '/'))
	{
		path = get_env(*static_env(NULL), "PATH");
		if(path)
		{
			paths = ft_split(path, ':');
			exec_path = find_exec(__file, paths);
		}
		if(!exec_path)
		{
				exec_path = path_join(".", (char*)__file);
		}
		__argv[0] = exec_path;
	}
	execve(__argv[0], __argv, NULL);
	perror(__file);
	_exit2(127);
	return -1;
}
