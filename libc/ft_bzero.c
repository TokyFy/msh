/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 16:03:55 by franaivo          #+#    #+#             */
/*   Updated: 2024/02/22 10:29:01 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_bzero(void *s, unsigned int n)
{
	ft_memset(s, '\0', n);
}
