#include "libft.h"
#include <msh.h>
#include <stdlib.h>
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

void* parse_cmd(t_list **tokens)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    t_list *argv = NULL;
    ft_bzero(cmd, sizeof(t_cmd));
    cmd->type = CMD;
    t_token *token = (*tokens)->content;
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
