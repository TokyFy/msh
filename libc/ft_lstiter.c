/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 15:44:51 by franaivo          #+#    #+#             */
/*   Updated: 2024/02/24 15:44:51 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst)
		return ;
	f(lst->content);
	ft_lstiter(lst->next, f);
}
void	**ft_lsttoarr(t_list *lst)
{
	t_size_t	len;
	void		**array;
	int			i;

	len = ft_lstsize(lst);
	array = malloc(sizeof(void *) * (len + 1));
	i = 0;
	if (!array)
		return (NULL);
	while (lst)
	{
		array[i] = lst->content;
		i++;
		lst = lst->next;
	}
	array[len] = NULL;
	return (array);
}
