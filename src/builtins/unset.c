/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:27:38 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/18 16:36:30 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

int	builtin_unset(t_list **env, char *name)
{
	t_list	*current;
	t_list	*prev;

	prev = NULL;
	current = *env;
	while (current)
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

int	unset(t_cmd *cmd)
{
	t_list	**env;
	int		i;

	env = static_env(NULL);
	i = 1;
	while ((cmd->argv)[i])
	{
		if (builtin_unset(env, (cmd->argv)[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}
