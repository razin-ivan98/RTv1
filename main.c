/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 19:25:19 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/21 21:00:16 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RTv1.h"

t_vector get_pixel_pisition(int x, int y)
{
	t_vector pixel_pos;
	double x3;
	double y3;

	x3 = (double)VW/(double)CW*(double)x;
	y3 = (double)VW/(double)CW*(double)y;

	pixel_pos = vector_init(x3, y3, 1.0);
	return (pixel_pos);
}

t_vector reflect_ray(t_vector R, t_vector N)
{
	t_vector ret;

	ret = vector_subt(vector_int_mult(N, 2 * scal_mult(N, R)), R);
	return (ret);
}

double ray_intersect_plane(t_vector start, t_vector dir, t_obj *plane)
{
	double zeroThreshold = 0.0001;
	double dir_dot_c;
	double cen_dot_c;
	double t;

	start = vector_subt(start, plane->center); // перенесем центр координат в центр сферы
	dir_dot_c = scal_mult(dir, plane->dir);
	cen_dot_c = scal_mult(start, plane->dir);

	if (dir_dot_c != 0.0)
	{
		t = -cen_dot_c / dir_dot_c;
		if (t < 100000.0)
			return(t);
	}
	return(0.0);

}


double ray_intersect_cylinder(t_vector start, t_vector dir, t_obj *cyl)
{
	double zeroThreshold = 0.0001;

	start = vector_subt(start, cyl->center); // перенесем центр координат в центр сферы

	double a = scal_mult(dir, dir) - scal_mult(dir, cyl->dir) * scal_mult(dir, cyl->dir); // Здесь и далее операция * над векторами - скалярное произведение
	double b = 2 * (scal_mult(dir, start) - scal_mult(dir, cyl->dir) * scal_mult(start, cyl->dir));
	double c = scal_mult(start, start) - scal_mult(start, cyl->dir) * scal_mult(start, cyl->dir) - cyl->radius * cyl->radius;
	double D = b*b - 4*a*c; // Дискриминант

	if ( D < zeroThreshold )
		return (0.0); // Если меньше некоторого эпсилон (никогда не сравнивайте даблы на чистый ноль!), то не пересекается
	double qD = sqrt(D); // Иначе вычислим точки пересечения по всем известной формуле корней
	double t1 = ( -b + qD)/(2*a); // Больший корень
	double t2 = ( -b - qD)/(2*a); // Меньший корень
	if (t1 <= zeroThreshold) // Если больший корень отрицательный, то мы пересекаем сферу в противоположном направлении луча
		return (0.0); // То есть не пересекаем
	double t = (t2 > zeroThreshold) ? t2 : t1; // Если да, то точка пересечения это больший корень, если меньший за точкой старта (мы "внутри сферы"), иначе меньший корень. Точка пересечения пригодится дальше.
		return (t);
}

double ray_intersect_cone(t_vector start, t_vector dir, t_obj *cone)
{
	double zeroThreshold = 0.0001;

	start = vector_subt(start, cone->center); // перенесем центр координат в центр сферы
	double k = tan(cone->angle);

	double a = scal_mult(dir, dir) - (1 + k * k) * ((scal_mult(dir, cone->dir) * (scal_mult(dir, cone->dir)))); // Здесь и далее операция * над векторами - скалярное произведение
	double b = 2 * (scal_mult(dir, start) - (1 + k * k) * scal_mult(dir, cone->dir) * scal_mult(start, cone->dir));
	double c = scal_mult(start, start) - (1 + k * k) * ((scal_mult(start, cone->dir)* (scal_mult(start, cone->dir))));
	double D = b*b - 4*a*c; // Дискриминант

	if ( D < 0.0 )
		return (0.0); // Если меньше некоторого эпсилон (никогда не сравнивайте даблы на чистый ноль!), то не пересекается
	double qD = sqrt(D); // Иначе вычислим точки пересечения по всем известной формуле корней
	double t1 = ( -b + qD)/(2*a); // Больший корень
	double t2 = ( -b - qD)/(2*a); // Меньший корень
	if (t1 <= zeroThreshold) // Если больший корень отрицательный, то мы пересекаем сферу в противоположном направлении луча
		return (0.0); // То есть не пересекаем
	double t = (t2 > zeroThreshold) ? t2 : t1; // Если да, то точка пересечения это больший корень, если меньший за точкой старта (мы "внутри сферы"), иначе меньший корень. Точка пересечения пригодится дальше.
		return (t);
}


double ray_intersect_sphere(t_vector start, t_vector dir, t_obj *obj)
{
	double zeroThreshold = 0.0001;

	start = vector_subt(start, obj->center); // перенесем центр координат в центр сферы
	double a = scal_mult(dir, dir); // Здесь и далее операция * над векторами - скалярное произведение
	double b = scal_mult(start, dir);
	double c = scal_mult(start, start) - obj->radius * obj->radius;
	double D = b*b -a*c; // Дискриминант

	if ( D < zeroThreshold )
		return (0.0); // Если меньше некоторого эпсилон (никогда не сравнивайте даблы на чистый ноль!), то не пересекается
	double qD = sqrt(D); // Иначе вычислим точки пересечения по всем известной формуле корней
	double t1 = ( -b + qD)/(a); // Больший корень
	double t2 = ( -b - qD)/(a); // Меньший корень
	if (t1 <= zeroThreshold) // Если больший корень отрицательный, то мы пересекаем сферу в противоположном направлении луча
		return (0.0); // То есть не пересекаем
	double t = (t2 > zeroThreshold) ? t2 : t1; // Если да, то точка пересечения это больший корень, если меньший за точкой старта (мы "внутри сферы"), иначе меньший корень. Точка пересечения пригодится дальше.
		return (t);
}

double ray_intersect_obj(t_vector start, t_vector dir, t_obj *obj)
{
	if (obj->type == sphere)
		return (ray_intersect_sphere(start, dir, obj));
	else if (obj->type == cone)
		return (ray_intersect_cone(start, dir, obj));
	else if (obj->type == cylinder)
		return (ray_intersect_cylinder(start, dir, obj));
	else if (obj->type == plane)
		return (ray_intersect_plane(start, dir, obj));
}

t_obj *get_closest_object(double *closest_t, t_vector start, t_vector dir, t_scene *scene)
{
	double t = 0.0;
	int i = 0;
	t_obj *closest_obj = NULL;



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








void scene_init(t_RTv1 *RTv1, char *file_name)
{
    read_scene(&(RTv1->scene), file_name);
}

void provider(t_RTv1 *RTv1)
{
    size_t global_work_size = CW * CH;

    RTv1->ret = clEnqueueWriteBuffer(RTv1->command_queue,
			RTv1->utils_memobj, CL_TRUE, 0,
			sizeof(t_scene), &(RTv1->scene), 0, NULL, NULL);

	RTv1->ret = clSetKernelArg(RTv1->kernel, 1,
			sizeof(cl_mem), (void *)&RTv1->utils_memobj);


    RTv1->ret = clEnqueueNDRangeKernel(RTv1->command_queue, RTv1->kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
    RTv1->ret = clEnqueueReadBuffer(RTv1->command_queue, RTv1->memobj, CL_TRUE, 0, CW * CH * 4, RTv1->image_data, 0, NULL, NULL);

    mlx_put_image_to_window(RTv1->mlx_ptr, RTv1->win_ptr, RTv1->image, 0, 0);
}

int mouse_pressed(int button, int x, int y, void *param)
{
	t_RTv1 *RTv1;
	t_vector pixel_pos_3d;
	double trash = 99999.0;
	t_obj *ptr = NULL;
	RTv1 = (t_RTv1 *)param;

	if (button == 1)
	{
		pixel_pos_3d = get_pixel_pisition(x - CW / 2, -y + CH / 2);

		ptr = get_closest_object(&trash, RTv1->scene.camera.center, pixel_pos_3d, &(RTv1->scene));
		if (ptr)
			RTv1->selected = ptr;
	}

	else if (button == 4)
	{
		RTv1->scene.camera.center.z -= 0.2;
	}

	else if (button == 5)
		RTv1->scene.camera.center.z += 0.2;

	provider(RTv1);
	return (0);
}


int key_pressed(int key, void *param)
{
	t_RTv1 *RTv1;

	RTv1 = (t_RTv1 *)param;

	if (!RTv1->selected)
		RTv1->selected = &(RTv1->scene.objs[0]);

	if (key == 0x35)
	{
		exit(1);
	}

	else if (key == 0x7C)
	{
		RTv1->selected->center.x += 0.05;
	}
	else if (key ==0x7B)
	{
		RTv1->selected->center.x -= 0.05;
	}
	else if (key ==0x7E)
	{
		RTv1->selected->center.y += 0.05;
	}
	else if (key ==0x7D)
	{
		RTv1->selected->center.y -= 0.05;
	}
	else if (key ==0x5B)
	{
		RTv1->selected->center.z += 0.05;
	}
	else if (key ==0x54)
	{
		RTv1->selected->center.z -= 0.05;
	}
	provider(RTv1);
}


int main(int ac, char **av)
{
    t_RTv1 RTv1;

    if (ac != 2)
        exit(-1);

    graphics_init(&RTv1);
    scene_init(&RTv1, av[1]);
    provider(&RTv1);






	mlx_hook(RTv1.win_ptr, 2, 4, key_pressed, &RTv1);
	mlx_hook(RTv1.win_ptr, 4, 0, mouse_pressed, &RTv1);

    mlx_loop(RTv1.mlx_ptr);
    return (0);
}

