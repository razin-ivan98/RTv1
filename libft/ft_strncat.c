/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 20:05:21 by chorange          #+#    #+#             */
/*   Updated: 2018/11/28 20:05:22 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *s1, const char *s2, size_t n)
{
	char	*ptr;

	ptr = s1;
	while (*ptr != '\0')
	{
		ptr++;
	}
	while (*s2 != '\0' && n > 0)
	{
		*ptr = *s2;
		ptr++;
		s2++;
		n--;
	}
	*ptr = '\0';
	return (s1);
}
