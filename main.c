/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaerema <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 23:14:01 by nlaerema          #+#    #+#             */
/*   Updated: 2023/11/06 23:32:56 by nlaerema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char *argv[], char *const envp[])
{
	if (1 < argc)
	{
		ft_pipex(STDIN_FILENO, argv + 1, envp, STDOUT_FILENO);
	}
	return (1);
}
