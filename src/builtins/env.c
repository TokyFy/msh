/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:16:38 by sranaivo          #+#    #+#             */
/*   Updated: 2024/12/09 17:25:45 by sranaivo         ###   ########.fr       */
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
	env = malloc(sizeof(t_env));
	while (str[i] && str[i] != '=')
		i++;
	env->name = ft_substr(str, 0, i);
	tmp = ft_substr(str, i + 1, -1);
	env->value = trim_space(tmp);
	if (!is_valid_env_name(env->name))
	{
		ft_putendl_fd("msh : export : invalid identidier", STDERR_FILENO);
		free(env->name);
		free(env->value);
		free(env);
		return (NULL);
	}
	free(tmp);
	return (env);
}

int	builtin_env(t_list *env)
{
	t_list	*tmp;

	if (!env)
		return (1);
	tmp = env;
	while (tmp)
	{
		printf("%s=%s\n", ((t_env *)tmp->content)->name,
			((t_env *)tmp->content)->value);
		tmp = tmp->next;
	}
	return (0);
}

int	ft_env(t_cmd *cmd)
{
	(void)cmd;
	return (builtin_env(*(static_env(NULL))));
}
