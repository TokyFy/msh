/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 10:24:45 by franaivo          #+#    #+#             */
/*   Updated: 2024/11/18 16:15:48 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

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

int builtin_cd(t_cmd* cmd) {
	if(cmd->argv[2])
	{
		printf("cd : 1 argument expected\n");
		return 1;
	}
    if (chdir(cmd->argv[1]) != 0) {
        perror("cd :");
        return 1;
    }
    return 0;
}

int exec_builtings(t_node *ast)
{
	if(ast->type != CMD)
		return -1;

	t_cmd* cmd = (t_cmd*)ast;

	if(strcmp("cd", cmd->argv[0]) == 0)
		return builtin_cd(cmd);
	else if (strcmp("export", cmd->argv[0]) == 0)
		return export(cmd);
	else if (ft_strcmp("env", cmd->argv[0]) == 0)
		return (env(cmd));
	else if (ft_strcmp("unset", cmd->argv[0]) == 0)
		return (unset(cmd));

	return -1;
}

int	execute(t_node *ast , char** env)
{
	int status = 0;
	if (analyse_ast(ast) && exec_builtings(ast) == -1)
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

// int	execute(t_node *ast , char** env)
// {
// 	int status = 0;
// 	if (analyse_ast(ast))
// 	{
// 		if (fork() == 0)
// 		{
// 			expand(copy_env(env), ast);
// 			exec_heredoc(ast);
// 			signal(SIGINT, SIG_DFL);
// 			signal(SIGQUIT, SIG_DFL);
// 			exec_ast(ast);
// 		}
// 		else
// 		{
// 			signal(SIGINT, SIG_IGN);
// 			signal(SIGQUIT, SIG_IGN);
// 			wait(&status);
// 			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
// 				write(STDOUT_FILENO, "\n", 1);
// 		}
// 	}
// 	return (status);
// }
