/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:22:04 by franaivo          #+#    #+#             */
/*   Updated: 2024/10/07 11:22:05 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

int	main(const int argc, char **argv)
{
	t_token	*token;
	char	*buff;
	char	*line;

	(void)(argc);
	(void)(argv);

	while(1)
	{
		line = readline("> ");
		buff = line;
		add_history(buff);
		while ((token = get_token(&buff)))
			print_token(token);
		free(line);
	}
}
