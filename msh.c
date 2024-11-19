/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:22:04 by franaivo          #+#    #+#             */
/*   Updated: 2024/11/18 15:42:19 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>
#include <string.h>

volatile sig_atomic_t	g_signal_received;

int	main(const int argc, char **argv, char **e)
{
	char	*line;
	t_node	*ast;
	int		status = 0;

	(void)(argc);
	(void)(argv);
	(void)(status);
	static_env(e);
	while (42)
	{
		setup_signal_handling();
		line = readline("> ");
		if (!line)
		{
			free_env(*static_env(NULL));
			exit(0);
		}
		ast = parser(line);
		status = execute(ast , e);
		free_ast(ast);
		free(line);
	}
}
