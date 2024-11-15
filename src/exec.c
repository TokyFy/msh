/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 10:24:45 by franaivo          #+#    #+#             */
/*   Updated: 2024/10/16 10:24:46 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <fcntl.h>
#include <msh.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int	ft_open(char *file, int oflags, int iflags)
{
	int	fd;

	fd = open(file, oflags, iflags);
	if (fd == -1)
	{
		perror("Open");
	}
	return (fd);
}

void	perrorexit(const char *error)
{
	perror(error);
	exit(1);
}

void	ft_pipe(int fds[2])
{
	if (pipe(fds) == -1)
	{
		perror("Pipe");
	}
}

pid_t	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork");
	}
	return (pid);
}

void	ft_dup2(int old, int new)
{
	if (dup2(old, new) == -1)
	{
		perror("Dup");
	}
	return ;
}

void	ft_close(int fd)
{
	if (close(fd) == -1)
	{
		perror("Close");
	}
}

void	ft_waitpid(pid_t pid, int *status, int op)
{
	if (waitpid(pid, status, op) == -1)
	{
		perror("waitpid");
	}
}

void	setup_redir(int *in, int *out, t_cmd *cmd)
{
	t_list	*redirs;
	t_redir	*redir;
	t_list	*heredoc;

	if (!cmd->redirs)
		return ;
	redirs = cmd->redirs;
	redir = redirs->content;
	while (redirs)
	{
		redir = redirs->content;
		if (*out != -1 && (redir->type == APPEND || redir->type == REDIR_O))
			ft_close(*out);
		if (*in != -1 && (redir->type == REDIR_I || redir->type == HEREDOC))
			ft_close(*in);
		if (redir->type == REDIR_O)
			*out = ft_open(redir->string, O_CREAT | O_TRUNC | O_RDWR, 0644);
		else if (redir->type == APPEND)
			*out = ft_open(redir->string, O_CREAT | O_APPEND | O_RDWR, 0644);
		else if (redir->type == REDIR_I)
			*in = ft_open(redir->string, O_RDONLY, 0644);
		else if (redir->type == HEREDOC)
		{
			heredoc = exec_heredoc(NULL);
			*in = ((t_heredoc *)heredoc->content)->fd;
		}
		redirs = redirs->next;
	}
}

void	_false(void)
{
	char	**args;

	args = (char *[2]){"false", NULL};
	execvp("false", args);
}

int	ft_execvp(const char *__file, char *const __argv[])
{
	execvp(__file, __argv);
	ft_putstr_fd((char *)__file, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	_false();
	return (0);
}

void	exec_t_cmd(t_cmd *cmd, char **env)
{
	int	out;
	int	in;

	(void)(env);
	out = -1;
	in = -1;
	setup_redir(&in, &out, cmd);
	if (out != -1)
	{
		ft_dup2(out, STDOUT_FILENO);
		ft_close(out);
	}
	if (in != -1)
	{
		ft_dup2(in, STDIN_FILENO);
		ft_close(in);
	}
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

	if (((t_node *)ast)->type == CMD)
		exec_t_cmd((t_cmd *)ast, NULL);
	if (((t_node *)ast)->type == PIPE)
	{
		exec_pipe(ast, &pid1, &pid2);
		ft_waitpid(pid1, &status, 0);
		ft_waitpid(pid2, &status, 0);
		_false();
	}
}
