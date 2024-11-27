/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 10:24:45 by franaivo          #+#    #+#             */
/*   Updated: 2024/11/26 10:34:51 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <msh.h>
#include <string.h>
#include <unistd.h>

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

void	redirect(t_cmd *cmd)
{
	int	out;
	int	in;

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
}

void	exec_t_cmd(t_cmd *cmd, char **env)
{
	(void)(env);
	redirect(cmd);
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
	{
		if (exec_builtings((t_node *)ast) == -1)
			exec_t_cmd((t_cmd *)ast, NULL);
		_false();
	}
	if (((t_node *)ast)->type == PIPE)
	{
		exec_pipe(ast, &pid1, &pid2);
		ft_waitpid(pid1, &status, 0);
		ft_waitpid(pid2, &status, 0);
		_false();
	}
}

int	builtin_cd(t_cmd *cmd)
{
	char *path = cmd->argv[1];
	if (!path)
		path = get_env(*static_env(NULL), "HOME");
	if(!path)
	{
		ft_putendl_fd("cd : Path required", STDERR_FILENO);
		return 1;
	}
	if (chdir(path) != 0)
	{
		perror("cd :");
		return (1);
	}
	return (0);
}

int	is_builting(t_cmd *cmd)
{
	char	*exec;

	if (!cmd || cmd->type != CMD)
		return (0);

	exec = cmd->argv[0];
	return (ft_strcmp(exec, "cd") == 0 || ft_strcmp(exec, "export") || ft_strcmp(exec,
			"env") || ft_strcmp(exec, "unset"));
}

int	exec_builtings(t_node *ast)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)ast;
	if (!is_builting(cmd))
		return (-1);
	redirect(cmd);
	if (strcmp("cd", cmd->argv[0]) == 0)
		return (builtin_cd(cmd));
	else if (strcmp("export", cmd->argv[0]) == 0)
		return (ft_export(cmd));
	else if (ft_strcmp("env", cmd->argv[0]) == 0)
		return (ft_env(cmd));
	else if (ft_strcmp("unset", cmd->argv[0]) == 0)
		return (ft_unset(cmd));
	else if (ft_strcmp("pwd", cmd->argv[0]) == 0)
		return (pwd());
	else if (ft_strcmp("echo", cmd->argv[0]) == 0)
		return (echo(cmd));
	else if (ft_strcmp("exit", cmd->argv[0]) == 0)
		return (msh_exit(cmd));
	return (-1);
}

int exec_high_level_builting(t_node* ast)
{
	if(ast->type != CMD)
		return -1;

	t_cmd *cmd = (t_cmd*)ast;
	char *exec = cmd->argv[0];

	if(!(strcmp(exec, "export") == 0 || strcmp(exec, "unset") || strcmp(exec, "cd")))
		return -1;

	if (strcmp("cd", cmd->argv[0]) == 0)
		return (builtin_cd(cmd));
	else if (strcmp("export", cmd->argv[0]) == 0)
		return (ft_export(cmd));
	else if (ft_strcmp("unset", cmd->argv[0]) == 0)
		return (ft_unset(cmd));

	return -1;
}

int	execute(t_node *ast, char **env)
{
	int	status;

	status = 0;
	if (analyse_ast(ast) && exec_high_level_builting((ast)) == -1)
	{
		if (fork() == 0)
		{
			expand(copy_env(env), ast);
			exec_heredoc(ast);
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			exec_ast(ast);
		}
		else
		{
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			wait(&status);
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
		}
	}
	return (status);
}
