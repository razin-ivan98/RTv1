/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 20:00:57 by chorange          #+#    #+#             */
/*   Updated: 2018/11/28 20:00:59 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ft_lstdel(t_list **alst, void (*del) (void*, size_t))
{
	if (*alst == NULL)
		return ;
	del((*alst)->content, (*alst)->content_size);
	ft_lstdel(&((*alst)->next), del);
	free(*alst);
	*alst = NULL;
}
