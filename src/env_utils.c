/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 09:55:51 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/15 10:28:51 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

static int	list_size(t_list *list)
{
	int	size;

	size = 0;
	while (list)
	{
		size++;
		list = list->next;
	}
	return (size);
}

// static void	free_string_array(char **array, int filled)
// {
// 	int	i;

// 	i = 0;
// 	while (i < filled)
// 	{
// 		free(array[i]);
// 		i++;
// 	}
// 	free(array);
// }

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**list_to_env_array(t_list *env_list)
{
	char	**env_array;
	int		i;
	t_env	*env;
	char	*name_eq;
	char	*full_env;

	i = list_size(env_list);
	env_array = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env_list)
	{
		env = (t_env *)env_list->content;
		name_eq = ft_strjoin(env->name, "=");
		full_env = ft_strjoin(name_eq, env->value);
		free(name_eq);
		env_array[i++] = full_env;
		env_list = env_list->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
