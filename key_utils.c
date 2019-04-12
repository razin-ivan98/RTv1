/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 12:11:52 by chorange          #+#    #+#             */
/*   Updated: 2019/04/12 12:54:09 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int	mouse_pressed(int button, int x, int y, t_rtv1 *rtv1)
{
	rtv1->prev_x = x;
	rtv1->prev_y = y;
	rtv1->selected_t = 9999999.9;
	if (y < 0)
		return (0);
	if (button == 1)
		select_object(rtv1, x, y);
	else if (button == 4)
		rtv1->scene.camera.center.z -= 0.2;
	else if (button == 5)
		rtv1->scene.camera.center.z += 0.2;
	else if (button == 2)
		rtv1->right_mouse_pressed = 1;
	provider(rtv1);
	return (0);
}

int	mouse_release(int button, int x, int y, t_rtv1 *rtv1)
{
	(void)x;
	(void)y;
	(void)button;
	rtv1->left_mouse_pressed = 0;
	rtv1->right_mouse_pressed = 0;
	provider(rtv1);
	return (0);
}

int	mouse_move(int x, int y, t_rtv1 *rtv1)
{
	int dx;
	int dy;

	if (rtv1->right_mouse_pressed)
	{
		dx = x - rtv1->prev_x;
		dy = y - rtv1->prev_y;
		rtv1->scene.view_alpha += atan((double)dy * 0.001);
		rtv1->scene.view_beta += atan((double)dx * 0.001);
	}
	else if (rtv1->left_mouse_pressed)
	{
		dx = x - rtv1->prev_x;
		dy = y - rtv1->prev_y;
		if (!(rtv1->selected))
			rtv1->selected = &(rtv1->scene.objs[0]);
		rtv1->selected->center.x += 0.001 * dx * rtv1->selected_t;
		rtv1->selected->center.y -= 0.001 * dy * rtv1->selected_t;
	}
	rtv1->prev_x = x;
	rtv1->prev_y = y;
	provider(rtv1);
	return (0);
}

int	key_pressed(int key, t_rtv1 *rtv1)
{
	if (!(rtv1->selected))
		rtv1->selected = &(rtv1->scene.objs[0]);
	if (key == 0x35)
		clean_exit(rtv1);
	else if (key == 0x7C)
		rtv1->scene.camera.center.x += 0.5;
	else if (key == 0x7B)
		rtv1->scene.camera.center.x -= 0.5;
	else if (key == 0x7E)
		rtv1->scene.camera.center.y += 0.5;
	else if (key == 0x7D)
		rtv1->scene.camera.center.y -= 0.5;
	else if (key == 0x1B)
		rtv1->selected->center.z += 0.2;
	else if (key == 0x18)
		rtv1->selected->center.z -= 0.2;
	else if (key == 0x01)
	{
		if (rtv1->scene.shadows_on == 1)
			rtv1->scene.shadows_on = 0;
		else
			rtv1->scene.shadows_on = 1;
	}
	provider(rtv1);
	return (0);
}
