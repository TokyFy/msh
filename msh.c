/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:22:04 by franaivo          #+#    #+#             */
/*   Updated: 2024/10/30 17:24:47 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

// int	main(const int argc, char **argv, char **e)
// {
// 	char	*buff;
// 	char	*line;
// 	(void)(argc);
// 	(void)(argv);
// 	t_list	*tokens;
// 	t_list	*env;
  
// 	env = get_env(e);
// 	msh_env(env);
  
// 	t_node *ast;

// 	// msh_env(env);
// 	while(42)
// 	{
// 		line = readline("> ");
// 		buff = line;
// 		add_history(buff);
// 		tokens = tokenizer(&buff);
// 		ast = parse(&tokens);
// 		print_ast(ast , 0);
// 		// while (tokens)
// 		// {
// 		// 	print_token(tokens->content);
// 		// 	tokens = tokens->next;
// 		// }

// 		if (ft_strcmp(line, "env") == 0)
// 		{
// 			builtin_env(env);
// 		} else
// 		{
// 			builtin_export(env, line);
// 		}
		
// 		// print_token(tokens);
// 		// parse(&tokens);
// 		if (ft_strcmp(line, "exit"))
// 		{
// 			free(line);
// 			break ;
// 		}
// 		free(line);
// 	}
// 	free_env(env);
// 	return (0);
// }

int	main(const int argc, char **argv, char **e)
{
	//t_token	*token;
	char	*buff;
	char	*line;
	(void)(argc);
	(void)(argv);
	t_list	*tokens;
	t_list	*env;
  
	env = copy_env(e);
  
	t_node *ast;
	while(42)
	{
		line = readline("> ");
		buff = line;
		add_history(buff);
		tokens = tokenizer(&buff);
		ast = parse(&tokens);
		print_ast(ast , 0);
		expand(env, ast);
		print_ast(ast, 0);
		free(line);
	}
}
