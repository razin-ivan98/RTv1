#define CW 1000
#define CH 1000
#define VW 1
#define VH 1
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

	//struct s_obj *next;
}		t_obj;



typedef struct s_light
{
	int type;
	double intensity;
	t_vector center;
	t_vector dir;

//	struct s_light *next;
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
	t_light lights[10];
	t_obj objs[10];
}		t_scene;

typedef struct s_cl_obj
{
	int type;

	double angle;
	double radius;
	t_rgb rgb;
	double specular;
	double reflective;

	double3 center;
	double3 dir;

	//struct s_obj *next;
}		t_cl_obj;

typedef struct s_cl_camera
{
	double3 center;
}		t_cl_camera;

typedef struct s_cl_light
{
	int type;
	double intensity;

	double3 center;
	double3 dir;

//	struct s_light *next;
}				t_cl_light;


typedef struct s_cl_scene
{
	int c_objs;
	int c_lights;
	t_cl_camera camera;
	t_cl_light lights[10];
	t_cl_obj objs[10];
}		t_cl_scene;

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

void	put_point_to_image(__global char *image_data, int x, int y, int color)
{
	int	index = 0;

	if (x < 0 || y < 0 || x > CW - 1 || y > CH - 1)
		return ;
	else
	{
		index = CW * y * 4 + x * 4;
        image_data[index] = color & 0x0000ff;
		image_data[index + 2] = color >> 16;
		image_data[index + 1] = (color & 0x00ff00) >> 8;
	}
}

double3 vector_to_double3(t_vector a)
{
	return((double3)(a.x, a.y, a.z));
}

double3 get_pixel_pisition(int x, int y)
{
	double3 pixel_pos;
	double x3;
	double y3;

	x3 = (double)VW/(double)CW*(double)x;
	y3 = (double)VH/(double)CH*(double)y;

	pixel_pos = (double3)(x3, y3, 1.0);
	return (pixel_pos);
}

double ray_intersect_plane(double3 start, double3 dir, t_cl_obj *plane)
{
	double zeroThreshold = 0.0001;
	double dir_dot_c;
	double cen_dot_c;
	double t;

	start = start - plane->center;
	dir_dot_c = dot(dir, plane->dir);
	cen_dot_c = dot(start, plane->dir);

	if (dir_dot_c != 0.0)
	{
		t = -cen_dot_c / dir_dot_c;
		if (t < 100000.0)
			return(t);
	}
	return(0.0);

}


double ray_intersect_cylinder(double3 start, double3 dir, t_cl_obj *cyl)
{
	double zeroThreshold = 0.0001;

	start = start - cyl->center; // перенесем центр координат в центр сферы

	double dot_start_cyl_dir = dot(start, cyl->dir);
	double dot_dir_cyl_dir = dot(dir, cyl->dir);

	double a = dot(dir, dir) - dot_dir_cyl_dir * dot_dir_cyl_dir; // Здесь и далее операция * над векторами - скалярное произведение
	double b = 2 * (dot(dir, start) - dot_dir_cyl_dir * dot_start_cyl_dir);
	double c = dot(start, start) - dot_start_cyl_dir * dot_start_cyl_dir - cyl->radius * cyl->radius;
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

double ray_intersect_cone(double3 start, double3 dir, t_cl_obj *cone)
{
	double zeroThreshold = 0.0001;

	start = start - cone->center; // перенесем центр координат в центр сферы
	double k = tan(cone->angle);

	double dot_start_cone_dir = dot(start, cone->dir);
	double dot_dir_cone_dir = dot(dir, cone->dir);

	double a = dot(dir, dir) - (1 + k * k) * dot_dir_cone_dir * dot_dir_cone_dir; // Здесь и далее операция * над векторами - скалярное произведение
	double b = 2 * (dot(dir, start) - (1 + k * k) * dot_dir_cone_dir * dot_start_cone_dir);
	double c = dot(start, start) - (1 + k * k) * dot_start_cone_dir * dot_start_cone_dir;
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


double ray_intersect_sphere(double3 start, double3 dir, t_cl_obj *obj)
{
	double zeroThreshold = 0.0001;

	start = start - obj->center; // перенесем центр координат в центр сферы
	double a = dot(dir, dir); // Здесь и далее операция * над векторами - скалярное произведение
	double b = dot(start, dir);
	double c = dot(start, start) - obj->radius * obj->radius;
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

double ray_intersect_obj(double3 start, double3 dir, t_cl_obj *obj)
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

t_cl_obj *get_closest_object(double *closest_t, double3 start, double3 dir, t_cl_scene *cl_scene)
{
	double t = 0.0;
	int i = 0;
	t_obj *closest_obj = 0;

	while (i < cl_scene->c_objs)
	{
		t = ray_intersect_obj(start, dir, &(cl_scene->objs[i]));
		if (t != 0.0 && t < (*closest_t))
		{
			(*closest_t) = t;
			closest_obj = &(cl_scene->objs[i]);
		}
		i++;
	}
	return (closest_obj);
}

double3 vector_project(double3 a, double3 b)
{
	double3 project;
	double dot_ab_div_dot_bb;

	dot_ab_div_dot_bb = dot(a, b) / dot(b, b);
	project = b * dot_ab_div_dot_bb;
	return (project);
}

double3 get_normal(double3 point, t_cl_obj obj)
{
	double3 normal;
	double3 project;
	double k;

	if (obj.type == sphere)
	{
		normal = point - obj.center;
		normal = normalize(normal);
	}
	else if (obj.type == cylinder)
	{
		normal = point - obj.center;
		project = vector_project(normal, obj.dir);
		normal = normalize(normal - project);
	}
	else if (obj.type == plane)
		normal = obj.dir;
	else if (obj.type == cone)
	{
		normal = point - obj.center;
		project = vector_project(normal, obj.dir);
		k = 1 + tan(obj.angle) * tan(obj.angle);
		project = project * k;
		normal = normalize(normal - project);
	}
	return (normal);
}

double3 reflect_ray(double3 R, double3 N)
{
	return (N * (2 * dot(N, R)) - R);
}

double compute_lighting(double3 P, double3 N, double3 V, double s, t_cl_scene *cl_scene)
{
	double intensity = 0.0;

	double3 L;
	double t;
	double shadow_t;
	t_cl_obj *shadow_obj = 0;

	for (int i = 0; i < cl_scene->c_lights; i++)
	{
		shadow_t = 9999;
		int j = 0;
		if (cl_scene->lights[i].type == ambient)
			intensity += cl_scene->lights[i].intensity;
		else
		{
			if (cl_scene->lights[i].type == point)
				L = cl_scene->lights[i].center - P;
			else
				L = cl_scene->lights[i].dir;

			while (j < cl_scene->c_objs)
			{
				t = ray_intersect_obj(P, L, &(cl_scene->objs[j]));
				if (t != 0.0 && t < shadow_t)
				{
					shadow_t = t;
					shadow_obj = &(cl_scene->objs[j]);
				}
				j++;
			}
			if (shadow_obj)
				continue ;

			double n_dot_l = dot(N, L);
			if (n_dot_l > 0.0)
				intensity += cl_scene->lights[i].intensity * n_dot_l / (length(N) * length(L));

			if (s > 0.0)
			{
				double3 R = reflect_ray(L, N);
				double r_dot_v = dot(R, V);
				if (r_dot_v > 0.0)
					intensity += cl_scene->lights[i].intensity * pow(r_dot_v / (sqrt(dot(R, R)) * sqrt(dot(V, V))), s);
			}
		}
	}
	return (intensity);
}

int cast_ray(t_cl_scene *cl_scene, double3 start, double3 dir, int depth)
{
	t_cl_obj closest_obj;
	t_cl_obj *ptr;
	double closest_t;
	double intensity;


	closest_t = 99999.0;
	ptr = get_closest_object(&closest_t, start, dir, cl_scene);

	if (ptr)
	{
		closest_obj = *ptr;

		double3 P = start + dir * closest_t;

		double3 N = get_normal(P, closest_obj);

		intensity = compute_lighting(P, N, -dir, closest_obj.specular, cl_scene);

		if ((closest_obj.rgb.r *= intensity) >= 255.0)
			closest_obj.rgb.r = 255.0;
		if ((closest_obj.rgb.g *= intensity) >= 255.0)
			closest_obj.rgb.g = 255.0;
		if ((closest_obj.rgb.b *= intensity) >= 255.0)
			closest_obj.rgb.b = 255.0;

		if (depth <= 0 || closest_obj.reflective <= 0)
			return (rgb_to_color(closest_obj.rgb));////////////////////////////////break ;

		double3 R = reflect_ray((-1.0) * dir, N);
/*
		t_rgb reflected = color_to_rgb(cast_ray(cl_scene, P, R, depth - 1));

		if ((closest_obj.rgb.r = closest_obj.rgb.r * (1 - closest_obj.reflective) + reflected.r * closest_obj.reflective) > 255.0)
			closest_obj.rgb.r = 255.0;
		if ((closest_obj.rgb.g = closest_obj.rgb.g * (1 - closest_obj.reflective) + reflected.g * closest_obj.reflective) > 255.0)
			closest_obj.rgb.g = 255.0;
		if ((closest_obj.rgb.b = closest_obj.rgb.b * (1 - closest_obj.reflective) + reflected.b * closest_obj.reflective) > 255.0)
			closest_obj.rgb.b = 255.0;*/
		return (rgb_to_color(closest_obj.rgb));
	}
	return(0x000000);
}

__kernel void mishania(__global char *image_data, __global t_scene *scene)
{
	int x = get_global_id(0) / CH;
	int y = get_global_id(0) % CH;
	double3 pixel_pos_3d;

	t_cl_scene cl_scene;
	int i = 0;
	int j = 0;
	t_vector ptr;

	ptr = scene->camera.center;
	cl_scene.camera.center = vector_to_double3(ptr);

	cl_scene.c_lights = scene->c_lights;
	cl_scene.c_objs = scene->c_objs;

	while (i < scene->c_objs)
	{
		ptr = scene->objs[i].center;
		cl_scene.objs[i].center = vector_to_double3(ptr);
		ptr = scene->objs[i].dir;
		cl_scene.objs[i].dir = vector_to_double3(ptr);
		cl_scene.objs[i].rgb = scene->objs[i].rgb;
		cl_scene.objs[i].radius = scene->objs[i].radius;
		cl_scene.objs[i].type = scene->objs[i].type;
		cl_scene.objs[i].reflective = scene->objs[i].reflective;
		cl_scene.objs[i].specular = scene->objs[i].specular;
		cl_scene.objs[i].angle = scene->objs[i].angle;
		i++;
	}
	while (j < scene->c_lights)
	{
		cl_scene.lights[j].type = scene->lights[j].type;
		ptr = scene->lights[j].center;
		cl_scene.lights[j].center = vector_to_double3(ptr);
		ptr = scene->lights[j].dir;
		cl_scene.lights[j].dir = vector_to_double3(ptr);
		cl_scene.lights[j].intensity = scene->lights[j].intensity;
		j++;
	}

	pixel_pos_3d = get_pixel_pisition(x - CW / 2, -y + CH / 2);
	put_point_to_image(image_data, x, y, cast_ray(&cl_scene, cl_scene.camera.center, pixel_pos_3d, DEPTH));


	//double3 camera = (double3)(scene->camera.center.x, scene->camera.center.y, scene->camera.center.z);//а это работает
}