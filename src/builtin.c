/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 09:12:23 by sranaivo          #+#    #+#             */
/*   Updated: 2024/10/10 16:57:37 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

t_env	*new_env(char *str)
{
	t_env			*env;
	unsigned int	i;

	i = 0;
	env = malloc(sizeof(t_env));
	while (str[i] && str[i] != '=')
		i++;
	env->name = ft_substr(str, 0, i);
	env->value = ft_substr(str, i + 1, -1);
	return (env);
}

t_list	*get_env(char **env)
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

void	msh_env(t_list *env)
{
	while (env)
	{
		printf("%s=%s\n", ((t_env *)env->content)->name,
			((t_env *)env->content)->value);
		env = env->next;
	}
	return ;
}
