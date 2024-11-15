/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 14:34:33 by franaivo          #+#    #+#             */
/*   Updated: 2024/10/11 14:34:36 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_H
# define MSH_H

# include <libft.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

# define WHITESPACE " \t\r\n\v"
# define SYMBOL "<|>"

typedef enum e_type
{
	WORD = 0,
	REDIR_I,
	REDIR_O,
	APPEND,
	PIPE,
	HEREDOC,
	CMD
}			t_type;

typedef struct s_token
{
	t_type	type;
	char	*value;
}			t_token;

t_token		*get_token(char **buff);
t_token		*get_operator(char **buff);
t_token		*get_word(char **buff);
void		print_token(const t_token *token);
t_list		*tokenizer(char **buff);
void		free_tokens(t_list *tokens);
int analyse_ast(void *tree);
void	exec_ast(void *ast);
t_list* exec_heredoc(void* ast);

typedef struct s_node
{
	t_type	type;
}			t_node;

typedef struct s_redirs
{
	t_type	type;
	char	*string;
}			t_redir;

typedef struct s_cmd
{
	t_type	type;
	char	**argv;
	t_list	*redirs;
}			t_cmd;

typedef struct s_pipe
{
	t_type	type;
	void	*left;
	void	*right;
}			t_pipe;

typedef struct s_env
{
	char	*name;
	char	*value;
}			t_env;

typedef struct s_heredoc{
	int fd;
} t_heredoc;

t_node		*parse(t_list **tokens);
void		print_ast(void *ast, int level);
void		free_ast(void *ast);

t_list		*get_env(char **env);
void		msh_env(t_list *env);

void exec(void *ast);


void handle_sigint(int sig);
void setup_signal_handling(void);
void	setup_heredoc_signal_handling(void);


int ft_execvp(const char *__file, char *const __argv[]);
void	_false(void);
#endif
