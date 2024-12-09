/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:00:01 by sranaivo          #+#    #+#             */
/*   Updated: 2024/12/09 12:16:42 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

char	*extract_env_name(char *string)
{
	char	*name;
	char	*start;
	int		len;

	len = 0;
	if (*string == '?')
		return (ft_strdup("?"));
	if ((ft_isspace(*string) || (*string == '"' && ft_chrc(string, '\"') % 2 != 0)
			|| !*string))
		return (ft_strdup("$"));
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
	return (NULL);
}

char	*get_env_name_in_string(char *string)
{
	while (*string)
	{
		if (*string == '$')
		{
			string++;
			return (extract_env_name(string));
		}
		string++;
	}
	return (NULL);
}

char	*append_env_value(t_list *env, char *result, char *env_name)
{
	char	*env_value;
	char	*temp;

	env_value = get_env(env, env_name);
	if (env_value)
	{
		temp = ft_strjoin(result, env_value);
		free(result);
		result = temp;
	}
	return (result);
}

char	*expand_variable_if_exists(t_list *env, char *result, char *input,
		int *i)
{
	char	*env_name;

	env_name = get_env_name_in_string(&input[*i]);
	if (ft_strcmp(env_name, "?") == 0)
	{
		result = append_env_value(env, result, env_name);
		free(env_name);
		(*i)++;
	}
	else if (env_name)
	{
		result = append_env_value(env, result, env_name);
		free(env_name);
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
