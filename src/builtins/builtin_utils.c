/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 11:54:24 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/18 16:40:19 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

t_list	**static_env(char **e)
{
	static t_list	*env;

	if (e)
	{
		env = copy_env(e);
		return (NULL);
	}
	return (&env);
}

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
