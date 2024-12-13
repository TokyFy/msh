/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:32:57 by franaivo          #+#    #+#             */
/*   Updated: 2024/12/13 08:31:08 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

char	*get_env(t_list *env, char *name)
{
	t_list	*tmp;
	int		status;

	if (ft_strcmp(name, "?") == 0)
	{
		status = WEXITSTATUS(get_status());
		return (ft_itoa(status));
	}
	if (ft_strcmp(name, "") == 0)
	{
		return (ft_strdup(""));
	}
	if (ft_strcmp(name, "$") == 0)
		return (ft_strdup("$"));
	tmp = env_exist(env, name);
	if (tmp)
	{
		return (get_element_value(tmp));
	}
	return (NULL);
}

void	set_env(char *env, char *value)
{
	char	*str1;
	char	*joined;

	str1 = ft_strjoin(env, "=");
	if (!value)
		joined = ft_strjoin(str1, "");
	else
		joined = ft_strjoin(str1, value);
	builtin_export(static_env(NULL), joined);
	free(str1);
	free(joined);
}

t_env	*new_env(char *str)
{
	t_env			*env;
	char			*tmp;
	unsigned int	i;

	i = 0;
	tmp = NULL;
	env = malloc(sizeof(t_env));
	while (str[i] && str[i] != '=')
		i++;
	env->name = ft_substr(str, 0, i);
	if (!(str[i]))
		env->value = NULL;
	else
		tmp = ft_substr(str, i + 1, -1);
	env->value = remove_quotes(tmp);
	if (!is_valid_env_name(env->name))
	{
		free(env->name);
		free(env->value);
		free(env);
		free(tmp);
		return (NULL);
	}
	return (free(tmp), env);
}

int	builtin_env(t_list *env)
{
	t_list	*tmp;

	if (!env)
		return (1);
	tmp = env;
	while (tmp)
	{
		if (((t_env *)tmp->content)->value != NULL)
		{
			printf("%s=%s\n", ((t_env *)tmp->content)->name,
				((t_env *)tmp->content)->value);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	ft_env(t_cmd *cmd)
{
	if (cmd->argv[1])
	{
		ft_putendl_fd("msh : No such file or directory", STDERR_FILENO);
		_exit2(127);
	}
	return (builtin_env(*(static_env(NULL))));
}
