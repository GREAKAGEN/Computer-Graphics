#ifndef OBJECT
#define OBJECT
#include<iostream>
#include"vec3.h"
#include<math.h>
#include<cmath>
#include<vector>


struct DirectRay {
	Vec3 direction;
	float intensity = 1.f;
};

DirectRay directional_light = {
	Vec3(-1.f, 1.f, 1.f).normalize(),
	2.4f
};

vector<DirectRay> rays = {
	{{ -20, 20, 20 },1.5},
	{{ 30, 60, -15 },1.9},
	{{ 15, 20, 30 },1.4}
};

struct Objectcolor {
	Vec3 diffuse_color = { 0, 0, 0 };  //diffusion coefficient
	float difintensity = 1.0f;   //light intensity
	float specular_exponent = 0;  //specular coefficient
	Vec4 albedo = { 1, 0, 0, 0 };   //albedo
	float refractive_index = 1;   //refraction
	float ambient = 0.1f;  //ambiant coefficient
};

struct Sphere {
	Vec3 centroid;
	float radius;
	Objectcolor color;

	Sphere(const Vec3& c, const float& r) :centroid(c), radius(r) {}
	bool ray_intersection(const Vec3& orig, const Vec3& direct, float& t0) const {
		Vec3 L = centroid - orig;
		float NL = L * direct;
		float isdoton = L * L - NL * NL;   //isdonton -- distance square between centroid and the eye ray line
		if (isdoton > radius * radius) {
			return false;
		}
		//cout << isdoton << " ";
		float hiccup = sqrtf(radius * radius - isdoton);
		t0 = NL - hiccup;  //distance between hit point on surface and center of eye line ray in sphere;
		float t1 = NL + hiccup;    //t1 is another spot
		if (t0 < 0) t0 = t1;   //contain the orig, set t1 to be the first spot
		if (t0 < 0) return false;    //no intersection, return false
		return true;
	}
};

struct Tetrahedron
{
	Objectcolor color;
	Vec3 A = Vec3(0.2f, -0.1f, -5.0f);
	Vec3 B = Vec3(0.5f, -0.15f, -5.0f);
	Vec3 C = Vec3(0.35f, -0.2f, -4.6f);
	Vec3 D = Vec3(0.4f, 0.3f, -4.8f);

	Vec3 AB = (B - A).normalize();
	Vec3 BC = (C - B).normalize();
	Vec3 AC = (C - A).normalize();
	Vec3 AD = (D - A).normalize();
	Vec3 BD = (D - B).normalize();
	Vec3 CD = (D - C).normalize();

	Vec3 ABC = AB.cross(BC).normalize();
	Vec3 ABD = AB.cross(AD * -1.f).normalize();
	Vec3 ACD = AC.cross(AD).normalize();
	Vec3 BCD = BC.cross(BD * -1.f).normalize();
};

bool tetra_ray_intersection(const Tetrahedron& T, const Vec3& orig, const Vec3& direct, float& ts, Vec3& N) {
	float flag[4];//t = n*(p1 - p0) / n*u;
	Vec3 nm;
	flag[0] = T.ABC * (T.A - orig) / (T.ABC * direct);
	flag[1] = T.ABD * (T.A - orig) / (T.ABD * direct);
	flag[2] = T.ACD * (T.A - orig) / (T.ACD * direct);
	flag[3] = T.BCD * (T.B - orig) / (T.BCD * direct);

	for (int i = 0; i < 4; i++) {
		//cout << flag[i] << " ";
		if (flag[i] >= 0) {
			Vec3 x = direct * flag[i] + orig;
			bool mark = false;
			switch (i)
			{
			case 0:
				if ((T.AB.cross(x - T.A)) * T.ABC > 0 && (T.BC.cross(x - T.B)) * T.ABC > 0 && ((T.AC * -1.f).cross(x - T.C)) * T.ABC > 0) {
					nm = T.ABC;
					mark = true;
				}
				break;
			case 1:
				if ((T.AD.cross(x - T.A)) * T.ABD > 0 && ((T.BD * -1.f).cross(x - T.D)) * T.ABD > 0 && ((T.AB * -1.f).cross(x - T.B)) * T.ABD > 0) {
					mark = true;
					nm = T.ABD;
				}
				break;
			case 2:
				if ((T.AC.cross(x - T.A)) * T.ACD > 0 && (T.CD.cross(x - T.C)) * T.ACD > 0 && ((T.AD * -1.f).cross(x - T.D)) * T.ACD > 0) {
					mark = true;
					nm = T.ACD;
				}
				break;
			case 3:
				if ((T.BD.cross(x - T.B)) * T.BCD > 0 && ((T.CD * -1.f).cross(x - T.D)) * T.BCD > 0 && ((T.BC * -1.f).cross(x - T.C)) * T.BCD > 0) {
					mark = true;
					nm = T.BCD;
				}
				break;
			default:
				break;
			}
			if (mark == true) {
				if (ts < 0) {
					ts = flag[i]; N = nm; //cout << nm.x << " " << nm.y << " " << nm.z << " ";
				}
				else if (ts > flag[i]) {
					ts = flag[i]; N = nm; //cout << nm.x << " " << nm.y << " " << nm.z << " ";
				}
				else {

				}
			}
		}
	}

	if (ts < 0) return false;
	return true;
}

Tetrahedron tehd;
Sphere sh1(Vec3(-0.25f, 0.0f, -4.7f), 0.15f);
Sphere sh2(Vec3(0.0f, 0.05f, -5.0f), 0.12f);

#endif // !OBJECT

