/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:24:39 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 14:26:39 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

static t_token	*parse_redirection_input(char **buff)
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

static t_token	*parse_redirection_output(char **buff)
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

static t_token	*parse_pipe_operator(char **buff)
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
