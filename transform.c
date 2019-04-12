/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 12:23:52 by chorange          #+#    #+#             */
/*   Updated: 2019/04/12 13:31:41 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vector		rotate_view(t_vector point, double alpha, double beta)
{
	t_vector tempo;

	tempo = point;
	point.y = tempo.y * cos(alpha) + tempo.z * sin(alpha);
	point.z = -tempo.y * sin(alpha) + tempo.z * cos(alpha);
	tempo = point;
	point.x = tempo.x * cos(beta) - tempo.z * sin(beta);
	point.z = tempo.x * sin(beta) + tempo.z * cos(beta);
	return (point);
}

static t_vector	get_pixel_pisition(int x, int y)
{
	t_vector	pixel_pos;
	double		x3;
	double		y3;

	x3 = (double)VW / (double)CW * (double)x;
	y3 = (double)VW / (double)CW * (double)y;
	pixel_pos = vector_init(x3, y3, 1.0);
	return (pixel_pos);
}

static t_obj	*get_closest_object(double *closest_t, t_vector start,
t_vector dir, t_scene *scene)
{
	double	t;
	int		i;
	t_obj	*closest_obj;

	t = 0.0;
	i = 0;
	closest_obj = NULL;
	while (i < scene->c_objs)
	{
		t = ray_intersect_obj(start, dir, &(scene->objs[i]));
		if (t != 0.0 && t < (*closest_t))
		{
			(*closest_t) = t;
			closest_obj = &(scene->objs[i]);
		}
		i++;
	}
	return (closest_obj);
}

void			select_object(t_rtv1 *rtv1, int x, int y)
{
	t_vector		pixel_pos_3d;
	static t_obj	*ptr;

	ptr = NULL;
	rtv1->left_mouse_pressed = 1;
	pixel_pos_3d = get_pixel_pisition(x - CW / 2, -y + CH / 2);
	pixel_pos_3d = rotate_view(pixel_pos_3d, rtv1->scene.view_alpha,
			rtv1->scene.view_beta);
	ptr = get_closest_object(&(rtv1->selected_t), rtv1->scene.camera.center,
			pixel_pos_3d, &(rtv1->scene));
	if (ptr)
		rtv1->selected = ptr;
	else
		rtv1->left_mouse_pressed = 0;
}
