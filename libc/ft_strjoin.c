/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:02:09 by franaivo          #+#    #+#             */
/*   Updated: 2024/01/29 13:16:22 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	t_size_t	l_s1;
	t_size_t	l_s2;
	t_size_t	l_joined;
	char		*joined;

	l_s1 = ft_strlen(s1);
	l_s2 = ft_strlen(s2);
	l_joined = l_s1 + l_s2 + 1;
	joined = ft_calloc(l_joined, sizeof(char));
	if (!joined)
		return (0);
	ft_strlcat(joined, (char *)s1, l_joined);
	ft_strlcat(joined, (char *)s2, l_joined);
	return (joined);
}

char	*ft_strjoins(char **str, char *delim)
{
	char	*joined;
	char	*tmp;

	joined = ft_strdup("");
	while (*str)
	{
		tmp = ft_strjoin(joined, delim);
		free(joined);
		joined = tmp;
		tmp = ft_strjoin(joined, *str);
		free(joined);
		joined = tmp;
		str++;
	}
	return (joined);
}
