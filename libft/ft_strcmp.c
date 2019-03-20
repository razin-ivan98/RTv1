/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 20:03:48 by chorange          #+#    #+#             */
/*   Updated: 2018/12/03 17:56:26 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strcmp(const char *s1, const char *s2)
{
	while ((*s1 != '\0') && (*s2 != '\0'))
	{
		if (*s1 > *s2)
			return ((unsigned const char)*s1 - (unsigned const char)*s2);
		else if (*s1 < *s2)
			return ((unsigned const char)*s1 - (unsigned const char)*s2);
		s1++;
		s2++;
	}
	return ((unsigned const char)*s1 - (unsigned const char)*s2);
}
