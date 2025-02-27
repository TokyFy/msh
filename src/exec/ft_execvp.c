/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antananariv>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:10:58 by franaivo          #+#    #+#             */
/*   Updated: 2024/12/12 22:10:00 by franaivo         ###   ########.fr       */
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

void	execvp_error(const char *__file)
{
	char		*error_str;
	struct stat	file_stat;

	if (ft_strcmp((char *)__file, ".") == 0)
	{
		ft_putstr_fd("filename argument required\n", STDERR_FILENO);
		_exit2(2);
	}
	if (!file_exists(__file))
	{
		error_str = ft_strjoin(__file, " : command not found\n");
		ft_putstr_fd(error_str, STDERR_FILENO);
		_exit2(127);
	}
	stat(__file, &file_stat);
	if (S_ISDIR(file_stat.st_mode))
	{
		error_str = ft_strjoin(__file, " : Is a directory\n");
		ft_putstr_fd(error_str, STDERR_FILENO);
		_exit2(126);
	}
	error_str = ft_strjoin(__file, " : Permission denied\n");
	ft_putstr_fd(error_str, STDERR_FILENO);
	_exit2(126);
}

int	ft_execvp(const char *__file, char *__argv[])
{
	char	*path;
	char	**paths;
	char	*exec_path;

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
	execvp_error(__file);
	return (-1);
}
