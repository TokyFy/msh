/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 09:12:23 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 08:38:25 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

int contains_equal_after_first(const char *str) {
    if (str == NULL || str[1] == '\0') {
        
        return 0;
    }

    int i = 1;
    while (str[i] != '\0') {
        if (str[i] == '=') {
            return 1;
        }
        i++;
    }

    return 0;
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

t_list *copy_env(char **env)
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

int	builtin_export(t_list *env, char *str)
{
	t_env	*new_element;
	t_list	*element;

	if (!contains_equal_after_first(str))
		return (1);
	new_element  = new_env(str);
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
	return (0);
}

int	builtin_unset(t_list **env, char *name)
{
	t_list	*current;
	t_list	*prev;

	prev = NULL;
	current = *env;
	
	while(current)
	{
		if (!ft_strcmp(name, ((t_env *)current->content)->name))
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(((t_env *)current->content)->name);
			free(((t_env *)current->content)->value);
			free(current->content);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
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

char *get_env(t_list *env, char *name)
{
	t_list	*tmp;

	tmp = env_exist(env, name);
	if (tmp)
	{
		return (get_element_value(tmp));
	}
	return (NULL);
}
