/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:32:08 by franaivo          #+#    #+#             */
/*   Updated: 2024/10/28 12:32:10 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern volatile sig_atomic_t	g_signal_received;

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void handle_sigquit(int sig)
{
	g_signal_received = sig;
}

void	setup_signal_handling(void)
{
	struct sigaction	sa , sa1;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);

	sa1.sa_handler = handle_sigquit;
	sigemptyset(&sa1.sa_mask);
	sa1.sa_flags = 0;
	sigaction(SIGQUIT , &sa1 , NULL);
}
