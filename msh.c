/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:22:04 by franaivo          #+#    #+#             */
/*   Updated: 2024/11/15 15:56:43 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

volatile sig_atomic_t	g_signal_received;

int	main(const int argc, char **argv, char **e)
{
	char	*line;
	t_node	*ast;
	int		status = 0;

	(void)(argc);
	(void)(argv);
	(void)(status);
	while (42)
	{
		setup_signal_handling();
		line = readline("> ");
		if (!line)
			exit(0);
		ast = parser(line);
		status = execute(ast , e);
		free_ast(ast);
		free(line);
	}
}
