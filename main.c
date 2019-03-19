#include "RTv1.h"

#include <stdio.h>
# define SIZE 700
# define VW 1
# define VH 1
# define CW 700
# define CH 700
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

double compute_lighting(t_vector P, t_vector N, t_vector V, double s, t_obj objs[])
{
	double intensity = 0.0;

	t_light lights[3];
	t_vector L;
	double t;
	double shadow_t;
	t_obj *shadow_obj = NULL;

	lights[0].type = directional;
	lights[0].intensity = 0.2;
	lights[0].direction = vector_init(2.0, 4.0, 4.0);

	lights[1].type = point;
	lights[1].intensity = 0.6;
	lights[1].position = vector_init(4.0, 5.0, 3.0);

	lights[2].type = ambient;
	lights[2].intensity = 0.2;

	for (int i = 0; i < 3; i++)
	{
		shadow_t = 9999;
		shadow_obj = NULL;
		if (lights[i].type == ambient)
			intensity += lights[i].intensity;
		else 
		{
			if (lights[i].type == point)
				L = vector_subt(lights[i].position, P);
			else 
				L = lights[i].direction;
		

			for (int j = 0; j <  6; j++)
			{
				t = ray_intersect_obj(P, L, objs[j]);
				if (t != 0.0 && t < shadow_t)
				{
					shadow_t = t;
					shadow_obj = &(objs[i]);
				}
				
					
			}
			if (shadow_obj)
				continue ;





			double n_dot_l = scal_mult(N, L);
			if (n_dot_l > 0.0)
				intensity += lights[i].intensity * n_dot_l / (sqrt(scal_mult(N, N)) * sqrt(scal_mult(L, L)));
		
			if (s > 0.0)
			{
				t_vector R = reflect_ray(L, N);
				double r_dot_v = scal_mult(R, V);
				if (r_dot_v > 0.0)
					intensity += lights[i].intensity * pow(r_dot_v / (sqrt(scal_mult(R, R)) * sqrt(scal_mult(V, V))), s);
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

int cast_ray(t_vector start, t_vector dir, int depth)
{
	t_obj obj[7];
	t_obj *closest_obj = NULL;
	double closest_t = 999999.0;
	double intensity;

	double t = 0.0;
	obj[0].type = sphere;
	obj[0].center = vector_init(0.0, -1.0, 3.0);
	obj[0].radius = 1.0;
	obj[0].rgb = color_to_rgb(0xFF0000);
	obj[0].specular = 100.0;
	obj[0].reflective = 0.5;

	obj[1].type = sphere;
	obj[1].center = vector_init(2.0, 0.0, 4.0);
	obj[1].radius = 1.0;
	obj[1].rgb = color_to_rgb(0x0000FF);
	obj[1].specular = 500;
	obj[1].reflective = 0.3;
	
	obj[2].type = sphere;
	obj[2].center = vector_init(-2.0, 0.0, 4.0);
	obj[2].radius = 1.0;
	obj[2].rgb = color_to_rgb(0x00FF00);
	obj[2].specular = 300;
	obj[2].reflective = 0.0;

	obj[3].type = sphere;
	obj[3].center = vector_init(0.0, -5001.0, 0.0);
	obj[3].radius = 5000.0;
	obj[3].rgb = color_to_rgb(0xFFFF00);
	obj[3].specular = 100;
	obj[3].reflective = 0.1;

	obj[4].type = cone;
	obj[4].angle = 10.0f * (M_PI / 180);
	obj[4].dir = vector_normalize(vector_init(2.0, 1.0, 1.0));
	obj[4].center = vector_init(0.0, 1.5, 4.3);
	obj[4].rgb = color_to_rgb(0xFF00FF);
	obj[4].specular = 0;
	obj[4].reflective = 0.0;

	obj[5].type = cylinder;
	obj[5].radius = 0.5;
	obj[5].dir = vector_normalize(vector_init(-0.2, 1.0, 0.8));
	obj[5].center = vector_init(1.2, 1.5, 4.8);
	obj[5].rgb = color_to_rgb(0xFF0000);
	obj[5].specular = 1;
	obj[5].reflective = 0.0;

	obj[6].type = plane;
	obj[6].dir = vector_init(0.0, 0.0, 1.0);
	obj[6].center = vector_init(0, 0, 800);
	obj[6].rgb = color_to_rgb(0xFF00FF);
	obj[6].specular = 0;
	obj[6].reflective = 0.0;

	for (int i = 0; i < 6; i++)
	{
		t = ray_intersect_obj(start, dir, obj[i]);
		if (t != 0.0 && t < closest_t)
		{
			closest_t = t;
			closest_obj = &(obj[i]);
		}
			
	}

	if (!closest_obj)
		return(0x000000);

	if (closest_obj)
	{
		

		t_vector P = vector_sum(start, vector_int_mult(dir, closest_t));


		t_vector N = get_normal(P, *closest_obj);
		
		/*vector_subt(P, closest_obj->center);
		N = vector_int_div(N, sqrt(scal_mult(N, N)));*/

		intensity = compute_lighting(P, N, vector_int_mult(dir, -1.0), closest_obj->specular, obj);


		if ((closest_obj->rgb.r *= intensity) >= 255.0) 
			closest_obj->rgb.r = 255.0;
		if ((closest_obj->rgb.g *= intensity) >= 255.0) 
			closest_obj->rgb.g = 255.0;
		if ((closest_obj->rgb.b *= intensity) >= 255.0) 
			closest_obj->rgb.b = 255.0;

		
		if (depth <= 0 || closest_obj->reflective <= 0)
			return (rgb_to_color(closest_obj->rgb));
		

		t_vector R = reflect_ray(vector_int_mult(dir, -1.0), N);
	
		t_rgb reflected = color_to_rgb(cast_ray(P, R, depth - 1));

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

void ray_tracing(void *mlx_ptr, char **image_data)
{
	int x;
	int y;
	t_vector camera = vector_init(0.0, 0.0, -2);
	t_vector pixel_pos_3d;

	
	for (y = -CH/2; y <= CH/2; y++)
	{
		for (x = -CW/2; x <= CW/2; x++)
		{
			pixel_pos_3d = get_pixel_pisition(x, y);
			
				put_vector_to_image(*image_data, x + CW/2, -y + CH/2, cast_ray(camera, pixel_pos_3d, DEPTH));

		}
	}
}




int main()
{
	void *mlx_ptr;
	void *win_ptr;
	void *image;
	char *image_data;

	int bytes;
	int len;
	int endian;

	bytes = 8;
	len = SIZE;
	endian = 0;
	mlx_ptr = mlx_init();
	win_ptr = mlx_new_window(mlx_ptr, SIZE, SIZE, "RTv1");
	image = mlx_new_image(mlx_ptr, SIZE, SIZE);
	image_data = mlx_get_data_addr(image, &bytes, &len, &endian);

	ray_tracing(mlx_ptr, &image_data);

	mlx_put_image_to_window(mlx_ptr, win_ptr, image, 0, 0);

	mlx_loop(mlx_ptr);

	return (0);
}