/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:21:46 by sranaivo          #+#    #+#             */
/*   Updated: 2024/12/09 12:59:17 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

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
		return (1);
	new_element = new_env(str);
	if (!new_element)
		return (1);
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

int are_all_env_names_valid(char **names)
{
	char *tmp;
    if (!names)
        return 0;

    while (*names)
    {
        tmp = ft_substr(*names, 0, ft_strchr(*names , '=') - *names);
		if(!is_valid_env_name(tmp))
		{
			ft_putendl_fd("msh : export : invalid identidier", STDERR_FILENO);
			free(tmp);
			return 0;
		}
		free(tmp);
		names++;
    }
    return 1;
}

int	ft_export(t_cmd *cmd)
{
	t_list	**env;
	int		i;

	env = static_env(NULL);
	i = 1;
	if (!are_all_env_names_valid(cmd->argv))
		return (1);
	while ((cmd->argv)[i])
	{
		if (builtin_export(env, (cmd->argv)[i]) != 0)
			return (0);
		i++;
	}
	return (0);
}
