#include "RTv1.h"

void reading_error(void)
{
	puts("error");
	exit(-2);
}
/*
t_obj *new_object(t_scene *scene)
{
	t_obj **tmp;

	tmp = &(scene->objs);
	if (!(*tmp))
	{
		(*tmp) = (t_obj *)malloc(sizeof(t_obj));//check
		(*tmp)->next = NULL;
		puts("first obj");
		return (*tmp);
		
	}
	while (*tmp)
		*tmp = (*tmp)->next;
	(*tmp) = (t_obj *)malloc(sizeof(t_obj));//check
	(*tmp)->next = NULL;
	puts("some else obj");
	return (*tmp);
}*/

t_vector read_vector(char *str)
{
	t_vector ret;

	while (*str == ' ' || *str == '\t' || *str == '(')
		str++;
	ret.x = ft_atof(str);
	str = strchr(str, ',') + 1;
	ret.y = ft_atof(str);
	str = strchr(str, ',') + 1;
	ret.z = ft_atof(str);

	return (ret);
}

void set_obj_type(char *line, t_obj *obj)
{
	if (strstr(line, "sphere"))
		obj->type = sphere;
	else if (strstr(line, "cylinder"))
		obj->type = cylinder;
	else if (strstr(line, "cone"))
		obj->type = cone;
	else if (strstr(line, "plane"))
		obj->type = plane;
	else
		reading_error();
}

t_obj *read_obj_parameters(char *line, t_obj **obj)
{
	if (strchr(line, '}'))
		return (NULL);
	else if (strstr(line, "type"))
		set_obj_type(line, *obj);
	else if (strstr(line, "position"))
		(*obj)->center = read_vector(strchr(line, '=') + 1);
	else if (strstr(line, "radius"))
		(*obj)->radius = ft_atof(strchr(line, '=') + 1);
	else if (strstr(line, "color"))
		(*obj)->rgb = color_to_rgb(atoi(strchr(line, '=') + 1));
	else if (strstr(line, "angle"))
		(*obj)->angle = ft_atof(strchr(line, '=') + 1);
	else if (strstr(line, "direction"))
		(*obj)->dir = vector_normalize(read_vector(strchr(line, '=') + 1));
	else if (strstr(line, "specular"))
		(*obj)->specular = ft_atof(strchr(line, '=') + 1);
	else if (strstr(line, "reflective"))
		(*obj)->reflective = ft_atof(strchr(line, '=') + 1);
	return (*obj);
}

t_camera *camera_init(char *line, t_camera *camera)
{
	if (strchr(line, '}'))
	{
	//	puts("camera inited");
		return (NULL);
	}
	else if (strstr(line, "position"))
		camera->center = read_vector(strchr(line, '=') + 1);
	return (camera);
}

void read_line_set_scene(char *line, t_scene *scene)
{
	static int i = -1;
	static t_obj *obj = NULL;
	static t_light *light = NULL;
	static t_camera *camera = NULL;
	if (!strcmp(line, "camera_init"))
		camera = camera_init(line, &(scene->camera));



	if (obj)
	{
		//puts(line);
		obj = read_obj_parameters(line, &obj);
		//printf("color     - %d\n", rgb_to_color(obj->rgb));
	}

	else if (camera)
	{
		camera = camera_init(line, camera);
	}

	else if (strstr(line, "new"))
	{
		if (strstr(line, "object"))
		{
			//puts("new object");
			//obj = new_object(scene);
			i++;
			obj = &(scene->objs[i]);
		}
		
		/*else if (strstr(line, "light"))
		{
			light = new_light(&(scene->lights));
		}*/
	}
	
}

void read_scene(t_scene *scene, char* file_name)
{
	int fd;
	char *line;

	fd = open(file_name, O_RDONLY);//check
	while (get_next_line(fd, &line) == 1)
	{
		read_line_set_scene(line, scene);

		free(line);
	}
	free(line);
	close(fd);
}