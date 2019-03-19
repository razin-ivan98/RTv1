/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 20:06:59 by chorange          #+#    #+#             */
/*   Updated: 2018/11/28 20:07:00 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_strnlen(const char *str, size_t maxlen)
{
	unsigned len;

	len = 0;
	while (len < maxlen && *str)
	{
		++len;
		++str;
	}
	return (len);
}
