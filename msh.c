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

#include "libc/libft.h"
#include <msh.h>
#include <libft.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

volatile sig_atomic_t g_signal_received;

typedef struct s_heredoc{
	int fd;
} t_heredoc;

void exec_herdoc(void* ast)
{
	static t_list *heredocs = NULL;
	t_node* node = ast;

	(void)(heredocs);
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
				while (line == NULL || ft_strcmp(line, redir->string) != 0) {
					if(line)
					{
						ft_putstr_fd(line, fds[1]);
						ft_putstr_fd("\n", fds[1]);
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
	}
	if(node->type == PIPE)
	{
		t_pipe *pipe = ast;
		exec_herdoc(pipe->left);
		exec_herdoc(pipe->right);
	}
	return;
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
	setup_signal_handling();
	while (42)
	{
		line = readline("> ");
		buff = line;
		add_history(buff);
		tokens = tokenizer(&buff);
		tokens_t = tokens;
		ast = parse(&tokens);
		if(analyse_ast(ast))
		{
			if(fork() == 0)
			{
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				exec_herdoc(ast);
				exit(EXIT_SUCCESS);
				exec_herdoc(ast);
				exec_ast(ast);
			}
			else
			{
				signal(SIGINT, SIG_IGN);
				wait(&status);
				setup_signal_handling();
				if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
			}
		}
		else
		  printf("Error\n");
		free_tokens(tokens_t);
		free_ast(ast);
		free(line);
	}
}
