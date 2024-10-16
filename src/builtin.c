/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 09:12:23 by sranaivo          #+#    #+#             */
/*   Updated: 2024/10/15 17:33:02 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

t_env	*new_env(char *str)
{
	t_env	*env;
	unsigned int	i;

	i = 0;
	env = malloc(sizeof(t_env));
	while (str[i] && str[i] != '=')
		i++;
	env->name = ft_substr(str, 0, i);
	env->value = ft_substr(str, i + 1, -1);
	return (env);
}

t_list *get_env(char **env)
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
			element = ft_lstnew(env_element );
			ft_lstadd_back(&list_env, element);
		}
		env++;
	}
	return (list_env);
}

void	builtin_env(t_list *env)
{
	while (env)
	{
		printf("%s=%s\n", ((t_env *)env->content)->name, ((t_env *)env->content)->value);
		env = env->next;
	}
	return ;
}

int	ft_strcmp(char *s1, char *s2)
{
	unsigned int	i;

	i = 0;
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
		i++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

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

void	update_env_element(t_list *element, char *value)
{
	char *new_value;

	new_value = ft_strdup(value);
	free(((t_env *)element->content)->value);
	((t_env *)element->content)->value = new_value;
	return ;
}

void	builtin_export(t_list *env, char *str)
{
	t_env	*new_element = new_env(str);
	t_list	*element;
	element = env_exist(env, new_element->name);
	if (element)
	{
		update_env_element(element, new_element->value);
		free(new_element->name);
		free(new_element->value);
		free(new_element);
		new_element = NULL;
	} else
	{
		ft_lstadd_back(&env, ft_lstnew(new_element));
	}
}

void	free_env(t_list *env)
{
	t_list *temp;

	temp = env;
	while (temp)
	{
		free(((t_env *)temp->content)->name);
		free(((t_env *)temp->content)->value);
		temp = temp->next;
	}
	ft_lstclear(&env, free);
}

