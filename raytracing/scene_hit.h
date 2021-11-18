#ifndef SCENE_HIT
#define SCENE_HIT

#include"vec3.h"
#include"object.h"
#include<limits>

bool SceneHitPal(const vector<Sphere>& spheres, const Tetrahedron& tehd, const Vec3& orig, const Vec3& direct, Vec3& hitpoint, Vec3& Norm, Vec3& color) { //return the intersection point, color, normalization
	float distance = std::numeric_limits<float>::max();

	//detect the spheres
	for (const Sphere& s : spheres) {
		float meet;
		if (s.ray_intersection(orig, direct, meet) && meet < distance) {
			distance = meet;
			hitpoint = orig + direct * distance;
			Norm = (hitpoint - s.centroid).normalize();
			color = s.color.diffuse_color;
		}
	}

	//detect the tetrahedron
	float distanceprem = std::numeric_limits<float>::max();
	Vec3 N;
	if (tetra_ray_intersection(tehd, orig, direct, distanceprem, N) && distanceprem < distance) {
		distance = distanceprem;
		hitpoint = orig + direct * distance;
		Norm = N;
		color = tehd.color.diffuse_color; 
	}

	return distance < 10000;
}

bool SceneHitShade(const vector<Sphere>& spheres, const Tetrahedron& tehd, const Vec3& orig, const Vec3& direct, Vec3& hitpoint, Vec3& Norm, Objectcolor& color) { //return the intersection point, color, normalization
	float distance = std::numeric_limits<float>::max();

	//detect the spheres
	for (const Sphere& s : spheres) {
		float meet;
		if (s.ray_intersection(orig, direct, meet) && meet < distance) {
			distance = meet;
			hitpoint = orig + direct * distance;
			Norm = (hitpoint - s.centroid).normalize();
			color = s.color;
		}
	}

	//detect the tetrahedron
	float distanceprem = numeric_limits<float>::max();
	Vec3 N;
	if (tetra_ray_intersection(tehd, orig, direct, distanceprem, N) && distanceprem < distance) {
		distance = distanceprem;
		hitpoint = orig + direct * distance;
		Norm = N; //cout << N.x << " " << N.y << " " << N.z << " " << endl;
		color = tehd.color;
	}
	
/*	if (distance < 10000) {
		for (const Sphere& sx : spheres) {
			float met;
			if (sx.ray_intersection(hitpoint, directional_light.direction, met) && met > 0.1) {
				color.diffuse_color = sx.color.diffuse_color * 0.1f;
				return distance < 10000;
			}
		}

		float distprem = std::numeric_limits<float>::max();
		Vec3 N0;
		if (tetra_ray_intersection(tehd, hitpoint, directional_light.direction, distprem, N0) && distprem > 0.1) {
			color.diffuse_color = tehd.color.diffuse_color * 0.1f;
			return distance < 10000;
		}
	}*/
	return distance < 10000;
}

#endif // !SCENE_HIT