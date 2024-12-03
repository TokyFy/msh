/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:22:04 by franaivo          #+#    #+#             */
/*   Updated: 2024/11/29 12:23:44 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <msh.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t	g_signal_received;

const char* shell_path(char **argv)
{
	static char *path;
	char *pwd = NULL;
	if(!argv)
		return path;

	pwd = getcwd(pwd, 0);
	path = ft_strjoin(pwd, "/minishell");
	free(pwd);
	return NULL;
}

int	main(const int argc, char **argv, char **e)
{
	char	*line;
	t_node	*ast;
	if(argc > 1)
		exit(ft_atoi(argv[1]));
	shell_path(argv);
	static_env(e);
	while (42)
	{
		setup_signal_handling();
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("exit\n" , STDERR_FILENO);
			free(line);
			free_env(*static_env(NULL));
			free((void*)shell_path(NULL));
			exit(WEXITSTATUS(get_status()));
		}
		ast = parser(line);
		set_status(execute(ast , e));
		free_ast(ast);
		free(line);
	}
}
