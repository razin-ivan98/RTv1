/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect_math2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 12:00:58 by chorange          #+#    #+#             */
/*   Updated: 2019/04/12 12:03:24 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double		vector_length(t_vector a)
{
	double result;

	result = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	return (result);
}

t_vector	vector_init(double x, double y, double z)
{
	t_vector ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

t_vector	vector_project(t_vector a, t_vector b)
{
	t_vector	project;
	double		dot;

	dot = scal_mult(a, b) / scal_mult(b, b);
	project = vector_int_mult(b, dot);
	return (project);
}

t_vector	vector_normalize(t_vector a)
{
	t_vector c;

	c = vector_int_div(a, sqrt(scal_mult(a, a)));
	return (c);
}
