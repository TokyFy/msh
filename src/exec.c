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
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void	exec_t_cmd(t_cmd *cmd, char **env)
{
	pid_t	pid;
	int		out;
	int		in;
	t_list	*redirs;
	t_redir	*redir;
	(void)(env);

	out = -1;
	in = -1;
	(void)(in);
	pid = fork();
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
		close(in);
		close(out);
		perror("msh");
	}
}

void	exec_ast(void *ast, char **env)
{
	t_node	*node;

	node = ast;
	if (node->type == CMD)
	{
		exec_t_cmd((t_cmd *)ast, env);
		wait(NULL);
	}
	return ;
}
