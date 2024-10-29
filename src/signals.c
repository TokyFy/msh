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

extern volatile sig_atomic_t g_signal_received;

void handle_sigint(int sig)
{
    (void)sig;
    g_signal_received = 1;
    write(STDOUT_FILENO, "\n", 1);    // Print newline
    rl_on_new_line();                 // Inform readline about the newline
    rl_replace_line("", 0);           // Clear the current line
    rl_redisplay();                   // Redisplay the prompt
}

void setup_signal_handling(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}
