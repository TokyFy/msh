/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 11:54:24 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/18 12:45:28 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

t_list  **static_env(char **e)
{
   static t_list   *env;


   if (e)
   {
       env = copy_env(e);
       return (NULL);
   }
   return (&env);
}
