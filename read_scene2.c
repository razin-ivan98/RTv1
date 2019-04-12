/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_scene2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 12:58:18 by chorange          #+#    #+#             */
/*   Updated: 2019/04/12 13:48:20 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		set_obj_type(char *line, t_obj *obj)
{
	if (ft_strstr(line, "sphere"))
		obj->type = sphere;
	else if (ft_strstr(line, "cylinder"))
		obj->type = cylinder;
	else if (ft_strstr(line, "cone"))
		obj->type = cone;
	else if (ft_strstr(line, "plane"))
		obj->type = plane;
	else
		err_exit();
}

void		set_light_type(char *line, t_light *light)
{
	if (ft_strstr(line, "point"))
		light->type = point;
	else if (ft_strstr(line, "directional"))
		light->type = directional;
	else if (ft_strstr(line, "ambient"))
		light->type = ambient;
	else
		err_exit();
}

t_obj		*read_obj_parameters(char *line, t_obj **obj)
{
	if (ft_strchr(line, '}'))
		return (NULL);
	else if (ft_strstr(line, "type"))
		set_obj_type(line, *obj);
	else if (ft_strstr(line, "position"))
		(*obj)->center = read_vector(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "radius"))
		(*obj)->radius = ft_atof(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "color"))
		(*obj)->rgb = color_to_rgb(ft_atoi(ft_strchr(line, '=') + 1));
	else if (ft_strstr(line, "angle"))
		(*obj)->angle = ft_atof(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "direction"))
		(*obj)->dir = vector_normalize(read_vector(ft_strchr(line, '=') + 1));
	else if (ft_strstr(line, "specular"))
		(*obj)->specular = ft_atof(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "reflective"))
		(*obj)->reflective = ft_atof(ft_strchr(line, '=') + 1);
	return (*obj);
}

t_light		*read_light_parameters(char *line, t_light **light)
{
	if (ft_strchr(line, '}'))
		return (NULL);
	else if (ft_strstr(line, "type"))
		set_light_type(line, *light);
	else if (ft_strstr(line, "position"))
		(*light)->center = read_vector(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "direction"))
		(*light)->dir = vector_normalize(read_vector(ft_strchr(line, '=') + 1));
	else if (ft_strstr(line, "intensity"))
		(*light)->intensity = ft_atof(ft_strchr(line, '=') + 1);
	return (*light);
}

t_camera	*camera_init(char *line, t_camera *camera)
{
	if (ft_strchr(line, '}'))
		return (NULL);
	else if (ft_strstr(line, "position"))
		camera->center = read_vector(ft_strchr(line, '=') + 1);
	else if (ft_strstr(line, "direction"))
		camera->dir = read_vector(ft_strchr(line, '=') + 1);
	return (camera);
}
