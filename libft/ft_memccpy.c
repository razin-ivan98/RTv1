/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 20:01:35 by chorange          #+#    #+#             */
/*   Updated: 2018/11/28 20:01:37 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char		*p_dst;
	unsigned char		*p_src;
	unsigned char		u_c;

	p_dst = (unsigned char *)dst;
	p_src = (unsigned char *)src;
	u_c = c;
	if (n)
	{
		while (n-- != 0)
		{
			if ((*p_dst++ = *p_src++) == u_c)
				return (p_dst);
		}
	}
	return (0);
}
