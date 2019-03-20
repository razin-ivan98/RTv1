/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 20:01:28 by chorange          #+#    #+#             */
/*   Updated: 2018/11/28 20:01:29 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void *pointer;

	if ((pointer = malloc(size)))
	{
		ft_bzero(pointer, size);
		return (pointer);
	}
	else
		return (NULL);
}
