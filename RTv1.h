#ifndef RTV1_H
# define RTV1_H
# include "math.h"
# include <fcntl.h>
# include "get_next_line.h"
# include <OpenCL/opencl.h>
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
	t_vector center;
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

typedef struct s_RTv1
{
	void *mlx_ptr;
	void *win_ptr;
	void *image;
	char *image_data;
	t_obj *selected;

	t_scene scene;

	cl_int				ret;
	cl_platform_id		platform_id;
	cl_uint				ret_num_platforms;

	cl_device_id		device_id;
	cl_uint				ret_num_devices;

	cl_context			context;
	cl_command_queue	command_queue;

	cl_program			program;
	cl_kernel			kernel;

	char				file_name[40];
	char				kernel_name[40];

	size_t				source_size;
	char				*source_str;
	cl_mem				memobj;

	cl_mem				utils_memobj;
}			t_RTv1;

void read_scene(t_scene *scene, char* file_name);

double ft_atof(char *str);


void provider(t_RTv1 *RTv1);

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