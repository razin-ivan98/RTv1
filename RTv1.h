#ifndef RTV1_H
# define RTV1_H
# include "math.h"
# include <fcntl.h>
# include "get_next_line.h"
# include "mlx.h"

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

	//struct s_obj *next;
}		t_obj;



typedef struct s_light
{
	int type;
	double intensity;
	t_vector position;
	t_vector direction;

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

void read_scene(t_scene *scene, char* file_name);

double ft_atof(char *str);

t_vector vector_normalize(t_vector a);
t_vector vector_project(t_vector a, t_vector b);
t_vector vector_subt(t_vector a, t_vector b);
t_vector vector_sum(t_vector a, t_vector b);
t_vector vector_int_mult(t_vector a, double b);
t_vector vector_int_div(t_vector a, double b);
double scal_mult(t_vector a, t_vector b);
void obj_init(t_obj *obj);
t_vector vector_init(double x, double y, double z);

t_rgb color_to_rgb(int color);
int rgb_to_color(t_rgb rgb);

#endif