/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:28:32 by sranaivo          #+#    #+#             */
/*   Updated: 2024/12/10 12:58:39 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

static t_list	*copy_env_node(t_env *original, t_list *copy)
{
	t_env	*new_env;
	t_list	*new_node;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	new_env->name = ft_strdup(original->name);
	if (original->value)
		new_env->value = ft_strdup(original->value);
	else
		new_env->value = NULL;
	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		free(new_env->name);
		free(new_env->value);
		free(new_env);
		return (NULL);
	}
	new_node->content = new_env;
	new_node->next = copy;
	return (new_node);
}

t_list	*copy_env_list(t_list *env_list)
{
	t_list	*copy;
	t_list	*current;
	t_env	*original;
	t_list	*new_node;

	copy = NULL;
	current = env_list;
	while (current)
	{
		original = (t_env *)current->content;
		new_node = copy_env_node(original, copy);
		if (!new_node)
		{
			free_env(copy);
			return (NULL);
		}
		copy = new_node;
		current = current->next;
	}
	return (copy);
}

static void	print_env_var(t_env *env_var)
{
	if (!env_var || !env_var->name)
		return ;
	if (env_var->value)
		printf("export %s=\"%s\"\n", env_var->name, env_var->value);
	else
		printf("export %s\n", env_var->name);
}

static void	sort_env_list(t_list **env_list)
{
	t_list	*i;
	t_list	*j;
	t_env	*temp;
	t_env	*env_i;
	t_env	*env_j;

	i = *env_list;
	while (i)
	{
		j = i->next;
		while (j)
		{
			env_i = (t_env *)i->content;
			env_j = (t_env *)j->content;
			if (ft_strcmp(env_i->name, env_j->name) > 0)
			{
				temp = i->content;
				i->content = j->content;
				j->content = temp;
			}
			j = j->next;
		}
		i = i->next;
	}
}

void	export_without_argument(t_list *env)
{
	t_list	*env_list;
	t_list	*tmp;

	tmp = copy_env_list(env);
	env_list = tmp;
	if (!env_list)
		return ;
	sort_env_list(&env_list);
	while (env_list)
	{
		print_env_var((t_env *)env_list->content);
		env_list = env_list->next;
	}
	free_env(tmp);
}
