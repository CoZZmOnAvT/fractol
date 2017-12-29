/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia_fract.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/29 16:45:05 by pgritsen          #+#    #+#             */
/*   Updated: 2017/12/29 17:40:08 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	ft_init_julia(t_window *win)
{
	win->pivot.x = 0.0;
	win->pivot.y = 0.0;
	win->dx = 1.0;
	mlx_hook(win->win_p, 2, 0, &ft_key_handler, win);
	mlx_mouse_hook(win->win_p, &ft_mouse_jf_handler, win);
}

void	ft_julia(t_window *win)
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