#define SIZE 1000
#define VW 1
#define VH 1
#define CW 1000
#define CH 1000
#define DEPTH 0

typedef enum
{
	point,
	directional,
	ambient
}			light_type;

typedef enum
{
	sphere,
	cone,
	cylinder,
	plane,
	triangle
}			obj_type;

typedef struct s_vector
{
	double x;
	double y;
	double z;

}		t_vector;

typedef struct s_ray
{
	t_vector start;
	t_vector dir;
}			t_ray;

typedef struct s_rgb
{
	int r;
	int g;
	int b;
}				t_rgb;

typedef struct s_obj
{
	int type;
	t_vector center;
	t_vector dir;
	double angle;
	double radius;
	t_rgb rgb;
	double specular;
	double reflective;

}		t_obj;



typedef struct s_light
{
	int type;
	double intensity;
	t_vector center;
	t_vector direction;

}				t_light;

typedef struct s_camera
{
	t_vector center;
}		t_camera;



typedef struct s_scene
{
	int c_objs;
	int c_lights;
	t_camera camera;
	t_light lights;
	t_obj objs;
}		t_scene;


t_rgb color_to_rgb(int color)
{
	t_rgb rgb;

	rgb.r = color >> 16;
	rgb.g = (color & 0x00ff00) >> 8;
	rgb.b = color & 0x0000ff;

	return (rgb);
}

int rgb_to_color(t_rgb rgb)
{
	int color;

	color = rgb.r * 0x010000 + rgb.g * 0x000100 + rgb.b;

	return (color);
}

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

void	put_point_to_image(__global char *image_data, int x, int y, int color)
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

void	clear_image_data(__global char *image_data)
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
	t_ray ray;

	double zeroThreshold = 0.0001;

	ray.start = start;
	ray.dir = dir;

	ray.start = vector_subt(ray.start, cyl->center); // перенесем центр координат в центр сферы

	double a = scal_mult(ray.dir, ray.dir) - scal_mult(ray.dir, cyl->dir) * scal_mult(ray.dir, cyl->dir); // Здесь и далее операция * над векторами - скалярное произведение
	double b = 2 * (scal_mult(ray.dir, ray.start) - scal_mult(ray.dir, cyl->dir) * scal_mult(ray.start, cyl->dir));
	double c = scal_mult(ray.start, ray.start) - scal_mult(ray.start, cyl->dir) * scal_mult(ray.start, cyl->dir) - cyl->radius * cyl->radius;
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
	t_ray ray;

	double zeroThreshold = 0.0001;

	ray.start = start;
	ray.dir = dir;

	ray.start = vector_subt(ray.start, cone->center); // перенесем центр координат в центр сферы
	double k = tan(cone->angle);

	double a = scal_mult(ray.dir, ray.dir) - (1 + k * k) * ((scal_mult(ray.dir, cone->dir) * (scal_mult(ray.dir, cone->dir)))); // Здесь и далее операция * над векторами - скалярное произведение
	double b = 2 * (scal_mult(ray.dir, ray.start) - (1 + k * k) * scal_mult(ray.dir, cone->dir) * scal_mult(ray.start, cone->dir));
	double c = scal_mult(ray.start, ray.start) - (1 + k * k) * ((scal_mult(ray.start, cone->dir)* (scal_mult(ray.start, cone->dir))));
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
	t_ray ray;

	double zeroThreshold = 0.0001;

	ray.start = start;
	ray.dir = dir;

	ray.start = vector_subt(ray.start, obj->center); // перенесем центр координат в центр сферы
	double a = scal_mult(ray.dir, ray.dir); // Здесь и далее операция * над векторами - скалярное произведение
	double b = scal_mult(ray.start, ray.dir);
	double c = scal_mult(ray.start, ray.start) - obj->radius * obj->radius;
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
	return(0.0);
}

double compute_lighting(t_vector P, t_vector N, t_vector V, double s, __global t_scene *scene)
{
	double intensity = 0.0;

	//t_light lights[3];
	t_vector L;
	double t;
	double shadow_t;
	t_obj *shadow_obj = 0;

	for (int i = 0; i < scene->c_lights; i++)
	{
		shadow_t = 9999;
		int j = 0;
		shadow_obj = 0;
		if (scene->lights[i].type == ambient)
			intensity += scene->lights[i].intensity;
		else
		{
			if (scene->lights[i].type == point)
				L = vector_subt(scene->lights[i].center, P);
			else
				L = scene->lights[i].direction;

			while (j < scene->c_objs)
			{
				t = ray_intersect_obj(P, L, &(scene->objs[j]));
				if (t != 0.0 && t < shadow_t)
				{
					shadow_t = t;
					shadow_obj = &(scene->objs[j]);
				}
				j++;
			}
			if (shadow_obj)
				continue ;

			double n_dot_l = scal_mult(N, L);
			if (n_dot_l > 0.0)
				intensity += scene->lights[i].intensity * n_dot_l / (sqrt(scal_mult(N, N)) * sqrt(scal_mult(L, L)));

			if (s > 0.0)
			{
				t_vector R = reflect_ray(L, N);
				double r_dot_v = scal_mult(R, V);
				if (r_dot_v > 0.0)
					intensity += scene->lights[i].intensity * pow(r_dot_v / (sqrt(scal_mult(R, R)) * sqrt(scal_mult(V, V))), s);
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

t_obj *get_closest_object(double *closest_t, t_vector start, t_vector dir, __global t_scene *scene)
{
	double t = 0.0;
	int i = 0;
	t_obj *closest_obj = 0;



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

int cast_ray(__global t_scene *scene, t_vector start, t_vector dir, int depth)
{

	t_obj closest_obj;
	t_obj *ptr;
	double closest_t;
	closest_t = 99999.0;
	double intensity;
	double t = 0.0;
	int i = 0;

	ptr = get_closest_object(&closest_t, start, dir, scene);
	if (!ptr)
		return(0x000000);

	if (ptr)
	{
		closest_obj = *ptr;

		t_vector P = vector_sum(start, vector_int_mult(dir, closest_t));

		t_vector N = get_normal(P, closest_obj);

		intensity = compute_lighting(P, N, vector_int_mult(dir, -1.0), closest_obj.specular, scene);

		if ((closest_obj.rgb.r *= intensity) >= 255.0)
			closest_obj.rgb.r = 255.0;
		if ((closest_obj.rgb.g *= intensity) >= 255.0)
			closest_obj.rgb.g = 255.0;
		if ((closest_obj.rgb.b *= intensity) >= 255.0)
			closest_obj.rgb.b = 255.0;

		if (depth <= 0 || closest_obj.reflective <= 0)
			return (rgb_to_color(closest_obj.rgb));

		t_vector R = reflect_ray(vector_int_mult(dir, -1.0), N);

		t_rgb reflected = color_to_rgb(cast_ray(scene, P, R, depth - 1));

		if ((closest_obj.rgb.r = closest_obj.rgb.r * (1 - closest_obj.reflective) + reflected.r * closest_obj.reflective) > 255.0)
			closest_obj.rgb.r = 255.0;
		if ((closest_obj.rgb.g = closest_obj.rgb.g * (1 - closest_obj.reflective) + reflected.g * closest_obj.reflective) > 255.0)
			closest_obj.rgb.g = 255.0;
		if ((closest_obj.rgb.b = closest_obj.rgb.b * (1 - closest_obj.reflective) + reflected.b * closest_obj.reflective) > 255.0)
			closest_obj.rgb.b = 255.0;
		return (rgb_to_color(closest_obj.rgb));
	}
	return (0xFFFFFF);
}

__kernel void kernel(__global char *image_data, __global t_scene *scene)
{
	int x = get_global_id(0) / CW;
	int y = get_global_id(0) % CH;
	t_vector pixel_pos_3d;

	pixel_pos_3d = get_pixel_pisition(x - CW / 2, -y + CH / 2);
	put_point_to_image(image_data, x + CW/2, -y + CH/2, cast_ray(scene, scene->camera.center, pixel_pos_3d, DEPTH));
}
