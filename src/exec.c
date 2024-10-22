/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 10:24:45 by franaivo          #+#    #+#             */
/*   Updated: 2024/10/16 10:24:46 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void exec_t_cmd(t_cmd *cmd, char **env)
{
    int     out;
    int     in;
    t_list  *redirs;
    t_redir *redir;
    (void)(env);
    out = -1;
    in = -1;
    redirs = cmd->redirs;

    while (redirs)
    {
        if (out != -1)
        {
            if (close(out) == -1)
                perror("close");
        }
        if (in != -1)
        {
            if (close(in) == -1)
                perror("close");
        }

        redir = redirs->content;
        if (redir->type == REDIR_O)
        {
            out = open(redir->string, O_CREAT | O_TRUNC | O_RDWR, 0644);
            if (out == -1)
            {
                perror(redir->string);
                exit(1);
            }
        }
        else if (redir->type == APPEND)
        {
            out = open(redir->string, O_CREAT | O_APPEND | O_RDWR, 0644);
            if (out == -1)
            {
                perror(redir->string);
                exit(1);
            }
        }
        else if (redir->type == REDIR_I)
        {
            in = open(redir->string, O_RDONLY);
            if (in == -1)
            {
                perror(redir->string);
                exit(1);
            }
        }
        redirs = redirs->next;
    }

    if (out != -1)
    {
        if (dup2(out, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
    }
    if (in != -1)
    {
        if (dup2(in, STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
    }

    if (out != -1 && close(out) == -1)
        perror("close");
    if (in != -1 && close(in) == -1)
        perror("close");

    execvp(cmd->argv[0], cmd->argv);
    perror(cmd->argv[0]);
    exit(127);
}

void exec_ast(void *ast)
{
    t_node *node = ast;
    t_cmd *cmd = NULL;
    t_pipe *pipe_nodes;
    int fds[2];
    int pid1, pid2;
    int status;

    if (node->type == CMD)
    {
        cmd = ast;
        exec_t_cmd(cmd, NULL);
        return;
    }

    if (node->type == PIPE)
    {
        pipe_nodes = ast;
        if (pipe(fds) == -1)
        {
            perror("pipe");
            exit(1);
        }

        pid1 = fork();
        if (pid1 == -1)
        {
            perror("fork");
            exit(1);
        }

        if (pid1 == 0)
        {
            if (close(fds[1]) == -1)
                perror("close");
            if (dup2(fds[0], STDIN_FILENO) == -1)
            {
                perror("dup2");
                exit(1);
            }
            if (close(fds[0]) == -1)
                perror("close");
            exec_ast(pipe_nodes->right);
        }

        pid2 = fork();
        if (pid2 == -1)
        {
            perror("fork");
            exit(1);
        }

        if (pid2 == 0)
        {
            if (close(fds[0]) == -1)
                perror("close");
            if (dup2(fds[1], STDOUT_FILENO) == -1)
            {
                perror("dup2");
                exit(1);
            }
            if (close(fds[1]) == -1)
                perror("close");
            exec_ast(pipe_nodes->left);
        }

        if (close(fds[0]) == -1)
            perror("close");
        if (close(fds[1]) == -1)
            perror("close");

        if (waitpid(pid1, &status, 0) == -1)
            perror("waitpid");
        if (waitpid(pid2, &status, 0) == -1)
            perror("waitpid");

        exit(status);
    }
    return;
}
