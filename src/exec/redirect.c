/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antananariv>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:20:26 by franaivo          #+#    #+#             */
/*   Updated: 2024/12/05 19:14:18 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

void	setup_redir(int *in, int *out, t_cmd *cmd)
{
	t_list	*redirs;
	t_redir	*redir;

	if (!cmd->redirs)
		return ;
	redirs = cmd->redirs;
	redir = redirs->content;
	while (redirs)
	{
		redir = redirs->content;
		if (*out != -1 && (redir->type == APPEND || redir->type == REDIR_O))
			ft_close(*out);
		if (*in != -1 && (redir->type == REDIR_I || redir->type == HEREDOC))
			ft_close(*in);
		if (redir->type == REDIR_O)
			*out = ft_open(redir->string, O_CREAT | O_TRUNC | O_RDWR, 0644);
		else if (redir->type == APPEND)
			*out = ft_open(redir->string, O_CREAT | O_APPEND | O_RDWR, 0644);
		else if (redir->type == REDIR_I)
			*in = ft_open(redir->string, O_RDONLY, 0644);
		else if (redir->type == HEREDOC)
			*in = redir->fd;
		redirs = redirs->next;
	}
}

void	redirect(t_cmd *cmd)
{
	int	out;
	int	in;

	out = -1;
	in = -1;
	setup_redir(&in, &out, cmd);
	if (out != -1)
	{
		ft_dup2(out, STDOUT_FILENO);
		ft_close(out);
	}
	if (in != -1)
	{
		ft_dup2(in, STDIN_FILENO);
		ft_close(in);
	}
}
