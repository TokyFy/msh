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

#include <fcntl.h>
#include <msh.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void	exec_t_cmd(t_cmd *cmd, char **env)
{
	int		out;
	int		in;
	t_list	*redirs;
	t_redir	*redir;

	(void)(env);
	out = -1;
	in = -1;
	redirs = cmd->redirs;
	while (redirs)
	{
		if (out != -1)
		{
			if (close(out) == -1)
				perror("close");
		}
		if (in != -1)
		{
			if (close(in) == -1)
				perror("close");
		}
		redir = redirs->content;
		if (redir->type == REDIR_O)
		{
			out = open(redir->string, O_CREAT | O_TRUNC | O_RDWR, 0644);
			if (out == -1)
			{
				perror(redir->string);
				exit(1);
			}
		}
		else if (redir->type == APPEND)
		{
			out = open(redir->string, O_CREAT | O_APPEND | O_RDWR, 0644);
			if (out == -1)
			{
				perror(redir->string);
				exit(1);
			}
		}
		else if (redir->type == REDIR_I)
		{
			in = open(redir->string, O_RDONLY);
			if (in == -1)
			{
				perror(redir->string);
				exit(1);
			}
		}
		redirs = redirs->next;
	}
	if (out != -1)
	{
		if (dup2(out, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
	}
	if (in != -1)
	{
		if (dup2(in, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
	}
	if (out != -1 && close(out) == -1)
		perror("close");
	if (in != -1 && close(in) == -1)
		perror("close");
	execvp(cmd->argv[0], cmd->argv);
	perror(cmd->argv[0]);
	exit(127);
}

void perrorexit(const char *error)
{
	perror(error);
	exit(1);
}

void ft_pipe(int fds[2])
{
	if(pipe(fds) == -1)
	{
		perror("Pipe");
		exit(EXIT_FAILURE);
	}
}

pid_t ft_fork()
{
	pid_t pid = fork();
	if(pid == -1)
	{
		perror("Fork");
		exit(EXIT_FAILURE);
	}
	return pid;
}

void ft_dup2(int old , int new)
{
	if(dup2(old, new) == -1)
	{
		perror("Dup");
		exit(EXIT_FAILURE);
	}
	return;
}

void ft_close(int fd)
{
	if(close(fd) == -1)
	{
		perror("Close");
		exit(EXIT_FAILURE);
	}
}

void ft_waitpid(pid_t pid , int *status , int op)
{
	if (waitpid(pid, status, op) == -1)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
}

void	exec_ast(void *ast)
{
	t_node	*node;
	t_cmd	*cmd;
	t_pipe	*pipe_nodes;
	int		fds[2];
	int		status;
	int pid1;
	int pid2;
	node = ast;
	cmd = NULL;
	if (node->type == CMD)
	{
		cmd = ast;
		exec_t_cmd(cmd, NULL);
		return ;
	}
	if (node->type == PIPE)
	{
		pipe_nodes = ast;
		ft_pipe(fds);
		pid1 = ft_fork();
		if (pid1 == 0)
		{
			ft_close(fds[1]);
			ft_dup2(fds[0], STDIN_FILENO);
			ft_close(fds[0]);
			exec_ast(pipe_nodes->right);
		}
		pid2 = ft_fork();
		if (pid2 == 0)
		{
			ft_close(fds[0]);
			ft_dup2(fds[1], STDOUT_FILENO);
			ft_close(fds[1]);
			exec_ast(pipe_nodes->left);
		}
		ft_close(fds[0]);
		ft_close(fds[1]);
		ft_waitpid(pid1, &status, 0);
		ft_waitpid(pid2, &status, 0);
		exit(status);
	}
	return ;
}
