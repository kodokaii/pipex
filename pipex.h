/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaerema <nlaerema@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 10:58:17 by nlaerema          #+#    #+#             */
/*   Updated: 2023/11/07 01:24:29 by nlaerema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h>

typedef enum e_pipe_error
{
	NO_ERROR		= 0b00000000,
	ALLOC_ERROR		= 0b00000001,
	PIPE_ERROR		= 0b00000010,
	CMD_INVALID		= 0b00000100,
	DUP_ERROR		= 0b00001000,
	FORK_ERROR		= 0b00010000,
	CLOSE_ERROR		= 0b00100000,
	CMD_ERROR		= 0b01000000,
	IN_FORK			= 0b10000000
}	t_pipe_error;

typedef struct s_pipe
{
	int	in[2];
	int	out[2];
}	t_pipe;

typedef struct s_exec
{
	char	*cmd_path;
	char	**argv;
	char	**envp;
}	t_exec;

pid_t	ft_execve(int *in, char const *cmd, char *const *envp, int *out);

int		ft_pipex(int in, char *const *cmd, char *const *envp, int out);

char	*ft_which(char const *cmd, char *const *envp);

#endif
