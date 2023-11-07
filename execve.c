/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaerema <nlaerema@student.42lehavre.fr>	+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 10:58:17 by nlaerema          #+#    #+#             */
/*   Updated: 2023/11/07 02:19:58 by nlaerema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	g_fork = 0;

static void	execve_free(t_exec *exec)
{
	size_t	i;

	i = 0;
	while (exec->argv && exec->argv[i])
		free(exec->argv[i++]);
	if (exec->argv)
	{
		free(exec->argv);
		exec->argv = NULL;
	}
	if (exec->cmd_path)
	{
		free(exec->cmd_path);
		exec->cmd_path = NULL;
	}
}

pid_t	execve_error(t_pipe *pipefd, t_exec *exec,
			char const *cmd, t_pipe_error error)
{
	close(pipefd->in[0]);
	close(pipefd->in[1]);
	close(pipefd->out[0]);
	close(pipefd->out[1]);
	execve_free(exec);
	if (error & ALLOC_ERROR)
		ft_dprintf(STDERR_FILENO, "%d: alloc error\n", g_fork);
	if (error & PIPE_ERROR)
		ft_dprintf(STDERR_FILENO, "%d: pipe error\n", g_fork);
	if (error & CMD_INVALID)
		ft_dprintf(STDERR_FILENO, "command not found: %s\n", cmd);
	if (error & DUP_ERROR)
		ft_dprintf(STDERR_FILENO, "%d: dup error\n", g_fork);
	if (error & FORK_ERROR)
		ft_dprintf(STDERR_FILENO, "%d: fork error\n", g_fork);
	if (error & CLOSE_ERROR)
		ft_dprintf(STDERR_FILENO, "%d: close error\n", g_fork);
	if (error & CMD_ERROR)
		ft_dprintf(STDERR_FILENO, "command not work: %s\n", exec->cmd_path);
	if (error & IN_FORK)
		exit(1);
	return (INVALID_PID);
}

static int	init_pipe(int *in, t_pipe *pipefd, int *out)
{
	if (*in != INVALID_FD)
	{
		pipefd->in[1] = INVALID_FD;
		pipefd->in[0] = *in;
	}
	else
	{
		if (pipe(pipefd->in))
			return (1);
		*in = pipefd->in[1];
	}
	if (*out != INVALID_FD)
	{
		pipefd->out[1] = *out;
		pipefd->out[0] = INVALID_FD;
	}
	else
	{
		if (pipe(pipefd->out))
			return (1);
		*out = pipefd->out[0];
	}
	return (0);
}

static void	cmd_execve(t_pipe *pipefd, t_exec *exec, char const *cmd)
{
	g_fork++;
	if (ft_close(&pipefd->in[1]) || ft_close(&pipefd->out[0]))
		execve_error(pipefd, exec, cmd, CLOSE_ERROR | IN_FORK);
	if (dup2(pipefd->in[0], STDIN_FILENO) == INVALID_FD)
		execve_error(pipefd, exec, cmd, DUP_ERROR | IN_FORK);
	if (dup2(pipefd->out[1], STDOUT_FILENO) == INVALID_FD)
		execve_error(pipefd, exec, cmd, DUP_ERROR | IN_FORK);
	execve(exec->cmd_path, exec->argv, exec->envp);
	execve_error(pipefd, exec, cmd, CMD_ERROR | IN_FORK);
}

pid_t	ft_execve(int *in, char const *cmd, char *const *envp, int *out)
{
	t_pipe	pipefd;
	t_exec	exec;
	pid_t	pid;

	exec.cmd_path = NULL;
	exec.envp = (char **)envp;
	exec.argv = ft_split(cmd, ' ');
	if (!exec.argv)
		return (execve_error(&pipefd, &exec, cmd, ALLOC_ERROR));
	exec.cmd_path = ft_which(exec.argv[0], envp);
	if (!exec.cmd_path)
		return (execve_error(&pipefd, &exec, cmd, CMD_INVALID));
	if (init_pipe(in, &pipefd, out))
		return (execve_error(&pipefd, &exec, cmd, PIPE_ERROR));
	pid = fork();
	if (pid == 0)
		cmd_execve(&pipefd, &exec, cmd);
	if (pid == INVALID_PID)
		return (execve_error(&pipefd, &exec, cmd, FORK_ERROR));
	execve_free(&exec);
	if (ft_close(&pipefd.in[0]) || ft_close(&pipefd.out[1]))
		return (execve_error(&pipefd, &exec, cmd, CLOSE_ERROR));
	return (pid);
}