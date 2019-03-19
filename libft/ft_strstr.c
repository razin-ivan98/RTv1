/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 20:07:25 by chorange          #+#    #+#             */
/*   Updated: 2018/12/05 19:10:03 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *s1, const char *s2)
{
	char	*it1;
	char	*it2;

	if (!*s2)
		return ((char *)s1);
	while (*s1)
	{
		if (*s1 == *s2)
		{
			it1 = (char *)s1 + 1;
			it2 = (char *)s2 + 1;
			while (*it1 && *it2 && *it1 == *it2)
			{
				it1++;
				it2++;
			}
			if (!*it2)
				return ((char *)s1);
		}
		s1++;
	}
	return (NULL);
}
