/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antananariv>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:14:13 by franaivo          #+#    #+#             */
/*   Updated: 2024/12/05 19:13:01 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

int	builtin_cd(t_cmd *cmd)
{
	char	*path;

	path = cmd->argv[1];
	if (!path)
		path = get_env(*static_env(NULL), "HOME");
	if (!path)
	{
		ft_putendl_fd("cd : Path required", STDERR_FILENO);
		return (1);
	}
	set_env("OLDPWD", get_env(*static_env(NULL), "PWD"));
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	path = getcwd(NULL, 0);
	set_env("PWD", path);
	free(path);
	return (0);
}
