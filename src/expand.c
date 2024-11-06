/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:14:30 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/06 16:53:36 by sranaivo         ###   ########.fr       */
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

char	*handle_quote(char *input)
{
	char	*result;
	int		i;
	char	current_quote;
	char	*final_result;
	int		j;

	result = malloc(ft_strlen(input) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	current_quote = '\0';
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (current_quote == '\0')
				current_quote = input[i];
			else if (input[i] == current_quote)
				current_quote = '\0';
			else
				result[j++] = input[i];
		}
		else
			result[j++] = input[i];
		i++;
	}
	if (current_quote != '\0')
	{
		free(result);
		return (NULL);
	}
	result[j] = '\0';
	final_result = strndup(result, j);
	free(result);
	return (final_result);
}

char	*expand_variables_in_string(t_list *env, char *input)
{
	char	*result;
	int		i;
	int		in_double_quotes;
	int		in_single_quotes;
	char	*before_var;
	char	*env_name;
	char	*env_value;
	char	*remaining;
	char	*temp;
	int		start;

	result = ft_strdup("");
	i = 0, start = 0;
	in_double_quotes = 0;
	in_single_quotes = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			i++;
			continue ;
		}
		else if (input[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			i++;
			continue ;
		}
		if ((input[i] == '$' && in_double_quotes && !in_single_quotes)
			|| (input[i] == '$' && !in_double_quotes && !in_single_quotes))
		{
			if (i > start)
			{
				before_var = ft_substr(input, start, i - start);
				temp = ft_strjoin(result, before_var);
				free(result);
				free(before_var);
				result = temp;
			}
			env_name = get_env_name_in_string(&input[i]);
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
				while (isalnum(input[i + 1]) || input[i + 1] == '_')
					i++;
			}
			start = i + 1;
		}
		i++;
	}
	if (i > start)
	{
		remaining = ft_substr(input, start, i - start);
		temp = ft_strjoin(result, remaining);
		free(result);
		free(remaining);
		result = temp;
	}
	return (result);
}

void	expand(t_list *env, void *tree)
{
	t_node	*ast;
	t_cmd	*cmd;
	char	**temp;
	int		i;
	char	*input;
	char	*tmp;

	(void)env;
	ast = tree;
	if (ast->type == CMD)
	{
		cmd = tree;
		i = -1;
		temp = cmd->argv;
		while (temp[++i])
		{
			input = ft_strdup(temp[i]);
			free(temp[i]);
			tmp = expand_variables_in_string(env, input);
			temp[i] = handle_quote(tmp);
			free(tmp);
			free(input);
		}
	}
}
