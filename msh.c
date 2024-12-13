/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:31:55 by franaivo          #+#    #+#             */
/*   Updated: 2024/12/13 08:01:23 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

volatile sig_atomic_t	g_signal_received;

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
	path = ft_strjoin(pwd, "/minishell");
	free(pwd);
	return (NULL);
}

int	main(const int argc, char **argv, char **e)
{
	char	*line;
	t_node	*ast;

	if (argc > 1)
		exit(ft_atoi(argv[1]));
	static_env(e);
	shell_path(argv);
	while (42)
	{
		setup_signal_handling();
		line = readline("> ");
		if (!line)
		{
			free_env(*static_env(NULL));
			free((void *)shell_path(NULL));
			exit(WEXITSTATUS(get_status()));
		}
		if (!*line)
			continue ;
		ast = parser(line);
		set_status(execute(ast));
		free_ast(ast);
		free(line);
	}
}
