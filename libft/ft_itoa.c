/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 20:00:42 by chorange          #+#    #+#             */
/*   Updated: 2018/11/28 20:00:43 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int		count_of_chars(int n)
{
	int		count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		count++;
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char			*ft_itoa(int n)
{
	char	*res;
	int		c;
	short	neg;

	if (n < 0)
		neg = -1;
	else
		neg = 1;
	c = count_of_chars(n);
	if (!(res = (char *)malloc((c + 1) * sizeof(char))))
		return (NULL);
	res += (c);
	*res = '\0';
	if (n == 0)
		*--res = 48;
	while (n != 0)
	{
		*--res = 48 + neg * (n % 10);
		n /= 10;
	}
	if (neg == -1)
		*--res = '-';
	return (res);
}
