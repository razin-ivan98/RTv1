#include "RTv1.h"

#include <stdio.h>
# define SIZE 500
# define VW 1
# define VH 1
# define CW 500
# define CH 500
# define DEPTH 3

void	put_vector_to_image(char *image_data, int x, int y, int color)
{
	int	index = 0;

	if (x < 0 || y < 0 || x > SIZE - 1 || y > SIZE - 1)
		return ;
	else
	{
		index = SIZE * y * 4 + x * 4;
        image_data[index] = color & 0x0000ff;
		image_data[index + 2] = color >> 16;
		image_data[index + 1] = (color & 0x00ff00) >> 8;
	}
}

void	clear_image_data(char *image_data)
{
	int index;

	index = 0;
	while (index < SIZE * SIZE * 4)
		image_data[index++] = 0;
}

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

double ray_intersect_plane(t_vector start, t_vector dir, t_obj plane)
{


	double zeroThreshold = 0.0001;
	double dir_dot_c;
	double cen_dot_c;
	double t;

	start = vector_subt(start, plane.center); // перенесем центр координат в центр сферы
	dir_dot_c = scal_mult(dir, plane.dir);
	cen_dot_c = scal_mult(start, plane.dir);

	if (dir_dot_c != 0.0)
	{
		t = -cen_dot_c / dir_dot_c;
		if (t < 100000.0)
			return(t);
	}
	return(0.0);

}


double ray_intersect_cylinder(t_vector start, t_vector dir, t_obj cyl)
{
	t_ray ray;

	double zeroThreshold = 0.0001;


	ray.start = start;
	ray.dir = dir;

	ray.start = vector_subt(ray.start, cyl.center); // перенесем центр координат в центр сферы
	
	double a = scal_mult(ray.dir, ray.dir) - scal_mult(ray.dir, cyl.dir) * scal_mult(ray.dir, cyl.dir); // Здесь и далее операция * над векторами - скалярное произведение
	double b = 2 * (scal_mult(ray.dir, ray.start) - scal_mult(ray.dir, cyl.dir) * scal_mult(ray.start, cyl.dir));
	double c = scal_mult(ray.start, ray.start) - scal_mult(ray.start, cyl.dir) * scal_mult(ray.start, cyl.dir) - cyl.radius * cyl.radius; 
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

double ray_intersect_cone(t_vector start, t_vector dir, t_obj cone)
{
	t_ray ray;

	double zeroThreshold = 0.0001;


	ray.start = start;
	ray.dir = dir;

	ray.start = vector_subt(ray.start, cone.center); // перенесем центр координат в центр сферы
	double k = tan(cone.angle);
	
	double a = scal_mult(ray.dir, ray.dir) - (1 + k * k) * ((scal_mult(ray.dir, cone.dir) * (scal_mult(ray.dir, cone.dir)))); // Здесь и далее операция * над векторами - скалярное произведение
	double b = 2 * (scal_mult(ray.dir, ray.start) - (1 + k * k) * scal_mult(ray.dir, cone.dir) * scal_mult(ray.start, cone.dir));
	double c = scal_mult(ray.start, ray.start) - (1 + k * k) * ((scal_mult(ray.start, cone.dir)* (scal_mult(ray.start, cone.dir)))); 
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


double ray_intersect_sphere(t_vector start, t_vector dir, t_obj obj)
{
	t_ray ray;

	double zeroThreshold = 0.0001;


	ray.start = start;
	ray.dir = dir;

	ray.start = vector_subt(ray.start, obj.center); // перенесем центр координат в центр сферы
	double a = scal_mult(ray.dir, ray.dir); // Здесь и далее операция * над векторами - скалярное произведение
	double b = scal_mult(ray.start, ray.dir);
	double c = scal_mult(ray.start, ray.start) - obj.radius * obj.radius;
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

double ray_intersect_obj(t_vector start, t_vector dir, t_obj obj)
{
	if (obj.type == sphere)
		return (ray_intersect_sphere(start, dir, obj));
	else if (obj.type == cone)
		return (ray_intersect_cone(start, dir, obj));
	else if (obj.type == cylinder)
		return (ray_intersect_cylinder(start, dir, obj));
	else if (obj.type == plane)
		return (ray_intersect_plane(start, dir, obj));

}

double compute_lighting(t_vector P, t_vector N, t_vector V, double s, t_scene scene)
{
	double intensity = 0.0;

	//t_light lights[3];
	t_vector L;
	double t;
	double shadow_t;
	t_obj *shadow_obj = NULL;

/*
	lights[0].type = directional;
	lights[0].intensity = 0.2;
	lights[0].direction = vector_init(2.0, 4.0, 4.0);

	lights[1].type = point;
	lights[1].intensity = 0.6;
	lights[1].position = vector_init(4.0, 5.0, 3.0);

	lights[2].type = ambient;
	lights[2].intensity = 0.2;
*/
	

	for (int i = 0; i < scene.c_lights; i++)
	{
		shadow_t = 9999;
		int j = 0;
		shadow_obj = NULL;
		if (scene.lights[i].type == ambient)
			intensity += scene.lights[i].intensity;
		else 
		{
			if (scene.lights[i].type == point)
				L = vector_subt(scene.lights[i].center, P);
			else 
				L = scene.lights[i].direction;
		

			while (j < scene.c_objs)
			{
				t = ray_intersect_obj(P, L, scene.objs[j]);
				if (t != 0.0 && t < shadow_t)
				{
					shadow_t = t;
					shadow_obj = &(scene.objs[j]);
				}
				
				j++;
			}
			if (shadow_obj)
				continue ;





			double n_dot_l = scal_mult(N, L);
			if (n_dot_l > 0.0)
				intensity += scene.lights[i].intensity * n_dot_l / (sqrt(scal_mult(N, N)) * sqrt(scal_mult(L, L)));
		
			if (s > 0.0)
			{
				t_vector R = reflect_ray(L, N);
				double r_dot_v = scal_mult(R, V);
				if (r_dot_v > 0.0)
					intensity += scene.lights[i].intensity * pow(r_dot_v / (sqrt(scal_mult(R, R)) * sqrt(scal_mult(V, V))), s);
			}
		}
	}
	return (intensity);

}

t_vector get_normal(t_vector point, t_obj obj)
{
	t_vector normal;
	t_vector project;
	double k;

	if (obj.type == sphere)
	{
		normal = vector_subt(point, obj.center);
		normal = vector_normalize(normal);
	}
	else if (obj.type == cylinder)
	{
		normal = vector_subt(point, obj.center);
		project = vector_project(normal, obj.dir);
		normal = vector_normalize(vector_subt(normal, project));
	}
	else if (obj.type == plane)
		normal = obj.dir;
	else if (obj.type == cone)
	{
		normal = vector_subt(point, obj.center);
		project = vector_project(normal, obj.dir);
		k = 1 + tan(obj.angle) * tan(obj.angle);
		project = vector_int_mult(project, k);
		normal = vector_normalize(vector_subt(normal, project));
	}
	return (normal);
}

t_obj *get_closest_object(t_vector start, t_vector dir, t_scene scene)
{
	double t = 0.0;
	int i = 0;
	double closest_t = 99999.0;
	t_obj *closest_obj;

	closest_obj = NULL;

	while (i < scene.c_objs)
	{
		t = ray_intersect_obj(start, dir, scene.objs[i]);
		if (t != 0.0 && t < (closest_t))
		{
			(closest_t) = t;
			closest_obj = &(scene.objs[i]);
		}
		i++;
	}
	return (closest_obj);
}

int cast_ray(t_vector start, t_vector dir, int depth, t_scene scene)
{

	t_obj *closest_obj = NULL;
	double closest_t;
	closest_t = 99999.0;
	double intensity;
	double t = 0.0;
	int i = 0;
	while (i < scene.c_objs)
	{
		t = ray_intersect_obj(start, dir, scene.objs[i]);
		if (t != 0.0 && t < closest_t)
		{
			closest_t = t;
			closest_obj = &(scene.objs[i]);
		}
		i++;
	}
	//closest_obj = get_closest_object(ptr, start, dir, scene);
	if (!closest_obj)
		return(0x000000);

	if (closest_obj)
	{

		t_vector P = vector_sum(start, vector_int_mult(dir, closest_t));


		t_vector N = get_normal(P, *closest_obj);
		
		/*vector_subt(P, closest_obj->center);
		N = vector_int_div(N, sqrt(scal_mult(N, N)));*/

		intensity = compute_lighting(P, N, vector_int_mult(dir, -1.0), closest_obj->specular, scene);


		if ((closest_obj->rgb.r *= intensity) >= 255.0) 
			closest_obj->rgb.r = 255.0;
		if ((closest_obj->rgb.g *= intensity) >= 255.0) 
			closest_obj->rgb.g = 255.0;
		if ((closest_obj->rgb.b *= intensity) >= 255.0) 
			closest_obj->rgb.b = 255.0;

		
		if (depth <= 0 || closest_obj->reflective <= 0)
			return (rgb_to_color(closest_obj->rgb));
		

		t_vector R = reflect_ray(vector_int_mult(dir, -1.0), N);
	
		t_rgb reflected = color_to_rgb(cast_ray(P, R, depth - 1, scene));

		if ((closest_obj->rgb.r = closest_obj->rgb.r * (1 - closest_obj->reflective) + reflected.r * closest_obj->reflective) > 255.0) 
			closest_obj->rgb.r = 255.0;
		if ((closest_obj->rgb.g = closest_obj->rgb.g * (1 - closest_obj->reflective) + reflected.g * closest_obj->reflective) > 255.0) 
			closest_obj->rgb.g = 255.0;
		if ((closest_obj->rgb.b = closest_obj->rgb.b * (1 - closest_obj->reflective) + reflected.b * closest_obj->reflective) > 255.0) 
			closest_obj->rgb.b = 255.0;


		return (rgb_to_color(closest_obj->rgb));
	}
	return (0xFFFFFF);
}

void ray_tracing(void *mlx_ptr, char **image_data, t_scene scene)
{
	int x;
	int y;
	
	t_vector pixel_pos_3d;

	
	for (y = -CH/2; y <= CH/2; y++)
	{
		for (x = -CW/2; x <= CW/2; x++)
		{
			pixel_pos_3d = get_pixel_pisition(x, y);
			
			put_vector_to_image(*image_data, x + CW/2, -y + CH/2, cast_ray(scene.camera.center, pixel_pos_3d, DEPTH, scene));

		}
	}
}

int mouse_press(int key, int x, int y, void *param)
{
	t_RTv1 *RTv1;
	t_vector pixel_pos_3d;


	RTv1 = (t_RTv1 *)param;

	pixel_pos_3d = get_pixel_pisition(x - CW / 2, -y + CH / 2);

	RTv1->selected = get_closest_object(RTv1->camera.center, pixel_pos_3d, RTv1->scene);
	puts("lol");
}

int key_pressed(int key, void *param)
{
	t_RTv1 *RTv1;

	RTv1 = (t_RTv1 *)param;

	if (!RTv1->selected)
		RTv1->selected = &(RTv1->scene.objs[0]);

	if (key == 0xff53)
	{
		RTv1->selected->center.x += 0.1;
	}
	else if (key ==0xff51)
	{
		RTv1->selected->center.x -= 0.1;
	}
	else if (key ==0xff52)
	{
		RTv1->selected->center.y += 0.1;
	}
	else if (key ==0xff54)
	{
		RTv1->selected->center.y -= 0.1;
	}
	provider(*RTv1);
}

void RTv1_init(t_RTv1 *RTv1, char *file_name)
{
	int bytes;
	int len;
	int endian;

	bytes = 8;
	len = SIZE;
	endian = 0;
	RTv1->mlx_ptr = mlx_init();
	read_scene(&(RTv1->scene), file_name);

	RTv1->win_ptr = mlx_new_window(RTv1->mlx_ptr, SIZE, SIZE, "RTv1");
	RTv1->image = mlx_new_image(RTv1->mlx_ptr, SIZE, SIZE);
	RTv1->image_data = mlx_get_data_addr(RTv1->image, &bytes, &len, &endian);
}

void provider(t_RTv1 RTv1)
{
	ray_tracing(RTv1.mlx_ptr, &(RTv1.image_data), RTv1.scene);

	mlx_put_image_to_window(RTv1.mlx_ptr, RTv1.win_ptr, RTv1.image, 0, 0);
}

int main(int ac, char **av)
{
	t_RTv1 RTv1;

	RTv1_init(&RTv1, av[1]);

	provider(RTv1);

	

	mlx_hook(RTv1.win_ptr, 2, 1L << 0, key_pressed, &RTv1);
	mlx_hook(RTv1.win_ptr, 4, 0L, mouse_press, &RTv1);
//	mlx_hook(fractal->win_ptr, 5, 1L << 0, mouse_release, fractal);
//	mlx_hook(fractal->win_ptr, 6, 1L << 0, mouse_move, fractal);
//	mlx_hook(fractal->win_ptr, 17, 1L << 0, close_window, &windows_count);

	mlx_loop(RTv1.mlx_ptr);

	return (0);
}