#ifndef CAST_RAY
#define CAST_RAY

#include"vec3.h"
#include"object.h"
#include"scene_hit.h"
#include<math.h>
#include<cmath>
using namespace std;

Objectcolor background = {
	{ 0.6f, 0.7f, 0.8f },  //diffuse
	1.f, //intensive
	0.f, //specular
	{ 0.9f, 0.1f, 0.0f, 0.0f }, //albedo
	1.f //refraction
};

Vec3 reflect(const Vec3& direct, const Vec3& norm) {  //refelct derection
	return direct - norm * 2.f * (direct * norm);
}

//render object without light
Vec3 Cast_Ray_Parallel(const Vec3& orig, const Vec3& direct, const vector<Sphere>& spheres, const Tetrahedron& tehd) {  
	Vec3 point, n, color;
	//Objectcolor objcolor;

	if (!SceneHitPal(spheres, tehd, orig, direct, point, n, color)) {
		return Vec3(0.6, 0.7, 0.8);
	}

	return color;
}

//Add lights and shading
Vec3 Cast_Ray_Shading(const Vec3& orig, const Vec3& direct, const vector<Sphere>& spheres, const Tetrahedron& tehd, const vector<DirectRay>& lights, size_t depth = 0) {
	Vec3 point, n , shapo, sn;
	Objectcolor color, color1;

	//background, diffuse + specular + ambient
	bool ifhit = SceneHitShade(spheres, tehd, orig, direct, point, n, color);
	float difang = directional_light.direction * n;
	float spotfading = (directional_light.direction * -1.f + direct) / (directional_light.direction * -1.f + direct).magnitude() * n;  //power p
	if (!ifhit) { 
		Vec3 Ld = background.diffuse_color * background.difintensity * max(0.f, difang); //diffuse color
		Vec3 Ls = background.diffuse_color * background.difintensity * pow(max(0.f, spotfading), background.specular_exponent); //specular color, basing on color white
		Vec3 La = background.diffuse_color * background.ambient * (2 - background.refractive_index);
		return Ld + Ls + La;
	}

	if (SceneHitShade(spheres, tehd, point, directional_light.direction, shapo, sn, color1)) {
		color.diffuse_color = color.diffuse_color * 0.1f;
	}

	Vec3 Ldobj = color.diffuse_color * color.difintensity * (1 - color.refractive_index) * max(0.f, difang); //diffuse color
	Vec3 Lsobj = color.diffuse_color * color.difintensity * pow(max(0.f, spotfading), color.specular_exponent); //specular Vec3(1.f, 1.f, 1.f)
	Vec3 Laobj = color.diffuse_color * color.ambient * (2 - color.refractive_index);
	return Ldobj +Lsobj + Laobj;
}

//Add reflection and a glazing surface
Vec3 Cast_Ray_Glazing(const Vec3& orig, const Vec3& direct, const vector<Sphere>& spheres, const Tetrahedron& tehd, const vector<DirectRay>& lights, size_t depth = 0) {
	Vec3 point, n, shapo, sn;
	Objectcolor color, color1;

	//background, diffuse + specular + ambient
	bool ifhit = SceneHitShade(spheres, tehd, orig, direct, point, n, color);
	float difang = directional_light.direction * n;
	float spotfading = (directional_light.direction * -1.f + direct) / (directional_light.direction * -1.f + direct).magnitude() * n;  //power p

	if (depth > 3 || !ifhit) {
		Vec3 Ld = background.diffuse_color * background.difintensity * max(0.f, difang); //diffuse color
		Vec3 Ls = background.diffuse_color * background.difintensity * pow(max(0.f, spotfading), background.specular_exponent); //specular color, basing on color white
		Vec3 La = background.diffuse_color * background.ambient * (2 - background.refractive_index); //ambient color
		return Ld + Ls + La;
	}

	Vec3 reflect_direct = reflect(direct, n).normalize();// *-1;
	Vec3 reflect_color = Cast_Ray_Glazing(point, reflect_direct, spheres, tehd, lights, depth + 1);

	/*if (SceneHitShade(spheres, tehd, point, directional_light.direction, shapo, sn, color1)) {
		//color.diffuse_color = color.diffuse_color * 0.1f;
		return Vec3(1.f,1.f,1.f);
	}*/
	Vec3 Ldobj = color.diffuse_color * color.difintensity * (1 - color.refractive_index) * max(0.f, difang); //diffuse color
	Vec3 Lsobj = Vec3(1.f, 1.f, 1.f) * color.difintensity * pow(max(0.f, spotfading), color.specular_exponent); //specular Vec3(1.f, 1.f, 1.f)
	Vec3 Laobj = color.diffuse_color * color.ambient * (2 - color.refractive_index);
	return Ldobj * color.albedo[0] + Lsobj * color.albedo[1] + reflect_color * color.albedo[2] + Laobj;
}

#endif // !CAST_RAY

