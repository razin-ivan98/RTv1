/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 20:01:03 by chorange          #+#    #+#             */
/*   Updated: 2018/11/28 20:01:04 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ft_lstdelone(t_list **alst, void (*del)(void*, size_t))
{
	t_list	*p;

	p = *alst;
	if (p == NULL || alst == NULL)
		return ;
	p->next = NULL;
	if (del != NULL)
		del(p->content, p->content_size);
	free(p);
	*alst = NULL;
}
