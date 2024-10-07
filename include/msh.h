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

#endif

