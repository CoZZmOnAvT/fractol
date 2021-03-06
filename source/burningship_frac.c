/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   burningship_frac.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/20 17:07:51 by pgritsen          #+#    #+#             */
/*   Updated: 2018/01/02 20:32:12 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	ft_burninghelp(t_window *win)
{
	mlx_string_put(win->env->mlx_p, win->win_p, 100, 105,
					0x8C8C, "BurningShip Set HELP");
	mlx_string_put(win->env->mlx_p, win->win_p, 20, 165,
					0, "- You can zoom this set, controls: ");
	mlx_string_put(win->env->mlx_p, win->win_p, 40, 185,
					0xAC, "SCROLL_UP to zoom in, SCROLL_DOWN - otherwise.");
	mlx_string_put(win->env->mlx_p, win->win_p, 20, 215,
					0, "- You can move this set, controls: ");
	mlx_string_put(win->env->mlx_p, win->win_p, 40, 235,
					0x8C, "ARROW KEYS");
	mlx_string_put(win->env->mlx_p, win->win_p, 20, 355, 0x8C, "ESC");
	mlx_string_put(win->env->mlx_p, win->win_p, 60, 355, 0, "- to close.");
}

void	ft_init_buringship(t_window *win)
{
	win->pivot.x = -0.5;
	win->pivot.y = -0.5;
	win->dx = 0.0017;
	mlx_hook(win->win_p, 2, 0, &ft_key_handler, win);
	mlx_mouse_hook(win->win_p, &ft_mouse_handler, win);
}

void	ft_burningshipfract(t_window *win)
{
	cl_int	err;

	(err = clSetKernelArg(win->cl_data.kernel, 0, sizeof(cl_mem),
		(void *)&win->cl_data.mem)) ? ft_err_handler("OpenCL", "Fail!", 0) : 0;
	(err = clSetKernelArg(win->cl_data.kernel, 1, sizeof(double),
		(void *)&win->pivot.x)) ? ft_err_handler("OpenCL", "Fail!", 0) : 0;
	(err = clSetKernelArg(win->cl_data.kernel, 2, sizeof(double),
		(void *)&win->pivot.y)) ? ft_err_handler("OpenCL", "Fail!", 0) : 0;
	(err = clSetKernelArg(win->cl_data.kernel, 3, sizeof(double),
		(void *)&win->dx)) ? ft_err_handler("OpenCL", "Fail!", 0) : 0;
	(err = clSetKernelArg(win->cl_data.kernel, 4, sizeof(int),
		(void *)&win->height)) ? ft_err_handler("OpenCL", "Fail!", 0) : 0;
	(err = clSetKernelArg(win->cl_data.kernel, 5, sizeof(int),
		(void *)&win->width)) ? ft_err_handler("OpenCL", "Fail!", 0) : 0;
	err = clEnqueueNDRangeKernel(win->env->cl_data.queue, win->cl_data.kernel,
		2, NULL, (size_t[3]){win->width, win->height, 0}, NULL, 0, NULL, NULL);
	err ? ft_err_handler("OpenCL", "Fail!", 0) : 0;
	err = clEnqueueReadBuffer(win->env->cl_data.queue, win->cl_data.mem,
		CL_TRUE, 0, win->height * win->width * sizeof(t_uint), win->pixels.buff,
		0, NULL, NULL);
}
