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

#include <msh.h>
#include <stdlib.h>
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
	if(cmd->argv[0])
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
	int	status = 0;
	int	pid1;
	int	pid2;

	if (((t_node *)ast)->type == CMD)
	{
		status = exec_builtings((t_node *)ast);
		if (status == -1)
			exec_t_cmd((t_cmd *)ast, NULL);
		_exit2(WEXITSTATUS(status));
	}
	if (((t_node *)ast)->type == PIPE)
	{
		exec_pipe(ast, &pid1, &pid2);
		ft_waitpid(pid1, &status, 0);
		ft_waitpid(pid2, NULL, 0);
		_exit2(WEXITSTATUS(status));
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
		perror("cd");
		return (1);
	}
	path = getcwd(NULL, 0);
	set_env("PWD", path);
	return (0);
}

int	is_builting(t_cmd *cmd)
{
	char	*exec;

	if (!cmd || cmd->type != CMD)
		return (0);

	exec = cmd->argv[0];
	return (ft_strcmp(exec, "cd") == 0 || ft_strcmp(exec, "export") == 0 || ft_strcmp(exec,
			"env") == 0 || ft_strcmp(exec, "unset") == 0 || ft_strcmp(exec, "pwd") == 0 ||
			ft_strcmp(exec, "echo") == 0 || ft_strcmp(exec , "exit") == 0);
}

int	exec_builtings(t_node *ast)
{
	t_cmd	*cmd;
	int status = -1;
	cmd = (t_cmd *)ast;
	if (!is_builting(cmd))
		return (-1);
	redirect(cmd);
	if (strcmp("cd", cmd->argv[0]) == 0)
		status = (builtin_cd(cmd));
	else if (strcmp("export", cmd->argv[0]) == 0)
		status = (ft_export(cmd));
	else if (ft_strcmp("env", cmd->argv[0]) == 0)
		status = (ft_env(cmd));
	else if (ft_strcmp("unset", cmd->argv[0]) == 0)
		status = (ft_unset(cmd));
	else if (ft_strcmp("pwd", cmd->argv[0]) == 0)
		status = (pwd());
	else if (ft_strcmp("echo", cmd->argv[0]) == 0)
		status = (echo(cmd));
	else if (ft_strcmp("exit", cmd->argv[0]) == 0)
		status = (msh_exit(cmd));
	return (status);
}

int exec_high_level_builting(t_node* ast)
{
	if(!ast || ast->type != CMD || !((t_cmd*)ast)->argv[0])
		return -1;
	t_cmd *cmd = (t_cmd*)ast;
	char *exec = cmd->argv[0];
	int status = -1;
	if(!(ft_strcmp(exec, "export") == 0 || ft_strcmp(exec, "unset") == 0 || ft_strcmp(exec, "cd") == 0 || ft_strcmp(exec, "exit") == 0))
		return -1;
	expand(ast);
	if (ft_strcmp("cd", cmd->argv[0]) == 0)
		status = (builtin_cd(cmd));
	else if (ft_strcmp("export", cmd->argv[0]) == 0)
		status = (ft_export(cmd));
	else if (ft_strcmp("unset", cmd->argv[0]) == 0)
		status = (ft_unset(cmd));
	else if (ft_strcmp("exit", cmd->argv[0]) == 0)
		status = msh_exit(cmd);
	if(status != -1)
		return status << 8;
	return (status);
}

int	execute(t_node *ast, char **env)
{
	int	status;
	int analyse;

	(void) env;
	status = exec_high_level_builting((ast)) ;
	analyse = analyse_ast(ast);
	if(!analyse)
		return 2 << 8;
	if (analyse && status == -1)
	{
		if (fork() == 0)
		{
			exec_heredoc(ast);
			expand(ast);
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
	return ((int)ft_abs(status));
}
