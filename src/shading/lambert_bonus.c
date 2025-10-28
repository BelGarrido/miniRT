#include "../../include/minirt.h"
#include "../../include/hit_bonus.h"
#include "../../include/shading_bonus.h"
#include "../../include/scene_bonus.h"
#include "../../include/material_bonus.h"
#include <math.h>



t_vec3	shade_lambert_spec(const t_scene *scene, const t_hit *hit)
{
	t_vec3	ambient;
	t_vec3	l_dir;
	float	ndotl;
	t_vec3	diff;
	t_vec3	c;

	if (!hit->ok)
		return v3(0,0,0);
	ambient = v3_mul(scene->ambient.color, scene->ambient.ratio);
	if (in_shadow(scene, hit->p, scene->light.pos))
		return (v3_ctoc(hit->albedo, ambient));
	l_dir = v3_norm(v3_sub(scene->light.pos, hit->p));
	//how “facing” the surface is toward the light.
	ndotl = v3_dot(hit->n, l_dir);
	//Negative values mean the light is behind the surface → ignore (no light).
	if (ndotl < 0.0f)
		ndotl = 0.0f;
	diff = v3_mul(v3_mul(scene->light.color, scene->light.bright), ndotl);
	//t_vec3 specular = specular_blinn_phong(scene, hit, r, scene->objects->u_obj.cy.material->ks);
	//specular Blinn-Phong
	//V = hacia la camara
	//Itotal​=Iambient​+albedo⋅Ilight​⋅max(0,N⋅L)
	//c = v3_add(ambient, v3_ctoc(hit->albedo, diff));
	// H = normalize(L + V) con control de longitud
	//Itotal​=Iambient​+kd​⋅(Ilight​⋅max(0,N⋅L))+ks​⋅(Ilight​⋅max(0,N⋅H)α)
	c = v3_add(ambient,v3_add(v3_ctoc (hit->albedo, diff), hit->specular));
	return (c);
}


/*
N = normal en el punto P. Es un vector que apunta perpendicularly hacia fuera de la superficie. (Imagina una flecha que sale de la superficie.)
L = dirección desde P hacia la luz. Se calcula como light.pos - P. (Flecha que apunta al foco de luz.)
V = dirección desde P hacia la cámara/observador. Se calcula como camera.pos - P (o -ray.dir si tu rayo está normalizado).
H = half-vector = dirección “media” entre L y V: H=normalize(L+V). (Es una flecha que apunta justo a la mitad de la dirección a la luz y la dirección a la cámara.)

Todas deben estar normalizadas
En iluminación usamos max(0, dot)
*/
