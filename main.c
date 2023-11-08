/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaerema <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 23:14:01 by nlaerema          #+#    #+#             */
/*   Updated: 2023/11/09 00:43:33 by nlaerema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"

static int	close_fd(int in, int out, int returnCode)
{
	ft_close(&in);
	ft_close(&out);
	return (returnCode);
}

static int	argument_error(t_bool heredoc)
{
	if (heredoc)
		ft_dprintf(STDERR_FILENO,
			"Usage: %s here_doc LIMITER cmd1 ... cmdn file_out\n",
			ft_basename(ft_argv(NULL)[0]));
	else
		ft_dprintf(STDERR_FILENO,
			"Usage: %s file_in cmd1 ... cmdn file_out\n",
			ft_basename(ft_argv(NULL)[0]));
	return (EXIT_FAILURE);
}

static int	open_error(char const *file)
{
	ft_dprintf(STDERR_FILENO, "%s: %s: %s\n",
		ft_basename(ft_argv(NULL)[0]), strerror(errno), file);
	return (EXIT_FAILURE);
}

static int	open_file(int argc, char **argv, int *in, int *out)
{
	if (strcmp(argv[1], "here_doc"))
	{
		*in = open(argv[1], O_RDONLY);
		if (*in == INVALID_FD)
			return (open_error(argv[1]));
		*out = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0b110100100);
		if (*out == INVALID_FD)
			return (open_error(argv[argc - 1]));
	}
	else if (4 < argc)
	{
		*in = ft_get_heredoc(argv[2]);
		if (*in == INVALID_FD)
			return (EXIT_FAILURE);
		*out = open(argv[argc - 1], O_WRONLY | O_APPEND | O_CREAT, 0b110100100);
		if (*out == INVALID_FD)
			return (open_error(argv[argc - 1]));
	}
	else
		return (argument_error(FT_TRUE));
	return (EXIT_SUCCESS);
}

int	main(int argc, char *argv[], char *const envp[])
{
	int	infile;
	int	outfile;

	ft_argv(argv);
	if (3 < argc)
	{
		infile = INVALID_FD;
		outfile = INVALID_FD;
		if (open_file(argc, argv, &infile, &outfile))
			return (close_fd(infile, outfile, EXIT_FAILURE));
		argv[argc - 1] = NULL;
		if (!strcmp(argv[1], "here_doc"))
			argv++;
		argv += 2;
		if (ft_pipex(infile, argv, envp, outfile))
			return (close_fd(infile, outfile, EXIT_FAILURE));
		return (close_fd(infile, outfile, EXIT_SUCCESS));
	}
	return (argument_error(FT_FALSE));
}
