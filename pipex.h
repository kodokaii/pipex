/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaerema <nlaerema@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 10:58:17 by nlaerema          #+#    #+#             */
/*   Updated: 2023/11/08 03:30:11 by nlaerema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h>

typedef enum e_execve_error
{
	EXCV_NO_ERROR,
	EXCV_CMD_ERROR,
	EXCV_FORK_ERROR,
	EXCV_OTHER_ERROR
}	t_execve_error;

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

pid_t		ft_execve(int *in, char const *cmd, char *const *envp, int *out);

int			ft_pipex(int in, char *const *cmd, char *const *envp, int out);

int			ft_get_heredoc(char const *limiter);

char		*ft_which(char const *cmd, char *const *envp);

#endif
