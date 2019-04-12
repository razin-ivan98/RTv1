/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 11:10:50 by chorange          #+#    #+#             */
/*   Updated: 2019/04/12 12:53:22 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		scene_init(t_rtv1 *rtv1, char *number)
{
	char file_name[32];

	if (!ft_strcmp(number, "1"))
		ft_strcpy(file_name, "scenes/v1.rts");
	else if (!ft_strcmp(number, "2"))
		ft_strcpy(file_name, "scenes/v2.rts");
	else if (!ft_strcmp(number, "3"))
		ft_strcpy(file_name, "scenes/v3.rts");
	else if (!ft_strcmp(number, "4"))
		ft_strcpy(file_name, "scenes/v4.rts");
	else if (!ft_strcmp(number, "5"))
		ft_strcpy(file_name, "scenes/v5.rts");
	else if (!ft_strcmp(number, "6"))
		ft_strcpy(file_name, "scenes/v6.rts");
	else if (!ft_strcmp(number, "7"))
		ft_strcpy(file_name, "scenes/v7.rts");
	else if (!ft_strcmp(number, "8"))
		ft_strcpy(file_name, "scenes/v8.rts");
	else
	{
		ft_putendl(ERR);
		err_exit();
	}
	rtv1->scene.shadows_on = 1;
	read_scene(&(rtv1->scene), file_name);
}

static void	compile_from_file(char *file_name, t_rtv1 *rtv1)
{
	size_t	source_size;
	char	*source_str;
	int		fd;

	fd = open(file_name, O_RDONLY);
	if (fd <= 0)
	{
		err_exit();
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_str[read(fd, source_str, MAX_SOURCE_SIZE)] = 0;
	source_size = ft_strlen(source_str);
	close(fd);
	rtv1->program = clCreateProgramWithSource(rtv1->context, 1,
		(const char **)&source_str, (const size_t *)&source_size, &rtv1->ret);
	rtv1->ret = clBuildProgram(rtv1->program, 1, &rtv1->device_id,
		NULL, NULL, NULL);
	free(source_str);
}

static void	kernel_init(t_rtv1 *rtv1)
{
	rtv1->ret = clGetPlatformIDs(1, &rtv1->platform_id,
		&rtv1->ret_num_platforms);
	rtv1->ret = clGetDeviceIDs(rtv1->platform_id, CL_DEVICE_TYPE_GPU,
		1, &rtv1->device_id, &rtv1->ret_num_devices);
	rtv1->context = clCreateContext(NULL, 1, &rtv1->device_id, NULL,
		NULL, &rtv1->ret);
	rtv1->command_queue = clCreateCommandQueue(rtv1->context, rtv1->device_id,
		0, &rtv1->ret);
	compile_from_file("kernels/kernel.cl", rtv1);
	rtv1->kernel = clCreateKernel(rtv1->program, "mishania", &rtv1->ret);
	rtv1->memobj = clCreateBuffer(rtv1->context, CL_MEM_READ_WRITE,
		CW * CH * 4, NULL, &rtv1->ret);
	rtv1->ret = clEnqueueWriteBuffer(rtv1->command_queue, rtv1->memobj,
		CL_TRUE, 0, CW * CH * 4, rtv1->image_data, 0, NULL, NULL);
	rtv1->ret = clSetKernelArg(rtv1->kernel, 0, sizeof(cl_mem),
		(void *)&rtv1->memobj);
	rtv1->utils_memobj = clCreateBuffer(rtv1->context,
		CL_MEM_READ_WRITE, sizeof(t_scene), NULL, &rtv1->ret);
}

void		graphics_init(t_rtv1 *rtv1)
{
	int	bytes;
	int	len;
	int	endian;

	bytes = 8;
	len = CW;
	endian = 0;
	rtv1->guide_on = 0;
	rtv1->mlx_ptr = mlx_init();
	rtv1->win_ptr = mlx_new_window(rtv1->mlx_ptr, CW, CH, "rtv1");
	rtv1->image = mlx_new_image(rtv1->mlx_ptr, CW, CH);
	rtv1->image_data = mlx_get_data_addr(rtv1->image, &bytes, &len, &endian);
	kernel_init(rtv1);
}
