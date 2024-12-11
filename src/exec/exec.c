/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 10:24:45 by franaivo          #+#    #+#             */
/*   Updated: 2024/11/29 15:26:15 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <msh.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	exec_t_cmd(t_cmd *cmd, char **env)
{
	(void)(env);
	redirect(cmd);
	if (cmd->argv[0])
		ft_execvp(cmd->argv[0], cmd->argv);
}

void	exec_pipe(void *ast, int *pid1, int *pid2)
{
	int	fds[2];

	ft_pipe(fds);
	*pid1 = ft_fork();
	if (*pid1 == 0)
	{
		ft_close(fds[1]);
		ft_dup2(fds[0], STDIN_FILENO);
		ft_close(fds[0]);
		exec_ast(((t_pipe *)ast)->right);
	}
	*pid2 = ft_fork();
	if (*pid2 == 0)
	{
		ft_close(fds[0]);
		ft_dup2(fds[1], STDOUT_FILENO);
		ft_close(fds[1]);
		exec_ast(((t_pipe *)ast)->left);
	}
	ft_close(fds[0]);
	ft_close(fds[1]);
}

void	exec_ast(void *ast)
{
	int	status;
	int	pid1;
	int	pid2;

	status = 0;
	if (((t_node *)ast)->type == CMD)
	{
		status = exec_builtings((t_node *)ast);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (status == -1)
			exec_t_cmd((t_cmd *)ast, NULL);
		_exit2(status);
	}
	else if (((t_node *)ast)->type == PIPE)
	{
		exec_pipe(ast, &pid1, &pid2);
		ft_waitpid(pid2, NULL, 0);
		ft_waitpid(pid1, &status, 0);
		_exit2(status);
	}
}

int	execute(t_node *ast)
{
	int	status;
	int	analyse;

	status = exec_high_level_builting((ast));
	analyse = analyse_ast(ast);
	if (!analyse)
		return (2 << 8);
	if (analyse && status == -1)
	{
		if (fork() == 0)
		{
			expand(ast);
			exec_heredoc(ast);
			exec_ast(ast);
		}
		else
		{
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			wait(&status);
			if (WIFSIGNALED(status))
				return (write(1, "\n", 1), ((128 + WTERMSIG(status)) << 8));
		}
	}
	return (ft_abs(status));
}
