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
