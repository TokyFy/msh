#include "libft.h"
#include <msh.h>
#include <stdio.h>
#include <string.h>

int parse_redir(t_cmd* cmd , t_list **tokens)
{
    if(!cmd || !tokens)
        return -1;
    t_token *token = (*tokens)->content;
    t_token *token_next = NULL;
    if((*tokens)->next)
        token_next = ((*tokens)->next->content);
    if(!token_next || token_next->type != WORD)
        return -1;
    t_redir *redir = malloc(sizeof(t_redir));
    redir->type = token->type;
    redir->string = token_next->value;
    ft_lstadd_back(&cmd->redirs, ft_lstnew(redir));
    *tokens = (*tokens)->next;
    return 1;
}

void** ft_lsttoarr(t_list *lst)
{
    t_size_t len = ft_lstsize(lst);
    void **array = malloc(sizeof(void*) * (len + 1));
    int i = 0;

    if(!array)
        return NULL;
    while (lst) {
        array[i] = lst->content;
        i++;
        lst = lst->next;
    }
    array[len] = NULL;
    return array;
}

void* parse_cmd(t_list **tokens)
{
    if(!*tokens)
        return NULL;
    t_token *token = (*tokens)->content;
    if(token->type == PIPE)
        return NULL;
    t_cmd *cmd = malloc(sizeof(t_cmd));
    t_list *argv = NULL;
    ft_bzero(cmd, sizeof(t_cmd));
    cmd->type = CMD;
    while(token && token->type != PIPE)
    {
        if(token->type == WORD)
            ft_lstadd_back(&argv , ft_lstnew(token->value));
        else
            parse_redir(cmd , tokens);
        (*tokens) = (*tokens)->next;
        token = NULL;
        if((*tokens))
            token = (*tokens)->content;
    }
    cmd->argv = (char**)ft_lsttoarr(argv);
    ft_lstclear(&argv, NULL);
    return cmd;
}

t_node* parse(t_list **tokens)
{
    t_node *node = parse_cmd(tokens);
    if(!*tokens)
        return node;
    *tokens = (*tokens)->next;
    void *pipe = malloc(sizeof(t_pipe));
    ((t_pipe*)pipe)->type = PIPE;
    ((t_pipe*)pipe)->left = node;
    ((t_pipe*)pipe)->right = parse(tokens);
    return  pipe;
}

void debug_array_nulled(char** array)
{
    if(!array || !*array)
    {
         printf("NULL");
         return;
    }
    printf("[ ");
    while(*array)
    {
        printf("%s" , *array);
        if(*(array + 1))
            printf(", ");
        array++;
    }
    printf(" ]");
}

void debug_t_redirs(t_list *lst)
{
    if(!lst)
        printf("NULL");
    char* types[] = {"WORD" , "REDIR_I" , "REDIR_O" , "APPEND" , "PIPE" , "HEREDOC"};
    t_redir *redir = NULL;
    while(lst)
    {
        redir = lst->content;
        printf("[ %s : %s ] " , types[redir->type] , redir->string);
        lst = lst->next;
    }
}

void print_ast(void* tree , int level)
{
    int factor = 2;
    t_node *ast = tree;
    if(!ast)
    {
        printf("%*s", level * factor , " ");
        printf("NULL\n\n");
        return;
    }
    if(ast->type == CMD)
    {
        t_cmd *cmd = tree;
        printf("%*s", level * factor , " ");
        printf("CMD :\n");
        printf("%*s", level * factor , " ");
        printf("| Arg : ");
        debug_array_nulled(cmd->argv);
        printf("\n%*s", level * factor , " ");
        printf("| I/O : ");
        debug_t_redirs(cmd->redirs);
        printf("\n");
        return;
    }
    t_pipe *pipe = tree;
    printf("%*s", level * factor , " ");
    printf("PIPE :\n");
    print_ast(pipe->left, level + 1);
    print_ast(pipe->right, level + 1);
}
