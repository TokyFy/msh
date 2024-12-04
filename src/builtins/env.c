/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:16:38 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/29 11:08:58 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <msh.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char	*get_env(t_list *env, char *name)
{
	t_list	*tmp;
	int		status;

	if (ft_strcmp(name, "?") == 0)
	{
		status = WEXITSTATUS(get_status());
		return (ft_itoa(status));
	}
	if(ft_strcmp(name, "$") == 0)
		return ft_strdup("$");
	tmp = env_exist(env, name);
	if (tmp)
	{
		return (get_element_value(tmp));
	}
	return (NULL);
}

char* trim_space(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    size_t len = ft_strlen(str);
    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    const char *read = str;
    char *write = result;
    int space_found = 0;
    while (*read != '\0') {
        if (isspace((unsigned char)*read)) {
            if (!space_found) {
                *write++ = ' ';
                space_found = 1;
            }
        } else {
            *write++ = *read;
            space_found = 0;
        }
        read++;
    }
    *write = '\0';
    return result;
}

int is_valid_env_name(char *name) {
    if (!name || !*name) return 0;
    if (!isalpha(name[0]) && name[0] != '_') return 0;
    int i = 1;
    while (name[i] != '\0') {
        if (!isalnum(name[i]) && name[i] != '_') return 0;
        i++;
    }
    return 1;
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
	if(!is_valid_env_name(env->name))
	{
		ft_putendl_fd("msh : export : invalid identidier", STDERR_FILENO);
		free(env->name);
		free(env->value);
		free(env);
		return NULL;
	}
	free(tmp);
	return (env);
}

t_list	*copy_env(char **env)
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
