/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:27:38 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 13:29:10 by sranaivo         ###   ########.fr       */
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
