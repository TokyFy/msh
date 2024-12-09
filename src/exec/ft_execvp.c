/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antananariv>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:10:58 by franaivo          #+#    #+#             */
/*   Updated: 2024/12/09 15:39:33 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

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

int	ft_execvp(const char *__file, char *__argv[])
{
	char	*path;
	char	**paths;
	char	*exec_path;
	char	*error_str;

	exec_path = NULL;
	if (!ft_strchr(__file, '/'))
	{
		path = get_env(*static_env(NULL), "PATH");
		if (path)
		{
			paths = ft_split(path, ':');
			exec_path = find_exec(__file, paths);
		}
		if (!exec_path)
			exec_path = path_join(".", (char *)__file);
		__argv[0] = exec_path;
	}
	execve(__argv[0], __argv, list_to_env_array(*static_env(NULL)));
	error_str = ft_strjoin(__file, " : command not found\n");
	ft_putstr_fd(error_str, STDERR_FILENO);
	_exit2(127);
	return (-1);
}
