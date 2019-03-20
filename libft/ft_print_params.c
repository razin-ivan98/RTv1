/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_params.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 20:48:26 by chorange          #+#    #+#             */
/*   Updated: 2018/11/28 21:03:27 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_print_params(int argc, char **argv)
{
	int i;

	i = 1;
	while (i < argc)
	{
		while (*argv[i] != '\0')
		{
			ft_putchar(*argv[i]);
			argv[i]++;
		}
		ft_putchar('\n');
		i++;
	}
	return (0);
}
