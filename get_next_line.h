/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 06:28:29 by chorange          #+#    #+#             */
/*   Updated: 2019/02/18 19:31:18 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>

# define BUFF_SIZE 1024

# define MALLCHECK(x) if (!(x)) return (-1);
# define ERRCHECK(x, y, z) if ((x) < 0 || (y) == NULL || (z) < 0) return (-1);

typedef struct		s_file
{
	char			reserv[BUFF_SIZE + 1];
	int				fd;
	struct s_file	*next;
}					t_file;

int					get_next_line(int fd, char **line);

#endif
