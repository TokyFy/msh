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

int	main(const int argc, char **argv, char **e)
{
	char	*buff;
	char	*line;
	(void)(argc);
	(void)(argv);
	t_list	*tokens;
	t_list	*env;
  
	env = get_env(e);
	msh_env(env);
  
	t_node *ast;

	while(42)
	{
		line = readline("> ");
		buff = line;
		add_history(buff);
		tokens = tokenizer(&buff);
		ast = parse(&tokens);
		print_ast(ast , 0);
		free(line);
	}
}
