#ifndef SHADING_BONUS_H
# define SHADING_BONUS_H

# include "hit_bonus.h"

t_vec3	shade_lambert_spec(const t_scene *scene, const t_hit *hit, t_ray r);
int		in_shadow(const t_scene *scene, t_vec3 p, t_vec3 l_pos);
t_vec3	shade_phong(const t_scene *scene, const t_hit *hit);

#endif