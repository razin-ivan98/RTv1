/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 20:04:43 by chorange          #+#    #+#             */
/*   Updated: 2018/11/28 20:04:44 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int		row_saver(char *dest, const char *src, int len, int position)
{
	int i;

	i = 0;
	while (i < len)
	{
		*(dest + position + i) = *src;
		i++;
		src++;
	}
	return (i + position);
}

char			*ft_strjoin(char const *s1, char const *s2)
{
	char	*fresh;
	int		len;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	if (!(fresh = (char*)malloc(ft_strlen(s2) + ft_strlen(s1))))
		return (NULL);
	len = row_saver(fresh, s2, ft_strlen(s2),
	row_saver(fresh, s1, ft_strlen(s1), 0));
	*(fresh + len) = 0;
	return (fresh);
}
