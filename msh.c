/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:22:04 by franaivo          #+#    #+#             */
/*   Updated: 2024/10/10 12:44:14 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>
#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int	main(const int argc, char **argv, char **env)
{
	char	*buff;
	char	*line;
	t_list	*tokens;
	t_list	*tokens_t;
	t_node	*ast;
	int ret;

	(void)(argc);
	(void)(argv);
	(void)(env);
	ret = 0;
	while (42)
	{
		line = readline("> ");
		buff = line;
		add_history(buff);
		tokens = tokenizer(&buff);
		tokens_t = tokens;
		ast = parse(&tokens);
		if(analyse_ast(ast))
		{
			if(fork() == 0)
				exec_ast(ast);
			wait(&ret);
		}
		else
		  printf("Error\n");
		free_tokens(tokens_t);
		free_ast(ast);
		free(line);
	}
}
