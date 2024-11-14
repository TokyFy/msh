/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:22:04 by franaivo          #+#    #+#             */
/*   Updated: 2024/10/10 12:44:14 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t g_signal_received;

t_list* exec_herdoc(void* ast)
{
	static t_list *heredocs = NULL;
	t_node* node = ast;

	if(!ast)
	{
		if(!heredocs)
			return NULL;
		t_list *head = heredocs;
		heredocs = heredocs->next;
		return head;
	}
	setup_heredoc_signal_handling();
	if(node->type == CMD)
	{
		t_cmd* cmd = ast;
		t_list* redirs = cmd->redirs;
		while (redirs) {
			t_redir* redir = redirs->content;
			int fds[2];
			if(redir->type == HEREDOC)
			{
				pipe(fds);
				char *line = NULL;
				while (42) {
					if(line)
					{
						if(ft_strcmp(line, redir->string) == 0)
							break;
						ft_putendl_fd(line, fds[1]);
					}
					free(line);
					line = readline("heredoc < ");
				}
				close(fds[1]);
				t_heredoc *heredoc = malloc(sizeof(t_heredoc));
				heredoc->fd = fds[0];
				ft_lstadd_back(&heredocs, ft_lstnew(heredoc));
			}
			redirs = redirs->next;
		}
		return NULL;
	}
	if(node->type == PIPE)
	{
		t_pipe *pipe = ast;
		exec_herdoc(pipe->left);
		exec_herdoc(pipe->right);
	}
	return NULL;
}

int	main(const int argc, char **argv, char **env)
{
	char	*buff;
	char	*line;
	t_list	*tokens;
	t_list	*tokens_t;
	t_node	*ast;
	int status;

	(void)(argc);
	(void)(argv);
	(void)(env);
	status = 0;
	while (42)
	{
		setup_signal_handling();
		g_signal_received = 0;
		line = readline("> ");
		if(!line)
			exit(0);
		if(g_signal_received == SIGQUIT && !line)
			exit(EXIT_SUCCESS);
		buff = line;
		add_history(buff);
		tokens = tokenizer(&buff);
		tokens_t = tokens;
		ast = parse(&tokens);
		if(analyse_ast(ast))
		{
			if(fork() == 0)
			{
				exec_herdoc(ast);
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				exec_ast(ast);
				exit(1);
			}
			else
			{
				signal(SIGINT, SIG_IGN);
				wait(&status);
				if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
			}
		}
		free_tokens(tokens_t);
		free_ast(ast);
		free(line);
	}
}
