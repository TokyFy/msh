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

void msh_signal_handler(int sig , siginfo_t *info , void *context)
{
	(void)(info);
	(void)(context);
	(void)(sig);
	printf("\n");
  	rl_on_new_line();
   	rl_replace_line("", 0);
    rl_redisplay();
	return;
}

void desable_signal()
{
	struct sigaction sa;
	sa.sa_sigaction = msh_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
}

void restore_signal()
{
    struct sigaction sa;
    sa.sa_handler = NULL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);
}
