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

#include <msh.h>
#include <string.h>

volatile sig_atomic_t	g_signal_received;

int	main(const int argc, char **argv, char **e)
{
	char	*line;
	t_node	*ast;
	int		status;

	(void)(argc);
	(void)(argv);
	status = 0;
	static_env(e);
	while (42)
	{
		setup_signal_handling();
		line = readline("> ");
		if (!line)
		{
			free_env(*static_env(NULL));
			exit(get_status());
		}
		ast = parser(line);
		status = execute(ast , e);
		set_status(status);
		free_ast(ast);
		free(line);
	}
}
