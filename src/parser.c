/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:09:10 by franaivo          #+#    #+#             */
/*   Updated: 2024/10/11 13:09:16 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

int	parse_redir(t_cmd *cmd, t_list **tokens)
{
	t_token	*token;
	t_token	*token_next;
	t_redir	*redir;

	if (!cmd || !tokens)
		return (-1);
	token = (*tokens)->content;
	token_next = NULL;
	if ((*tokens)->next)
		token_next = ((*tokens)->next->content);
	redir = malloc(sizeof(t_redir));
	redir->type = token->type;
	redir->string = NULL;
	if (token_next && token_next->type == WORD)
		redir->string = token_next->value;
	ft_lstadd_back(&cmd->redirs, ft_lstnew(redir));
	if (token_next && token_next->type == WORD)
		*tokens = (*tokens)->next;
	return (1);
}

void	**ft_lsttoarr(t_list *lst)
{
	t_size_t	len;
	void		**array;
	int			i;

	len = ft_lstsize(lst);
	array = malloc(sizeof(void *) * (len + 1));
	i = 0;
	if (!array)
		return (NULL);
	while (lst)
	{
		array[i] = lst->content;
		i++;
		lst = lst->next;
	}
	array[len] = NULL;
	return (array);
}

void	*parse_cmd(t_list **tokens)
{
	t_token	*token;
	t_cmd	*cmd;
	t_list	*argv;

	if (!*tokens)
		return (NULL);
	token = (*tokens)->content;
	if (token->type == PIPE)
		return (NULL);
	cmd = malloc(sizeof(t_cmd));
	argv = NULL;
	ft_bzero(cmd, sizeof(t_cmd));
	cmd->type = CMD;
	while (token && token->type != PIPE)
	{
		if (token->type == WORD)
			ft_lstadd_back(&argv, ft_lstnew(token->value));
		else
			parse_redir(cmd, tokens);
		if (!*tokens)
			break ;
		(*tokens) = (*tokens)->next;
		token = NULL;
		if ((*tokens))
			token = (*tokens)->content;
	}
	cmd->argv = (char **)ft_lsttoarr(argv);
	ft_lstclear(&argv, NULL);
	return (cmd);
}

t_node	*parse(t_list **tokens)
{
	t_node	*node;
	void	*pipe;

	node = parse_cmd(tokens);
	if (!*tokens)
		return (node);
	*tokens = (*tokens)->next;
	pipe = malloc(sizeof(t_pipe));
	((t_pipe *)pipe)->type = PIPE;
	((t_pipe *)pipe)->left = node;
	((t_pipe *)pipe)->right = parse(tokens);
	return (pipe);
}

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
		ft_lstclear(&cmd->redirs, free);
		free(cmd);
		return ;
	}
	pipe = ast;
	free_ast(pipe->left);
	free_ast(pipe->right);
	free(pipe);
	return ;
}

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
	types = (char*[6]){"WORD", "REDIR_I", "REDIR_O", "APPEND", "PIPE",
		"HEREDOC"};
	t_redir	*redir;

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
