/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_dir_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 11:02:26 by chorange          #+#    #+#             */
/*   Updated: 2019/04/12 11:15:42 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	set_start_angles(t_scene *scene)
{
	t_vector vector_yz;
	t_vector vector_xz;
	t_vector main_dir;

	vector_yz = scene->camera.dir;
	vector_yz.x = 0;
	vector_xz = scene->camera.dir;
	vector_xz.y = 0;
	main_dir = vector_init(0, 0, 1);
	scene->view_alpha = acos(scal_mult(vector_yz, main_dir) /
		(vector_length(vector_yz) * vector_length(main_dir)));
	scene->view_beta = acos(scal_mult(vector_xz, main_dir) /
		(vector_length(vector_xz) * vector_length(main_dir)));
	if (scene->view_alpha > M_PI / 2)
		scene->view_alpha = M_PI - scene->view_alpha;
	if (vector_yz.y < 0)
		scene->view_alpha *= (-1);
	if (vector_xz.x < 0)
		scene->view_beta *= (-1);
	scene->view_beta = -1 * scene->view_beta;
}
