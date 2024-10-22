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

#include <msh.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void	exec_t_cmd(t_cmd *cmd, pid_t pid ,char **env)
{
	int		out;
	int		in;
	t_list	*redirs;
	t_redir	*redir;
	(void)(env);

	out = -1;
	in = -1;
	if (pid == 0)
	{
		redirs = cmd->redirs;
		while (redirs)
		{
			if(out != -1)
				close(out);

			if(in != -1)
				close(in);

			redir = redirs->content;
			if (redir->type == REDIR_O)
			{
				out = open(redir->string, O_CREAT | O_TRUNC | O_RDWR, 0644);
			}
			else if(redir->type == APPEND)
			{
				out = open(redir->string, O_CREAT | O_APPEND | O_RDWR, 0644);
			}
			else if(redir->type == REDIR_I)
			{
				in = open(redir->string, O_RDONLY);
			}
			redirs = redirs->next;
		}
		if (out != -1)
			dup2(out, STDOUT_FILENO);
		if(in != -1)
			dup2(in, STDIN_FILENO);
		// FORBIDEN FUNCT
		execvp(cmd->argv[0], cmd->argv);
		perror("msh");
		close(in);
		close(out);
	}
}

void	exec_ast(void *ast, void *parent , int stdout , char **env)
{
	t_node *node = ast;
	pid_t pid;
	int fd[2];
	t_pipe *pip;
	t_cmd *cmd;
	(void)(parent);

	if(node->type == CMD)
	{
		pid = fork();
		if(pid == 0)
		{
			pipe(fd);
			cmd = ast;
			dup2(stdout, STDIN_FILENO);
			execvp(cmd->argv[0], cmd->argv);
		}
		else {
			wait(NULL);
			return;
		}
	}

	if(node->type != PIPE)
		return;
	pipe(fd);
	pid = fork();
	pip = ast;
	if(pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		dup2(stdout, STDIN_FILENO);
		execvp(cmd->argv[0], cmd->argv);
		exec_ast(pip->right, ast, fd[0] , env);
	}
	else {
		wait(NULL);
	}
}
