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

extern volatile sig_atomic_t	g_signal_received;

void	handle_sigint(int sig)
{
	g_signal_received = sig;
	set_status(130 << 8);
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	g_signal_received = sig;
}

void	handle_sig_heredoc(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	_exit2(1);
}

void	setup_signal_handling(void)
{
	struct sigaction	sa;

	g_signal_received = 0;
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_heredoc_signal_handling(void)
{
	struct sigaction	sa;
	struct sigaction	sa1;

	sa.sa_handler = handle_sig_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa1.sa_handler = handle_sig_heredoc;
	sigemptyset(&sa1.sa_mask);
	sa1.sa_flags = 0;
	sigaction(SIGQUIT, &sa1, NULL);
}
