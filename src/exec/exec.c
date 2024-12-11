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

char	*ft_strjoins(char **str, char *delim)
{
	char	*joined;
	char	*tmp;

	joined = ft_strdup("");
	while (*str)
	{
		tmp = ft_strjoin(joined, delim);
		free(joined);
		joined = tmp;
		tmp = ft_strjoin(joined, *str);
		free(joined);
		joined = tmp;
		str++;
	}
	return (joined);
}

void	flatten_t_cmd(t_cmd **cmd)
{
	char	*joined;
	char	*tmp_char;
	t_list	*token;
	t_list	*lst;
	t_list	*tmp_lst;

	joined = ft_strjoins((*cmd)->argv, " ");
	tmp_char = joined;
	token = tokenizer(&tmp_char);
	lst = NULL;
	tmp_lst = token;
	while (tmp_lst)
	{
		ft_lstadd_back(&lst, ft_lstnew(((t_token *)tmp_lst->content)->value));
		tmp_lst = tmp_lst->next;
	}
	free_tokens(token);
	free(joined);
	free_array_nulled((*cmd)->argv);
	(*cmd)->argv = (char **)ft_lsttoarr(lst);
	ft_lstclear(&lst, NULL);
}

void	remove_quote_t_cmd(t_cmd *cmd)
{
	char	**argv;
	char	*tmp_str;

	argv = cmd->argv;
	while (*argv)
	{
		tmp_str = remove_quotes(*argv);
		free(*argv);
		*argv = tmp_str;
		argv++;
	}
}

void	exec_ast(void *ast)
{
	int	status;
	int	pid1;
	int	pid2;

	status = 0;
	if (((t_node *)ast)->type == CMD)
	{
		flatten_t_cmd((t_cmd **)&ast);
		remove_quote_t_cmd((t_cmd *)ast);
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
