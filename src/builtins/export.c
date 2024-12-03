/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:21:46 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/18 16:10:34 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <msh.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

t_list	*env_exist(t_list *env, char *name)
{
	t_list	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(((t_env *)temp->content)->name, name) == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

static int	contains_equal_after_first(const char *str)
{
	int	i;

	if (str == NULL || str[1] == '\0')
	{
		return (0);
	}
	i = 1;
	while (str[i] != '\0')
	{
		if (str[i] == '=')
		{
			return (1);
		}
		i++;
	}
	return (0);
}

static void	update_env_element(t_list *element, char *value)
{
	char	*new_value;

	new_value = ft_strdup(value);
	free(((t_env *)element->content)->value);
	((t_env *)element->content)->value = new_value;
	return ;
}

int	builtin_export(t_list **env, char *str)
{
	t_env	*new_element;
	t_list	*element;

	if (!contains_equal_after_first(str))
	{
		return (1);
	}
	new_element = new_env(str);
	element = env_exist(*env, new_element->name);
	if (element)
	{
		update_env_element(element, new_element->value);
		free(new_element->name);
		free(new_element->value);
		free(new_element);
		new_element = NULL;
		return (0);
	}
	else
	{
		ft_lstadd_back(env, ft_lstnew(new_element));
		return (0);
	}
	return (1);
}

void set_env(char *env , char* value)
{
	char *str1 = ft_strjoin(env, "=");
	char *joined = ft_strjoin(str1, value);
	builtin_export(static_env(NULL), joined);
	free(str1);
	free(joined);
}

int	ft_export(t_cmd *cmd)
{
	t_list	**env;
	int		i;

	env = static_env(NULL);
	i = 1;
	while ((cmd->argv)[i])
	{
		if (builtin_export(env, (cmd->argv)[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}
