/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 10:32:20 by franaivo          #+#    #+#             */
/*   Updated: 2024/12/16 10:32:21 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <msh.h>

const char	*shell_path(char **argv)
{
	static char	*path;
	char		*next_shlvl;
	char		*pwd;
	char		*shlvl;
	int			shell_level;

	pwd = NULL;
	if (!argv)
		return (path);
	shlvl = get_env(*static_env(NULL), "SHLVL");
	if (!shlvl)
		shlvl = ft_strdup("1");
	shell_level = ft_atoi(shlvl);
	next_shlvl = ft_itoa(shell_level + 1);
	set_env("SHLVL", next_shlvl);
	free(next_shlvl);
	pwd = getcwd(pwd, 0);
	path = ft_strjoin(pwd, argv[0] + 1);
	free(pwd);
	return (NULL);
}
