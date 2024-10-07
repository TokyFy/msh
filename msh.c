/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:22:04 by franaivo          #+#    #+#             */
/*   Updated: 2024/10/07 16:48:49 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

int	main(const int argc, char **argv)
{
	//t_token	*token;
	char	*buff;
	char	*line;

	(void)(argc);
	(void)(argv);

	t_list	*list;
	while(1)
	{
		line = readline("> ");
		buff = line;
		add_history(buff);
		// while ((token = get_token(&buff)))
		// 	print_token(token);

		list = tokenizer(&buff);
		while (list)
		{
			printf("%s\n", ((t_token *)list->content)->value);
			list = list->next;
		}
		
		free(line);
	}
}
