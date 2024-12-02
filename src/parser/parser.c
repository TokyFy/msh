/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:09:10 by franaivo          #+#    #+#             */
/*   Updated: 2024/11/15 15:32:49 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <msh.h>
#include <stdlib.h>
#include <string.h>

void	*parser(char *line)
{
	t_list	*tokens;
	t_list	*tokens_t;
	void	*ast;

	add_history(line);
	tokens = tokenizer(&line);
	tokens_t = tokens;
	ast = parse(&tokens);
	free_tokens(tokens_t);
	return (ast);
}

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

void	fill_t_cmd(t_cmd *cmd, t_list **tokens)
{
	t_list	*argv;
	t_token	*token;

	argv = NULL;
	token = (*tokens)->content;
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
}

void	*parse_cmd(t_list **tokens)
{
	t_token	*token;
	t_cmd	*cmd;

	if (!*tokens)
		return (NULL);
	token = (*tokens)->content;
	if (token->type == PIPE)
		return (NULL);
	cmd = malloc(sizeof(t_cmd));
	ft_bzero(cmd, sizeof(t_cmd));
	cmd->type = CMD;
	fill_t_cmd(cmd, tokens);
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