/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyse_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:47:35 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 14:48:40 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

static int	analyse_t_pipe(t_pipe *pipe)
{
	if (pipe->left && pipe->right)
	{
		if (!analyse_ast(pipe->right) || !analyse_ast(pipe->left))
			return (0);
		return 1;
	}
	return 0;
}

static int	analyse_t_cmd(t_cmd *cmd)
{
	t_list	*redirs;

	// if (!cmd->argv || !*cmd->argv)
	//	return (0);
	redirs = cmd->redirs;
	while (redirs)
	{
		if (((t_redir *)redirs->content)->string == NULL)
			return (0);
		redirs = redirs->next;
	}
	return (1);
}

int	analyse_ast(void *tree)
{
	t_node	*node;

	node = tree;
	if (!node)
		return (0);
	if (node->type == PIPE)
	{
		return (analyse_t_pipe((t_pipe *)tree));
	}
	if (node->type == CMD)
	{
		return (analyse_t_cmd((t_cmd *)tree));
	}
	return (1);
}
