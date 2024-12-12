/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 15:16:18 by sranaivo          #+#    #+#             */
/*   Updated: 2024/12/11 15:19:01 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>
#include <stdlib.h>

char	*expand_in_heredoc(char *str)
{
	char	*tmp_str;
	char	*result;

	tmp_str = str;
	while (*tmp_str)
	{
		if (*tmp_str == '\'')
			*tmp_str = 1;
		tmp_str++;
	}
	result = expand_variables_in_string(*static_env(NULL), str);
	tmp_str = result;
	while (*tmp_str)
	{
		if (*tmp_str == 1)
			*tmp_str = '\'';
		tmp_str++;
	}
	return (result);
}

static int	is_special_case(const char *original, int i, char current_quote)
{
	if (original[i] == '$' && current_quote == '\0' && (original[i + 1] == '\''
			|| original[i + 1] == '"'))
		return (1);
	return (0);
}

static void	update_quote_state(char c, char *current_quote)
{
	if (*current_quote == c)
		*current_quote = '\0';
	else if (*current_quote == '\0')
		*current_quote = c;
}

void	remove_translate(char **input)
{
	char	*original;
	char	*result;
	int		i;
	int		j;
	char	current_quote;

	original = *input;
	result = malloc(strlen(original) + 1);
	i = 0;
	j = 0;
	current_quote = '\0';
	while (original[i])
	{
		if (original[i] == '\'' || original[i] == '"')
			update_quote_state(original[i], &current_quote);
		else if (is_special_case(original, i, current_quote))
			i++;
		else
			result[j++] = original[i];
		i++;
	}
	result[j] = '\0';
	free(*input);
	*input = result;
}
