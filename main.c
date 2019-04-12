/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 19:25:19 by cocummin          #+#    #+#             */
/*   Updated: 2019/04/12 13:25:27 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		provider(t_rtv1 *rtv1)
{
	size_t global_work_size;

	global_work_size = CW * CH;
	rtv1->ret = clEnqueueWriteBuffer(rtv1->command_queue,
			rtv1->utils_memobj, CL_TRUE, 0,
			sizeof(t_scene), &(rtv1->scene), 0, NULL, NULL);
	rtv1->ret = clSetKernelArg(rtv1->kernel, 1,
			sizeof(cl_mem), (void *)&rtv1->utils_memobj);
	rtv1->ret = clEnqueueNDRangeKernel(rtv1->command_queue, rtv1->kernel, 1,
			NULL, &global_work_size, NULL, 0, NULL, NULL);
	rtv1->ret = clEnqueueReadBuffer(rtv1->command_queue, rtv1->memobj,
			CL_TRUE, 0, CW * CH * 4, rtv1->image_data, 0, NULL, NULL);
	mlx_put_image_to_window(rtv1->mlx_ptr, rtv1->win_ptr, rtv1->image, 0, 0);
	if (!(rtv1->selected))
		rtv1->selected = &(rtv1->scene.objs[0]);
}

int			main(int ac, char **av)
{
	static t_rtv1 rtv1;

	if (ac != 2)
	{
		ft_putendl("Argument is not valid. Please, set number of \
scene in range from 1 to 8 as a argument");
		err_exit();
	}
	scene_init(&rtv1, av[1]);
	graphics_init(&rtv1);
	set_start_angles(&(rtv1.scene));
	provider(&rtv1);
	mlx_hook(rtv1.win_ptr, 2, 0, key_pressed, &rtv1);
	mlx_hook(rtv1.win_ptr, 4, 0, mouse_pressed, &rtv1);
	mlx_hook(rtv1.win_ptr, 5, 0, mouse_release, &rtv1);
	mlx_hook(rtv1.win_ptr, 6, 0, mouse_move, &rtv1);
	mlx_hook(rtv1.win_ptr, 17, 0, clean_exit, &rtv1);
	mlx_loop(rtv1.mlx_ptr);
	return (0);
}
