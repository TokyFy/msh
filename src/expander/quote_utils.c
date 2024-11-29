/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:55:24 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/29 13:24:48 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

char	*remove_quotes(char *input)
{
	char	*result;
	int		i;
	int		j;
	char	quote_char;

	if (!input)
		return (NULL);
	result = malloc(strlen(input) + 1);
	if (!result)
		return (NULL);
	i = 0; 
	j = 0;
	quote_char = '\0';
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '"') && quote_char == '\0')
			quote_char = input[i];
		else if (input[i] == quote_char)
			quote_char = '\0';
		else
			result[j++] = input[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	remove_quote(char *input, char *current_quote, int *j, char **result)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (*current_quote == '\0')
				*current_quote = input[i];
			else if (input[i] == *current_quote)
				*current_quote = '\0';
			else
				(*result)[(*j)++] = input[i];
		}
		else
			(*result)[(*j)++] = input[i];
		i++;
	}
}

char	*handle_quote(char *input)
{
	char	*result;
	char	current_quote;
	char	*final_result;
	int		j;

	result = malloc(ft_strlen(input) + 1);
	if (!result)
		return (NULL);
	j = 0;
	current_quote = '\0';
	remove_quote(input, &current_quote, &j, &result);
	if (current_quote != '\0')
	{
		free(result);
		return (NULL);
	}
	result[j] = '\0';
	final_result = ft_strndup(result, j);
	free(result);
	return (final_result);
}

void	toggle_quotes(char c, int *in_single_quotes, int *in_double_quotes)
{
	if (c == '\'' && !(*in_double_quotes))
	{
		*in_single_quotes = !(*in_single_quotes);
	}
	else if (c == '"' && !(*in_single_quotes))
	{
		*in_double_quotes = !(*in_double_quotes);
	}
}

int	check_quote_status(char *input, int *i, int *in_single_quotes,
		int *in_double_quotes)
{
	return ((input[*i] == '$' && *in_double_quotes && !(*in_single_quotes))
		|| (input[*i] == '$' && !(*in_double_quotes) && !(*in_single_quotes)));
}
