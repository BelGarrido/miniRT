#include "../../include/minirt.h"
#include "../../include/hit_bonus.h"
#include "../../include/shading_bonus.h"
#include "../../include/scene_bonus.h"
#include "../../include/material_bonus.h"
#include <math.h>

//Ispecular​=ks​⋅Ilight​⋅max(0,N⋅H)α
t_vec3 specular_blinn_phong(const t_scene *scene, const t_hit *hit, t_ray r, t_material material)
{
	(void)r;
	//t_vec3 p = hit->p;
	t_vec3 n = v3_norm(hit->n); //
	t_vec3 l = v3_norm(v3_sub(scene->light.pos, hit->p)); //direccion hacia la luz
	t_vec3 v = v3_norm(v3_sub(scene->camera.pos, hit->p)); // direccion hacia la camara
	t_vec3 h = v3_norm(v3_add(l, v));
	//t_vec3 albedo =  material.albedo;
	float ks = material.ks;
	float shininess = material.shininess;


	//ks * (scene->light.pos * (n*h))shinniness;
	//v3_dot(n, h); mide cuanto el vector halfway se alinea con la normal, cuanto mayor es mas fuerte es el reflejo
	//n⋅h=cos(θ)
	//donde θ es el ángulo entre la normal y el vector halfway
	t_vec3 light_intensity = v3_mul(scene->light.color, scene->light.bright);
	float spec_angle = v3_dot(n, h);
	if(spec_angle < 0.0f)
		spec_angle = 0.0f;
	float spec_intensity = powf(spec_angle, shininess);
	printf("ks: %f, shininess: %f\n", ks, shininess);
	printf("spec_angle: %f, spec_intensity: %f\n", spec_angle, spec_intensity);
	t_vec3 spec_color = v3_mul(v3_mul(light_intensity, spec_intensity), ks);
	return (spec_color);


}

t_vec3	shade_lambert_spec(const t_scene *scene, const t_hit *hit, t_ray r)
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
	t_vec3 specular = specular_blinn_phong(scene, hit, r, scene->material);
	//specular Blinn-Phong
	//V = hacia la camara
	//Itotal​=Iambient​+albedo⋅Ilight​⋅max(0,N⋅L)
	//c = v3_add(ambient, v3_ctoc(hit->albedo, diff));
	// H = normalize(L + V) con control de longitud
	//Itotal​=Iambient​+kd​⋅(Ilight​⋅max(0,N⋅L))+ks​⋅(Ilight​⋅max(0,N⋅H)α)
	c = v3_add(ambient,v3_add(v3_ctoc (hit->albedo, diff), specular));
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
