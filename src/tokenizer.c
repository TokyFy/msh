/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 09:33:43 by franaivo          #+#    #+#             */
/*   Updated: 2024/11/11 16:19:12 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <msh.h>
#include <string.h>

void	print_token(const t_token *token)
{
	char	**types;

	types = (char *[6]){"word", "redir_i", "redir_o", "append", "pipe",
		"heredoc"};
	if (!token)
	{
		printf("| NULL |\n");
		return ;
	}
	printf("| TYPE : %-10s ", types[token->type]);
	printf("VALUE : %-10s |\n", token->value);
}

t_token	*get_word(char **buff)
{
	char	*str;
	t_token	*token;
	char	quoted;

	str = *buff;
	quoted = 0;
	if (!*str)
		return (NULL);
	while (*str && (quoted || (!ft_strchr(SYMBOL, *str)
				&& !ft_strchr(WHITESPACE, *str))))
	{
		if (!quoted && ft_strchr("\"\'", *str))
			quoted = *str;
		else if (quoted && quoted == *str)
			quoted = 0;
		str++;
	}
	token = malloc(sizeof(t_token));
	token->type = WORD;
	token->value = ft_strndup(*buff, str - *buff);
	*buff = str;
	return (token);
}

t_token	*parse_redirection_input(char **buff)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->value = NULL;
	if (*(*buff + 1) == '<')
	{
		(*buff) += 2;
		token->type = HEREDOC;
	}
	else
	{
		(*buff)++;
		token->type = REDIR_I;
	}
	return (token);
}

t_token	*parse_redirection_output(char **buff)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->value = NULL;
	if (*(*buff + 1) == '>')
	{
		(*buff) += 2;
		token->type = APPEND;
	}
	else
	{
		(*buff)++;
		token->type = REDIR_O;
	}
	return (token);
}

t_token	*parse_pipe_operator(char **buff)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->value = NULL;
	(*buff)++;
	token->type = PIPE;
	return (token);
}

t_token	*get_operator(char **buff)
{
	if (**buff == '<')
		return (parse_redirection_input(buff));
	if (**buff == '>')
		return (parse_redirection_output(buff));
	if (**buff == '|')
		return (parse_pipe_operator(buff));
	return (NULL);
}

t_token	*get_token(char **buff)
{
	char	*str;

	if (!buff || !*buff)
		return (NULL);
	while (**buff && ft_strchr(WHITESPACE, **buff))
		(*buff)++;
	str = *buff;
	if (!*str)
		return (NULL);
	if (strchr(SYMBOL, *str))
		return (get_operator(buff));
	return (get_word(buff));
}

void	free_tokens(t_list *tokens)
{
	ft_lstclear(&tokens, free);
}

t_list	*tokenizer(char **buff)
{
	t_list	*lst;
	t_list	*element;
	t_token	*temp;

	lst = NULL;
	temp = get_token(buff);
	while (temp)
	{
		element = ft_lstnew(temp);
		ft_lstadd_back(&lst, element);
		temp = get_token(buff);
	}
	return (lst);
}
