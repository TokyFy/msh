/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:52:39 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 15:00:55 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

void	debug_array_nulled(char **array)
{
	if (!array || !*array)
	{
		printf("NULL");
		return ;
	}
	printf("[ ");
	while (*array)
	{
		printf("%s", *array);
		if (*(array + 1))
			printf(", ");
		array++;
	}
	printf(" ]");
}

void	debug_t_redirs(t_list *lst)
{
	char	**types;
	t_redir	*redir;

	types = (char *[6]){"WORD", "REDIR_I", "REDIR_O", "APPEND", "PIPE",
		"HEREDOC"};
	if (!lst)
		printf("NULL");
	redir = NULL;
	while (lst)
	{
		redir = lst->content;
		printf("[ %s : %s ] ", types[redir->type], redir->string);
		lst = lst->next;
	}
}

void	debug_t_cmd(t_cmd *cmd, int level)
{
	printf("%*sCMD :\n%*s| Arg : ", level, " ", level, " ");
	debug_array_nulled(cmd->argv);
	printf("\n%*s| I/O : ", level, " ");
	debug_t_redirs(cmd->redirs);
	printf("\n");
}

void	print_ast(void *tree, int level)
{
	int		factor;
	t_node	*ast;
	t_pipe	*pipe;

	factor = 2;
	ast = tree;
	if (!ast)
	{
		printf("%*sNULL\n\n", level * factor, " ");
		return ;
	}
	if (ast->type == CMD)
	{
		debug_t_cmd(tree, level * factor);
		return ;
	}
	pipe = tree;
	printf("%*sPIPE :\n", level * factor, " ");
	print_ast(pipe->left, level + 1);
	print_ast(pipe->right, level + 1);
}
