#include <msh.h>

void print_token(const t_token *token)
{
    char* types[] = {"WORD" , "REDIR_I" , "REDIR_O" , "APPEND" , "PIPE" , "HEREDOC"};
    if(!token)
    {
        printf("| NULL |\n");
        return;
    }
    printf("| TYPE : %-10s " , types[token->type]);
    printf("VALUE : %-10s |\n" , token->value);
}

t_token*	get_word(char **buff)
{
    char *str;
    t_token *token;
    str = *buff;
    char quoted = 0;
    if(!*str)
        return NULL;
    while(*str && (quoted || (!ft_strchr(SYMBOL , *str) && !ft_strchr(WHITESPACE , *str))))
    {
        if(!quoted && ft_strchr("\"\'" , *str))
            quoted = *str;
        else if (quoted && quoted == *str)
            quoted = 0;
        str++;
    }
    token = malloc(sizeof(t_token));
    token->type = WORD;
    token->value = ft_strndup(*buff , str - *buff);
    *buff = str;
    return token;
}

t_token*	get_operator(char **buff)
{
    t_token *token = malloc(sizeof(t_token));
    token->value = NULL;

    if(**buff == '<')
    {
        if(*(*buff + 1) == '<')
        {
            (*buff) += 2;
            token->type = HEREDOC;
            return token;
        }
        (*buff)++;
        token->type = REDIR_I;
        return token;
    }
    if (**buff == '>')
    {
        if(*(*buff + 1) == '>')
        {
            (*buff) += 2;
            token->type = APPEND;
            return token;
        }
        (*buff)++;
        token->type = REDIR_O;
        return token;
    }
    if(**buff == '|')
    {
        (*buff)++;
        token->type = PIPE;
        return token;
    }
    return NULL;
}

t_token*	get_token(char **buff)
{
    if(!buff || !*buff)
        return NULL;
    char	*str;
    while (**buff && ft_strchr(WHITESPACE, **buff))
        (*buff)++;
    str = *buff;
    if(!*str)
        return NULL;
    if (strchr(SYMBOL, *str))
        return get_operator(buff);
    return (get_word(buff));
}
