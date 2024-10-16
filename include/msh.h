#ifndef MSH_H
#define MSH_H

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <libft.h>

#define WHITESPACE " \t\r\n\v"
#define SYMBOL "<|>"

typedef enum e_type
{
    WORD = 0,
    REDIR_I,
    REDIR_O,
    APPEND,
    PIPE,
    HEREDOC,
    CMD
} t_type;

typedef struct s_token
{
    t_type type;
    char *value;
} t_token;

t_token*	get_token(char **buff);
t_token*	get_operator(char **buff);
t_token*	get_word(char **buff);
void        print_token(const t_token *token);
t_list      *tokenizer(char **buff);

typedef struct s_node
{
    t_type type;
} t_node;

typedef struct s_redirs {
    t_type type;
    char *string;
} t_redir;

typedef struct s_cmd {
    t_type type;
    char **argv;
    t_list *redirs;
} t_cmd;

typedef struct s_pipe {
    t_type type;
    void* left;
    void* right;
} t_pipe;

typedef struct s_env
{
    char *name;
    char *value;
} t_env;

int	ft_strcmp(char *s1, char *s2);
void	free_env(t_list *env);

t_node* parse(t_list **tokens);
void print_ast(void* ast , int level);


t_list  *get_env(char **env);
void	builtin_env(t_list *env);
t_list	*env_exist(t_list *env, char *name);
void	builtin_export(t_list *env, char *str);

#endif
