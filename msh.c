/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:22:04 by franaivo          #+#    #+#             */
/*   Updated: 2024/11/15 15:56:43 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

volatile sig_atomic_t	g_signal_received;

void	write_heredoc(t_redir *redir, int fd)
{
	char	*line;

	line = NULL;
	while (42)
	{
		if (line)
		{
			if (ft_strcmp(line, redir->string) == 0)
				break ;
			ft_putendl_fd(line, fd);
		}
		free(line);
		line = readline("heredoc < ");
	}
}

void	feed_heredoc(t_cmd *cmd, t_list **heredocs)
{
	t_list		*redirs;
	t_redir		*redir;
	int			fds[2];
	t_heredoc	*heredoc;

	redirs = cmd->redirs;
	while (redirs)
	{
		redir = redirs->content;
		if (redir->type == HEREDOC)
		{
			pipe(fds);
			write_heredoc(redir, fds[1]);
			close(fds[1]);
			heredoc = malloc(sizeof(t_heredoc));
			heredoc->fd = fds[0];
			ft_lstadd_back(heredocs, ft_lstnew(heredoc));
		}
		redirs = redirs->next;
	}
}

t_list	*exec_heredoc(void *ast)
{
	static t_list	*heredocs = NULL;
	t_node			*node;
	t_list			*head;

	node = ast;
	if (!ast)
	{
		if (!heredocs)
			return (NULL);
		head = heredocs;
		heredocs = heredocs->next;
		return (head);
	}
	setup_heredoc_signal_handling();
	if (node->type == CMD)
	{
		feed_heredoc(ast, &heredocs);
		return (NULL);
	}
	if (node->type == PIPE)
	{
		exec_heredoc(((t_pipe *)ast)->left);
		exec_heredoc(((t_pipe *)ast)->right);
	}
	return (NULL);
}

void	*parser(char *line)
{
	t_list	*tokens;
	t_list	*tokens_t;
	void	*ast;

	add_history(line);
	tokens = tokenizer(&line);
	tokens_t = tokens;
	ast = parse(&tokens);
	free_tokens(tokens_t);
	return (ast);
}

int	execute(t_node *ast , char** env)
{
	int status;
	if (analyse_ast(ast))
	{
		if (fork() == 0)
		{
			expand(copy_env(env), ast);
			exec_heredoc(ast);
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			exec_ast(ast);
			exit(1);
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

int	main(const int argc, char **argv, char **e)
{
	char	*line;
	t_node	*ast;
	int		status;

	(void)(argc);
	(void)(argv);
	(void)(status);
	while (42)
	{
		setup_signal_handling();
		line = readline("> ");
		if (!line)
			exit(0);
		ast = parser(line);
		status = execute(ast , e);
		free_ast(ast);
		free(line);
	}
}
