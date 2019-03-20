#include "RTv1.h"

t_vector vector_subt(t_vector a, t_vector b)
{
	t_vector c;

	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	return (c);
}

t_vector vector_sum(t_vector a, t_vector b)
{
	t_vector c;

	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return (c);
}

t_vector vector_int_div(t_vector a, double b)
{
	t_vector c;

	c.x = a.x / b;
	c.y = a.y / b;
	c.z = a.z / b;
	return (c);
}

t_vector vector_int_mult(t_vector a, double b)
{
	t_vector c;

	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
	return (c);
}

double scal_mult(t_vector a, t_vector b)
{
	double c;
	c = a.x * b.x + a.y * b.y + a.z * b.z;
	return (c);
}

void obj_init(t_obj *obj)
{
	obj->center.x = 0.0;
	obj->center.y = 0.0;
	obj->center.z = 3.0;
	obj->radius = 1.0;
	//obj->color = 0xFFC0CB;
}

t_vector vector_init(double x, double y, double z)
{
	t_vector ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

t_vector vector_project(t_vector a, t_vector b)
{
	t_vector project;
	double dot;

	dot = scal_mult(a, b) / scal_mult(b, b);
	project = vector_int_mult(b, dot);
	return (project);
}

t_vector vector_normalize(t_vector a)
{
	t_vector c;
	c = vector_int_div(a, sqrt(scal_mult(a, a)));

	return (c);
}