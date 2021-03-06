/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/18 22:04:33 by pgritsen          #+#    #+#             */
/*   Updated: 2018/01/02 21:21:32 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <stdio.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <mlx.h>
# include <math.h>
# include <time.h>
# include <limits.h>
# include <pthread.h>
# include <OpenCL/opencl.h>
# include "libft.h"

# define PROGRAM_NAME "Fracto'l"
# define HELP "Help"
# define MANDELFRACT "Mandelbrot Set"
# define BURNINGSHIP "BurningShip"
# define JULIAFRACT "Julia Set"
# define TRICORNFRACT "Tricorn Set"
# define FERNFRACT "Barnsley fern fractal"
# define PYTHTREEFRACT "Pythagoras tree fractal"
# define FRACTTREE "Fractal tree"
# define SIERPINSKI "Sierpinski triangle fractal"

# define K_MF "./kernel/mandelfract.cl"
# define F_MF "fill_mandelfract"

# define K_BS "./kernel/burningship.cl"
# define F_BS "fill_burningship"

# define K_JF "./kernel/julia.cl"
# define F_JF "fill_julia"

# define K_TF "./kernel/tricorn.cl"
# define F_TF "fill_tricorn"

# define MAX_SOURCE_SIZE 0x400000

# define HW_WIDTH 600
# define HW_HEIGHT 400

# define MW_WIDTH 800
# define MW_HEIGHT 600

# define FW_WIDTH 2560
# define FW_HEIGHT 1440

# ifndef ONE_OVER_LOG2
#  define ONE_OVER_LOG2 1.0 / log(2.0)
# endif

# define KEY_PLUS 69
# define KEY_MINUS 78
# define KEY_TAB 48
# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_Q 12
# define KEY_E 14
# define KEY_ARR_UP 126
# define KEY_ARR_DOWN 125
# define KEY_ARR_LEFT 123
# define KEY_ARR_RIGHT 124
# define M_B_LEFT 1
# define M_B_RIGHT 2
# define M_SCROLL_UP 4
# define M_SCROLL_DOWN 5
# define KEY_ESC 53

typedef unsigned char	t_uchar;
typedef unsigned int	t_uint;
typedef unsigned long	t_ulong;
typedef unsigned short	t_ushort;

struct s_rot;
struct s_vertice;
struct s_poly;
struct s_object;
struct s_window;
struct s_cam;
struct s_env;
struct s_img;

typedef struct	s_cl
{
	cl_device_id		device;
	cl_context			context;
	cl_command_queue	queue;
	cl_program			program;
	cl_kernel			kernel;
	cl_mem				mem;
}				t_cl;

typedef struct	s_rot
{
	double	rx;
	double	ry;
	double	rz;
}				t_rot;

typedef struct	s_dpndc
{
	char	*key;
	void	(*func)(struct s_window *);
	char	*func_name;
	char	*kernel_name;
}				t_dpndc;

typedef struct	s_vertice
{
	struct s_vertice	*prev;
	double				x;
	double				y;
	double				z;
	long				color;
	struct s_vertice	*next;
}				t_vertice;

typedef struct	s_poly
{
	struct s_poly	*prev;
	t_vertice		*vertices;
	int				id;
	struct s_poly	*next;
}				t_poly;

typedef struct	s_object
{
	struct s_object	*prev;
	t_poly			*polygons;
	int				id;
	intmax_t		poly_c;
	struct s_object	*next;
}				t_object;

typedef struct	s_img
{
	int			s_l;
	int			endian;
	int			bpp;
	void		*p;
	char		*buff;
}				t_img;

typedef struct	s_window
{
	struct s_window	*prev;
	int				width;
	int				height;
	char			*title;
	void			*win_p;
	t_img			pixels;
	double			dx;
	t_vertice		c;
	t_vertice		pivot;
	struct s_env	*env;
	t_cl			cl_data;
	char			safe_m;
	char			intra_m;
	struct s_window	*next;
}				t_window;

typedef struct	s_cam
{
	t_vertice		align;
	t_rot			rotation;
	int				id;
	struct s_cam	*next;
}				t_cam;

typedef struct	s_env
{
	t_cam		*cams;
	t_object	*objs;
	void		*mlx_p;
	t_window	*wins;
	t_dpndc		*dpndc;
	t_dpndc		*init_table;
	t_dpndc		*help_table;
	t_cl		cl_data;
}				t_env;

typedef struct	s_thread_stuff
{
	t_window	*win;
	int			lim[4];
}				t_thread_stuff;

/*
**		Handlers.c
**		↓↓↓↓↓↓↓↓↓↓
*/

void			ft_err_handler(char *msg, char *add, int err);

int				ft_key_handler(int key, t_window *win);

int				ft_menu_mouse_handler(int key, int x, int y, t_window *win);

int				ft_mouse_handler(int key, int x, int y, t_window *win);

int				ft_mouse_jf_handler(int key, int x, int y, t_window *win);

/*
**		Handlers_2.c
**		↓↓↓↓↓↓↓↓↓↓
*/

int				ft_mouse_move_handler(int x, int y, t_window *win);

t_window		*ft_button_actions(int key, int x, int y, t_window *win);

int				ft_key_geom_handler(int key, t_window *win);

/*
**		Window_managing.c
**		↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

t_window		*ft_new_win(t_env *env, int width, int height, char *title);

t_window		*ft_get_win(t_window *wins, const char *title);

int				ft_destroy_win(t_window *win);

/*
**		Draw.c
**		↓↓↓↓↓↓
*/

void			ft_set_background(t_window *win, t_uint color);

void			ft_draw_line(t_window *win, t_vertice p0,
								t_vertice p1, int color);

int				ft_draw(t_env env, t_window *win);

/*
**		Color_helper.c
**		↓↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

t_uchar			ft_get_alpha(intmax_t c);

t_uchar			ft_get_red(intmax_t c);

t_uchar			ft_get_green(intmax_t c);

t_uchar			ft_get_blue(intmax_t c);

intmax_t		ft_g_color(intmax_t c1, intmax_t c2, double k);

/*
**		Menu.c
**		↓↓↓↓↓↓
*/

void			ft_menu(t_window *win);

/*
**		Mandelbrot_fract.c
**		↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

void			ft_mandelhelp(t_window *win);

void			ft_init_mandelfract(t_window *win);

void			ft_mandelfract(t_window *win);

/*
**		Burningship_fract.c
**		↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

void			ft_burninghelp(t_window *win);

void			ft_init_buringship(t_window *win);

void			ft_burningshipfract(t_window *win);

/*
**		Julia_fract.c
**		↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

void			ft_juliahelp(t_window *win);

void			ft_init_julia(t_window *win);

void			ft_julia(t_window *win);

/*
**		Tricorn_fract.c
**		↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

void			ft_tricornhelp(t_window *win);

void			ft_init_tricornfract(t_window *win);

void			ft_tricornfract(t_window *win);

/*
**		BernsleyFern_fract.c
**		↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

void			ft_fernhelp(t_window *win);

void			ft_init_fernfract(t_window *win);

void			ft_fernfract(t_window *win);

/*
**		PythagorasTree_fract.c
**		↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

void			ft_pythtreehelp(t_window *win);

void			ft_init_pythtree(t_window *win);

void			ft_pythtreefract(t_window *win);

/*
**		Fractal_tree.c
**		↓↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

void			ft_fracttreehelp(t_window *win);

void			ft_init_fracttree(t_window *win);

void			ft_fracttree(t_window *win);

/*
**		Sierpinski_fract.c
**		↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

void			ft_sierpinskihelp(t_window *win);

void			ft_init_sierpinski(t_window *win);

void			ft_sierpinski(t_window *win);

/*
**		Color_helper.c
**		↓↓↓↓↓↓↓↓↓↓↓↓↓↓
*/

t_uchar			ft_get_alpha(intmax_t c);

t_uchar			ft_get_red(intmax_t c);

t_uchar			ft_get_green(intmax_t c);

t_uchar			ft_get_blue(intmax_t c);

intmax_t		ft_g_color(intmax_t c1, intmax_t c2, double k);

/*
**		OpenCl.c
**		↓↓↓↓↓↓↓↓
*/

void			ft_init_cl(t_env *env, cl_device_type dev_type);

void			ft_parse_kernel(t_env *env, t_window *win,
						const char *kernel_name, const char *func_name);

/*
**		Usage.c
**		↓↓↓↓↓↓↓
*/

double			ft_lerp(double a, double b, double t);

void			ft_open_help(t_window *win);

void			ft_make_help_dependecies(t_env *env);

void			ft_make_draw_dependencies(t_env *env);

void			ft_make_environment(t_env *env);

#endif
