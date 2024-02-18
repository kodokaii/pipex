/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaerema <nlaerema@student.42lehavre.fr>	+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 10:58:17 by nlaerema          #+#    #+#             */
/*   Updated: 2023/12/14 16:44:04 by nlaerema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**_command_not_found(char **argv)
{
	ft_dprintf(STDERR_FILENO, "%s: command not found\n", argv[0]);
	return (argv);
}

static char	**_get_argv(char *cmd, char **envp)
{
	char	**argv;	
	char	*cmd_path;

	argv = ft_split(cmd, " \t");
	if (argv)
	{
		cmd_path = ft_which(argv[0], envp);
		if (!cmd_path)
			return (_command_not_found(argv));
		free(argv[0]);
		argv[0] = cmd_path;
	}
	return (argv);
}

int	ft_pipex(int in, char **cmd_tab, char **envp, int out)
{
	char	**argv;
	int		cmd_out;
	int		status;

	if (cmd_tab[0])
	{
		argv = _get_argv(cmd_tab[0], envp);
		if (!argv)
			return (EXIT_FAILURE);
		if (cmd_tab[1])
		{
			cmd_out = INVALID_FD;
			ft_execve(&in, argv, envp, &cmd_out);
			ft_pipex(cmd_out, cmd_tab + 1, envp, out);
		}
		else
			ft_execve(&in, argv, envp, &out);
		wait(&status);
		ft_split_free(argv);
		return (WEXITSTATUS(status));
	}
	return (EXIT_SUCCESS);
}
