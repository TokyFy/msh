/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sranaivo <sranaivo@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 14:34:33 by franaivo          #+#    #+#             */
/*   Updated: 2024/12/11 15:17:38 by sranaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_H
# define MSH_H

# include <fcntl.h>
# include <libft.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <time.h>
# include <unistd.h>

# define WHITESPACE " \t\r\n\v"
# define SYMBOL "<|>"
# define HEREDOC_ERROR "msh: warning: here-document delimited by end-of-file"

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
int			analyse_ast(void *tree);
void		exec_ast(void *ast);

typedef struct s_node
{
	t_type	type;
}			t_node;

typedef struct s_redirs
{
	t_type	type;
	char	*string;
	int		fd;
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

void		free_env(t_list *env);

t_node		*parse(t_list **tokens);
void		print_ast(void *ast, int level);
void		free_ast(void *ast);
void		free_array_nulled(char **array);
void		*parser(char *line);
int			execute(t_node *ast);
int			exec_builtings(t_node *ast);
int			parse_redir(t_cmd *cmd, t_list **tokens);
void		fill_t_cmd(t_cmd *cmd, t_list **tokens);

void		redirect(t_cmd *cmd);
typedef struct s_heredoc
{
	int		fd;
}			t_heredoc;

char		*remove_quotes(char *input);
void		remove_translate(char **input);
int			get_status(void);
void		set_status(int value);
int			ft_unset(t_cmd *cmd);
int			ft_env(t_cmd *cmd);
int			ft_export(t_cmd *cmd);
int			msh_exit(t_cmd *cmd);
int			builtin_cd(t_cmd *cmd);
int			pwd(void);
int			echo(t_cmd *cmd);
t_list		**static_env(char **e);
char		*get_env(t_list *env, char *name);
t_list		*copy_env(char **env);
int			builtin_env(t_list *env);
t_list		*env_exist(t_list *env, char *name);
int			builtin_export(t_list **env, char *str);
void		export_without_argument(t_list *env_list);
int			builtin_unset(t_list **env, char *name);
char		**list_to_env_array(t_list *env_list);
t_env		*new_env(char *str);
void		set_env(char *env, char *value);
int			is_valid_env_name(char *name);
char		**list_to_env_array(t_list *env_list);
int			are_all_env_names_valid(char **names);

void		remove_quote(char *input, char *current_quote, int *j,
				char **result);
char		*handle_quote(char *input);
void		toggle_quotes(char c, int *in_single_quotes, int *in_double_quotes);
int			is_valid_quotes(char *str);
int			check_quote_status(char *input, int *i, int *in_single_quotes,
				int *in_double_quotes);
char		*append_text(char *result, char *input, int start, int end);
char		*trim_space(const char *str);

char		*get_env_name_in_string(char *string);
char		*get_element_value(t_list *element);
char		*expand_variable_if_exists(t_list *env, char *result, char *input,
				int *i);
char		*expand_variables_in_string(t_list *env, char *input);
void		expand_main_loop(t_list *env, char ***temp);
int			exec_high_level_builting(t_node *ast);
void		expand(void *tree);

int			ft_open(char *file, int oflags, int iflags);
pid_t		ft_fork(void);
void		ft_pipe(int fds[2]);
void		ft_dup2(int old, int news);
void		ft_close(int fd);
void		perrorexit(const char *error);
void		ft_waitpid(pid_t pid, int *status, int op);
int			ft_execvp(const char *__file, char *__argv[]);

t_list		*exec_heredoc(void *ast);
void		handle_sigint(int sig);
void		setup_signal_handling(void);
void		setup_heredoc_signal_handling(void);
const char	*shell_path(char **argv);
void		_exit2(int status);


void	debug_array_nulled(char **array);
void	flatten_t_cmd(t_cmd **cmd);
#endif
