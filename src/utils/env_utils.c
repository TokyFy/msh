/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 09:55:51 by sranaivo          #+#    #+#             */
/*   Updated: 2024/12/12 09:02:21 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

char	**list_to_env_array(t_list *env_list)
{
	char	**env_array;
	int		i;
	t_env	*env;
	char	*name_eq;
	char	*full_env;

	i = ft_lstsize(env_list);
	env_array = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env_list)
	{
		env = (t_env *)env_list->content;
		name_eq = ft_strjoin(env->name, "=");
		if (!(env->value))
			full_env = ft_strjoin(name_eq, "");
		else
			full_env = ft_strjoin(name_eq, env->value);
		free(name_eq);
		env_array[i++] = full_env;
		env_list = env_list->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	*trim_space(const char *str)
{
	char	*result;
	char	*write;
	int		space_found;

	result = (char *)ft_calloc(sizeof(char), ft_strlen(str) + 1);
	write = result;
	space_found = 0;
	while (*str != '\0')
	{
		if (ft_isspace((unsigned char)*str))
		{
			if (!space_found)
			{
				*write++ = ' ';
				space_found = 1;
			}
		}
		else
		{
			*write++ = *str;
			space_found = 0;
		}
		str++;
	}
	return (result);
}

int	is_valid_env_name(char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i] != '\0')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

t_list	*copy_env(char **env)
{
	t_list	*list_env;
	t_list	*element;
	t_env	*env_element;

	list_env = NULL;
	while (*env)
	{
		env_element = new_env(*env);
		if (list_env == NULL)
			list_env = ft_lstnew(env_element);
		else
		{
			element = ft_lstnew(env_element);
			ft_lstadd_back(&list_env, element);
		}
		env++;
	}
	return (list_env);
}

int	are_all_env_names_valid(char **names)
{
	char	*tmp;
	int		error;

	error = 0;
	if (!names)
		return (0);
	while (*names)
	{
		tmp = ft_substr(*names, 0, ft_strchr(*names, '=') - *names);
		if (!is_valid_env_name(tmp))
		{
			ft_putendl_fd("msh : export : invalid identidier", STDERR_FILENO);
			error++;
		}
		free(tmp);
		names++;
	}
	return (!error);
}
