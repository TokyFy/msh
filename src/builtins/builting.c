/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franaivo <franaivo@student.42antananariv>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:15:08 by franaivo          #+#    #+#             */
/*   Updated: 2024/12/05 19:14:05 by franaivo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msh.h>

int	is_builting(t_cmd *cmd)
{
	char	*exec;

	if (!cmd || cmd->type != CMD)
		return (0);
	exec = cmd->argv[0];
	return (ft_strcmp(exec, "cd") == 0 || ft_strcmp(exec, "export") == 0
		|| ft_strcmp(exec, "env") == 0 || ft_strcmp(exec, "unset") == 0
		|| ft_strcmp(exec, "pwd") == 0 || ft_strcmp(exec, "echo") == 0
		|| ft_strcmp(exec, "exit") == 0);
}

int	exec_builtings(t_node *ast)
{
	t_cmd	*cmd;
	int		status;

	status = -1;
	cmd = (t_cmd *)ast;
	if (!is_builting(cmd))
		return (-1);
	redirect(cmd);
	if (strcmp("cd", cmd->argv[0]) == 0)
		status = (builtin_cd(cmd));
	else if (strcmp("export", cmd->argv[0]) == 0)
		status = (ft_export(cmd));
	else if (ft_strcmp("env", cmd->argv[0]) == 0)
		status = (ft_env(cmd));
	else if (ft_strcmp("unset", cmd->argv[0]) == 0)
		status = (ft_unset(cmd));
	else if (ft_strcmp("pwd", cmd->argv[0]) == 0)
		status = (pwd());
	else if (ft_strcmp("echo", cmd->argv[0]) == 0)
		status = (echo(cmd));
	else if (ft_strcmp("exit", cmd->argv[0]) == 0)
		status = (msh_exit(cmd));
	return (status);
}

int	exec_high_level_builting(t_node *ast)
{
	t_cmd	*cmd;
	int		status;

	if (!ast || ast->type != CMD || !((t_cmd *)ast)->argv[0])
		return (-1);
	cmd = (t_cmd *)ast;
	status = -1;
	if (!(ft_strcmp(cmd->argv[0], "export") == 0 || ft_strcmp(cmd->argv[0], "unset") == 0
			|| ft_strcmp(cmd->argv[0], "cd") == 0 || ft_strcmp(cmd->argv[0], "exit") == 0))
		return (-1);
	expand(ast);
	flatten_t_cmd(&cmd);
	remove_quote_t_cmd(cmd);
	if (ft_strcmp("cd", cmd->argv[0]) == 0)
		status = (builtin_cd(cmd));
	else if (ft_strcmp("export", cmd->argv[0]) == 0 && cmd->argv[1])
		status = (ft_export(cmd));
	else if (ft_strcmp("unset", cmd->argv[0]) == 0)
		status = (ft_unset(cmd));
	else if (ft_strcmp("exit", cmd->argv[0]) == 0)
		status = msh_exit(cmd);
	if (status != -1)
		return (status << 8);
	return (status);
}
