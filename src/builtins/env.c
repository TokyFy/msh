/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:16:38 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 13:32:46 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

void	free_env(t_list *env)
{
	t_list	*temp;

	temp = env;
	while (temp)
	{
		free(((t_env *)temp->content)->name);
		free(((t_env *)temp->content)->value);
		temp = temp->next;
	}
	ft_lstclear(&env, free);
}

char	*get_env(t_list *env, char *name)
{
	t_list	*tmp;

	tmp = env_exist(env, name);
	if (tmp)
	{
		return (get_element_value(tmp));
	}
	return (NULL);
}

t_env	*new_env(char *str)
{
	t_env			*env;
	char			*tmp;
	unsigned int	i;

	tmp = str;
	tmp++;
	i = 0;
	env = malloc(sizeof(t_env));
	while (str[i] && str[i] != '=')
		i++;
	env->name = ft_substr(str, 0, i);
	env->value = ft_substr(str, i + 1, -1);
	return (env);
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

void	builtin_env(t_list *env)
{
	t_list	*tmp;

	if (!env)
		return ;
	tmp = env;
	while (tmp)
	{
		printf("%s=%s\n", ((t_env *)tmp->content)->name,
			((t_env *)tmp->content)->value);
		tmp = tmp->next;
	}
	return ;
}
