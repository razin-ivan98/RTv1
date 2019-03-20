/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 20:05:03 by chorange          #+#    #+#             */
/*   Updated: 2018/11/28 20:05:03 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmap(char const *s, char (*f)(char))
{
	char	*str;
	char	*result;

	if (s == NULL || f == NULL)
		return (NULL);
	if (!(str = (char*)malloc(ft_strlen(s) + 1)))
		return (NULL);
	result = str;
	while (*s != 0)
	{
		*str = f(*s);
		str++;
		s++;
	}
	*str = '\0';
	return (result);
}
