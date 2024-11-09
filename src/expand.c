/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:14:30 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/09 15:24:46 by sranaivo         ###   ########.fr       */
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

char	*append_text(char *result, char *input, int start, int end)
{
	char	*portion;
	char	*temp;

	portion = ft_substr(input, start, end - start);
	temp = ft_strjoin(result, portion);
	free(result);
	free(portion);
	return (temp);
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

int	check_quote_status(char *input, int *i, int *in_single_quotes,
		int *in_double_quotes)
{
	return ((input[*i] == '$' && *in_double_quotes && !(*in_single_quotes))
		|| (input[*i] == '$' && !(*in_double_quotes) && !(*in_single_quotes)));
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

void	expand(t_list *env, void *tree)
{
	t_node	*ast;
	t_cmd	*cmd;
	char	**temp;
	int		i;
	char	*input;
	char	*tmp;

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
