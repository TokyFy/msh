/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 14:34:33 by franaivo          #+#    #+#             */
/*   Updated: 2024/11/13 14:22:03 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_H
# define MSH_H

# include <libft.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>

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

int	ft_strcmp(char *s1, char *s2);
void	free_env(t_list *env);

t_node* parse(t_list **tokens);
void print_ast(void* ast , int level);
t_node		*parse(t_list **tokens);
void		print_ast(void *ast, int level);
void		free_ast(void *ast);

void		msh_env(t_list *env);

void exec(void *ast);

void    expand(t_list *env, void *tree);
char    *get_env_name_in_string(char *str);
char    *get_element_value(t_list *element);
char    *handle_quote(char *input);

char *get_env(t_list *env, char *name);
t_list  *copy_env(char **env);
void	builtin_env(t_list *env);
t_list	*env_exist(t_list *env, char *name);
int	builtin_export(t_list *env, char *str);
int	builtin_unset(t_list *env, char *name);

void handle_sigint(int sig);
void setup_signal_handling(void);
#endif
