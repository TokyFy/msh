/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antananariv>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:42:45 by franaivo          #+#    #+#             */
/*   Updated: 2024/12/12 16:56:52 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

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

void	remove_quote_t_cmd(t_cmd *cmd)
{
	char	**argv;
	char	*tmp_str;

	argv = cmd->argv;
	t_list *tmp_redir = cmd->redirs;
	while (*argv)
	{
		tmp_str = remove_quotes(*argv);
		free(*argv);
		*argv = tmp_str;
		argv++;
	}
	while (tmp_redir)
	{
		tmp_str = remove_quotes(((t_redir*)tmp_redir->content)->string);
		free(((t_redir*)tmp_redir->content)->string);
		((t_redir*)tmp_redir->content)->string = tmp_str;
		tmp_redir = tmp_redir->next;
	}
}
