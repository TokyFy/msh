/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 10:09:54 by sranaivo          #+#    #+#             */
/*   Updated: 2024/11/29 12:05:24 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

int manage_status(int action, int value)
{
    static int status = 0;
    if (action == 1)
        status = value;
    else if (action == 0)
        return status;
    return 0;
}

int get_status(void)
{
    return manage_status(0, 0);
}

void set_status(int value)
{
    manage_status(1, value);
}
