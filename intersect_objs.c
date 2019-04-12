/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_objs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 10:41:13 by chorange          #+#    #+#             */
/*   Updated: 2019/04/12 12:42:06 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static double	ray_intersect_plane(t_vector start, t_vector dir, t_obj *plane)
{
	double dir_dot_c;
	double cen_dot_c;
	double t;

	start = vector_subt(start, plane->center);
	dir_dot_c = scal_mult(dir, plane->dir);
	cen_dot_c = scal_mult(start, plane->dir);
	if (dir_dot_c == 0.0 || (dir_dot_c < ZERO && cen_dot_c < ZERO) ||
			(dir_dot_c > ZERO && cen_dot_c > ZERO))
		return (0);
	t = -cen_dot_c / dir_dot_c;
	return (t >= ZERO ? t : 0);
}

static double	ray_intersect_cylinder(t_vector start, t_vector dir, t_obj *cyl)
{
	t_s s;

	start = vector_subt(start, cyl->center);
	s.a = scal_mult(dir, dir) - scal_mult(dir, cyl->dir) *
			scal_mult(dir, cyl->dir);
	s.b = 2 * (scal_mult(dir, start) - scal_mult(dir, cyl->dir) *
			scal_mult(start, cyl->dir));
	s.c = scal_mult(start, start) - scal_mult(start, cyl->dir) *
			scal_mult(start, cyl->dir) - cyl->radius * cyl->radius;
	s.diskr = s.b * s.b - 4 * s.a * s.c;
	if (s.diskr < ZERO)
		return (0.0);
	s.sq_diskr = sqrt(s.diskr);
	s.t1 = (-s.b + s.sq_diskr) / (2 * s.a);
	s.t2 = (-s.b - s.sq_diskr) / (2 * s.a);
	if (s.t1 <= ZERO)
		return (0.0);
	s.t = (s.t2 > ZERO) ? s.t2 : s.t1;
	return (s.t);
}

static double	ray_intersect_cone(t_vector start, t_vector dir, t_obj *cone)
{
	t_s s;

	start = vector_subt(start, cone->center);
	s.k = tan(cone->angle);
	s.a = scal_mult(dir, dir) - (1 + s.k * s.k) *
		((scal_mult(dir, cone->dir) * (scal_mult(dir, cone->dir))));
	s.b = 2 * (scal_mult(dir, start) - (1 + s.k * s.k) *
		scal_mult(dir, cone->dir) * scal_mult(start, cone->dir));
	s.c = scal_mult(start, start) - (1 + s.k * s.k) *
		((scal_mult(start, cone->dir) * (scal_mult(start, cone->dir))));
	s.diskr = s.b * s.b - 4 * s.a * s.c;
	if (s.diskr < ZERO)
		return (0.0);
	s.sq_diskr = sqrt(s.diskr);
	s.t1 = (-s.b + s.sq_diskr) / (2 * s.a);
	s.t2 = (-s.b - s.sq_diskr) / (2 * s.a);
	if ((s.t1 <= s.t2 && s.t1 >= ZERO) || (s.t1 >= ZERO && s.t2 < ZERO))
		return (s.t1);
	if ((s.t2 <= s.t1 && s.t2 >= ZERO) || (s.t2 >= ZERO && s.t1 < ZERO))
		return (s.t2);
	return (0.0);
}

static double	ray_intersect_sphere(t_vector start, t_vector dir, t_obj *obj)
{
	t_s s;

	start = vector_subt(start, obj->center);
	s.a = scal_mult(dir, dir);
	s.b = scal_mult(start, dir);
	s.c = scal_mult(start, start) - obj->radius * obj->radius;
	s.diskr = s.b * s.b - s.a * s.c;
	if (s.diskr < ZERO)
		return (0.0);
	s.sq_diskr = sqrt(s.diskr);
	s.t1 = (-s.b + s.sq_diskr) / (s.a);
	s.t2 = (-s.b - s.sq_diskr) / (s.a);
	if (s.t1 <= ZERO)
		return (0.0);
	s.t = (s.t2 > ZERO) ? s.t2 : s.t1;
	return (s.t);
}

double			ray_intersect_obj(t_vector start, t_vector dir, t_obj *obj)
{
	if (obj->type == sphere)
		return (ray_intersect_sphere(start, dir, obj));
	else if (obj->type == cone)
		return (ray_intersect_cone(start, dir, obj));
	else if (obj->type == cylinder)
		return (ray_intersect_cylinder(start, dir, obj));
	else if (obj->type == plane)
		return (ray_intersect_plane(start, dir, obj));
	return (0.0);
}
