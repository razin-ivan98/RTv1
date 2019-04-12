/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 06:30:59 by chorange          #+#    #+#             */
/*   Updated: 2019/02/21 17:44:46 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_file	*check_file_list(int fd, t_file **file)
{
	t_file	*tmp;

	tmp = *file;
	while (tmp)
	{
		if (tmp->fd == fd)
			return (tmp);
		tmp = tmp->next;
	}
	if (!(tmp = (t_file *)malloc(sizeof(t_file))))
		return (NULL);
	tmp->fd = fd;
	tmp->reserv[0] = 0;
	tmp->next = NULL;
	if (*file == NULL)
		*file = tmp;
	else
	{
		tmp->next = *file;
		*file = tmp;
	}
	tmp = *file;
	return (tmp);
}

int		remallocjoin(char **dst, char *src)
{
	char	*tmp;

	MALLCHECK((tmp = (char *)malloc(ft_strlen(*dst) + 1)));
	ft_strcpy(tmp, *dst);
	free(*dst);
	*dst = NULL;
	MALLCHECK((*dst = (char *)malloc(ft_strlen(tmp) + ft_strlen(src) + 1)));
	ft_strcpy(*dst, tmp);
	ft_strcat(*dst, src);
	free(tmp);
	return (0);
}

int		cycle_body(char *buf, t_file *file, int *read_chars, char **line)
{
	if (file->reserv[0])
	{
		ft_strcpy(buf, file->reserv);
		file->reserv[0] = 0;
	}
	else
	{
		*read_chars = read(file->fd, buf, BUFF_SIZE);
		if (*read_chars == 0)
			return ((*line[0]) ? 1 : 0);
		buf[*read_chars] = '\0';
	}
	if (ft_strchr(buf, '\n'))
	{
		ft_strcpy(file->reserv, ft_strchr(buf, '\n') + 1);
		*(ft_strchr(buf, '\n')) = '\0';
		if (remallocjoin(line, buf) == -1)
			return (-1);
		return (1);
	}
	else if (remallocjoin(line, buf) == -1)
		return (-1);
	return (2);
}

int		get_next_line(int fd, char **line)
{
	static t_file	*file;
	t_file			*curr;
	char			buf[BUFF_SIZE + 1];
	int				read_chars;
	int				returned;

	ERRCHECK(fd, line, read(fd, buf, 0));
	MALLCHECK(((*line) = (char *)malloc(1)));
	(*line)[0] = 0;
	MALLCHECK((curr = check_file_list(fd, &file)));
	while (1)
	{
		returned = cycle_body(buf, curr, &read_chars, line);
		if (returned != 2)
			return (returned);
	}
}
