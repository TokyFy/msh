/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:00:01 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 14:08:32 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

char	*get_env_name_in_string(char *string)
{
	char	*name;
	char	*start;
	int		len;

	len = 0;
	while (*string)
	{
		if (*string == '$')
		{
			string++;
			if (ft_isalpha(*string) || *string == '_')
			{
				start = string;
				while (ft_isalnum(*string) || *string == '_')
				{
					len++;
					string++;
				}
				name = ft_strndup(start, len);
				return (name);
			}
		}
		string++;
	}
	return (NULL);
}

char	*get_element_value(t_list *element)
{
	char	*value;

	value = ((t_env *)element->content)->value;
	return (value);
}

char	*expand_variable_if_exists(t_list *env, char *result, char *input,
		int *i)
{
	char	*env_name;
	char	*env_value;
	char	*temp;

	env_name = get_env_name_in_string(&input[*i]);
	if (env_name)
	{
		env_value = get_env(env, env_name);
		free(env_name);
		if (env_value)
		{
			temp = ft_strjoin(result, env_value);
			free(result);
			result = temp;
		}
		while (ft_isalnum(input[*i + 1]) || input[*i + 1] == '_')
			(*i)++;
	}
	return (result);
}

char	*expand_variables_in_string(t_list *env, char *input)
{
	int		i;
	int		start;
	int		in_double_quotes;
	int		in_single_quotes;
	char	*result;

	result = ft_strdup("");
	i = -1;
	start = 0;
	in_double_quotes = 0;
	in_single_quotes = 0;
	while (input[++i])
	{
		toggle_quotes(input[i], &in_single_quotes, &in_double_quotes);
		if (check_quote_status(input, &i, &in_single_quotes, &in_double_quotes))
		{
			if (i > start)
				result = append_text(result, input, start, i);
			result = expand_variable_if_exists(env, result, input, &i);
			start = i + 1;
		}
	}
	if (i > start)
		result = append_text(result, input, start, i);
	return (result);
}
