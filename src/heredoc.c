#include "libft.h"
#include <msh.h>
#include <string.h>

void	write_heredoc(t_redir *redir, int fd)
{
	char	*line;
	int		quoted;

	line = NULL;
	quoted = ft_strchr(redir->string, '\"') != NULL;
	while (42)
	{
		if (line)
		{
			if (ft_strcmp(line, redir->string) == 0)
				break ;
			if (!quoted)
				line = expand_variables_in_string(*static_env(NULL), line);
			ft_putendl_fd(line, fd);
		}
		free(line);
		line = readline("heredoc < ");
	}
}

void	feed_heredoc(t_cmd *cmd, t_list **heredocs)
{
	t_list		*redirs;
	t_redir		*redir;
	int			fds[2];
	t_heredoc	*heredoc;

	redirs = cmd->redirs;
	while (redirs)
	{
		redir = redirs->content;
		if (redir->type == HEREDOC)
		{
			pipe(fds);
			write_heredoc(redir, fds[1]);
			close(fds[1]);
			heredoc = malloc(sizeof(t_heredoc));
			heredoc->fd = fds[0];
			ft_lstadd_back(heredocs, ft_lstnew(heredoc));
		}
		redirs = redirs->next;
	}
}

t_list	*exec_heredoc(void *ast)
{
	static t_list	*heredocs = NULL;
	t_node			*node;
	t_list			*head;

	node = ast;
	if (!ast)
	{
		if (!heredocs)
			return (NULL);
		head = heredocs;
		heredocs = heredocs->next;
		return (head);
	}
	setup_heredoc_signal_handling();
	if (node->type == CMD)
	{
		feed_heredoc(ast, &heredocs);
		return (NULL);
	}
	if (node->type == PIPE)
	{
		exec_heredoc(((t_pipe *)ast)->left);
		exec_heredoc(((t_pipe *)ast)->right);
	}
	return (NULL);
}
