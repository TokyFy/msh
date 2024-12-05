/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antananariv>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 22:24:17 by franaivo          #+#    #+#             */
/*   Updated: 2024/12/05 19:15:14 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

void	write_heredoc(t_redir *redir, int fd)
{
	char	*line;
	int		quoted;

	line = NULL;
	quoted = ft_strchr(redir->string, '\"') != NULL;
	redir->string = remove_quotes(redir->string);
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

void	feed_heredoc(t_cmd *cmd)
{
	t_list	*redirs;
	t_redir	*redir;
	int		fds[2];

	redirs = cmd->redirs;
	while (redirs)
	{
		redir = redirs->content;
		if (redir->type == HEREDOC)
		{
			pipe(fds);
			write_heredoc(redir, fds[1]);
			close(fds[1]);
			redir->fd = fds[0];
		}
		redirs = redirs->next;
	}
}

t_list	*exec_heredoc(void *ast)
{
	t_node	*node;

	node = ast;
	setup_heredoc_signal_handling();
	if (node->type == CMD)
	{
		feed_heredoc(ast);
		return (NULL);
	}
	if (node->type == PIPE)
	{
		exec_heredoc(((t_pipe *)ast)->left);
		exec_heredoc(((t_pipe *)ast)->right);
	}
	return (NULL);
}
