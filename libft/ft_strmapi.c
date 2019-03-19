/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 20:05:11 by chorange          #+#    #+#             */
/*   Updated: 2018/11/28 20:05:12 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	char			*result;
	unsigned int	i;

	i = 0;
	if (s == NULL || f == NULL)
		return (NULL);
	if (!(str = (char*)malloc(ft_strlen(s) + 1)))
		return (NULL);
	result = str;
	while (*s != 0)
	{
		*str = f(i, *s);
		str++;
		s++;
		i++;
	}
	*str = '\0';
	return (result);
}
