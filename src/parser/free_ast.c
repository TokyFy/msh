/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:56:01 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 14:58:00 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

void	free_array_nulled(char **array)
{
	char	**str;

	str = array;
	while (*str)
	{
		free(*str);
		str++;
	}
	free(array);
	return ;
}

void	free_redir(void *ptr)
{
	t_redir	*redir;

	redir = ptr;
	free(redir->string);
	free(redir);
}

void	free_ast(void *ast)
{
	t_node	*node;
	t_cmd	*cmd;
	t_pipe	*pipe;

	node = ast;
	if (!ast)
		return ;
	if (node->type == CMD)
	{
		cmd = ast;
		free_array_nulled(cmd->argv);
		ft_lstclear(&cmd->redirs, free_redir);
		free(cmd);
		return ;
	}
	pipe = ast;
	free_ast(pipe->left);
	free_ast(pipe->right);
	free(pipe);
	return ;
}
