/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 14:11:17 by chorange          #+#    #+#             */
/*   Updated: 2019/03/21 15:45:57 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RTv1.h"

void compile_from_file(char *file_name, t_RTv1 *RTv1)
{
    size_t l_size;
    size_t source_size;
    char *source_str;

    int fd;

    fd = open(file_name, O_RDONLY);
    if (fd <= 0)
    {
        ft_putendl("Failed to load kernel.");
        exit(-2);
    }
    source_str = (char *)malloc(MAX_SOURCE_SIZE);
    source_str[read(fd, source_str, MAX_SOURCE_SIZE)] = 0;
    source_size = strlen(source_str);
    close(fd);

    RTv1->program = clCreateProgramWithSource(RTv1->context, 1, (const char **)&source_str, (const size_t *)&source_size, &RTv1->ret);
    RTv1->ret = clBuildProgram(RTv1->program, 1, &RTv1->device_id, NULL, NULL, NULL);
    if (RTv1->ret != 0)
    {
        char *log;
        ft_putendl("Could not compile pregram\n");
        clGetProgramBuildInfo(RTv1->program, RTv1->device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &l_size);
        log = malloc(l_size);
        clGetProgramBuildInfo(RTv1->program, RTv1->device_id, CL_PROGRAM_BUILD_LOG, l_size, log, NULL);
        ft_putendl(log);
        free(log);
        exit(-2);
    }
    free(source_str);

}

void kernel_init(t_RTv1 *RTv1)
{

    RTv1->ret = clGetPlatformIDs(1, &RTv1->platform_id, &RTv1->ret_num_platforms);
    RTv1->ret = clGetDeviceIDs(RTv1->platform_id, CL_DEVICE_TYPE_GPU, 1, &RTv1->device_id, &RTv1->ret_num_devices);
    RTv1->context = clCreateContext(NULL, 1, &RTv1->device_id, NULL, NULL, &RTv1->ret);
    RTv1->command_queue = clCreateCommandQueue(RTv1->context, RTv1->device_id, 0, &RTv1->ret);

    compile_from_file("kernels/kernel.cl", RTv1);

    RTv1->kernel = clCreateKernel(RTv1->program, "mishania", &RTv1->ret);
    RTv1->memobj = clCreateBuffer(RTv1->context, CL_MEM_READ_WRITE, CW * CH * 4, NULL, &RTv1->ret);
    RTv1->ret = clEnqueueWriteBuffer(RTv1->command_queue, RTv1->memobj, CL_TRUE, 0, CW * CH * 4, RTv1->image_data, 0, NULL, NULL);
    RTv1->ret = clSetKernelArg(RTv1->kernel, 0, sizeof(cl_mem), (void *)&RTv1->memobj);
	RTv1->utils_memobj = clCreateBuffer(RTv1->context,
			CL_MEM_READ_WRITE, sizeof(t_scene), NULL, &RTv1->ret);
}

void graphics_init(t_RTv1 *RTv1)
{
    int bytes;
    int len;
    int endian;

    bytes = 8;
	len = CW;
	endian = 0;
    RTv1->mlx_ptr = mlx_init();

	RTv1->win_ptr = mlx_new_window(RTv1->mlx_ptr, CW, CH, "RTv1");
	RTv1->image = mlx_new_image(RTv1->mlx_ptr, CW, CH);
	RTv1->image_data = mlx_get_data_addr(RTv1->image, &bytes, &len, &endian);


    kernel_init(RTv1);
}
