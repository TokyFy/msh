/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 09:33:43 by franaivo          #+#    #+#             */
/*   Updated: 2024/11/15 14:28:06 by sranaivo         ###   ########.fr       */
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
