#ifndef APP_H
# define APP_H

#ifndef SCENE_HEADER
# define SCENE_HEADER "scene.h"
#endif
# include SCENE_HEADER
# include "ui.h"

typedef struct s_app
{
	mlx_t			*mlx;
	mlx_image_t		*image;
	uint32_t		*framebuffer;
	int				show_normals;
	t_scene			scene;
	t_toggle_info	overlay;
}	t_app;

void	app_on_key(mlx_key_data_t keydata, void *param);

#endif