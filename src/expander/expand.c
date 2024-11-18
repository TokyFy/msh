/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:14:30 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 14:09:11 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

static void	expand_main_loop(t_list *env, char ***temp)
{
	int		i;
	char	*input;
	char	*tmp;

	i = -1;
	while ((*temp)[++i])
	{
		input = ft_strdup((*temp)[i]);
		free((*temp)[i]);
		tmp = expand_variables_in_string(env, input);
		(*temp)[i] = handle_quote(tmp);
		free(tmp);
		free(input);
	}
}

void	expand(t_list *env, void *tree)
{
	t_node	*ast;
	t_cmd	*cmd;
	char	**temp;

	ast = tree;
	if (ast->type == CMD)
	{
		cmd = tree;
		temp = cmd->argv;
		expand_main_loop(env, &temp);
	}
}
