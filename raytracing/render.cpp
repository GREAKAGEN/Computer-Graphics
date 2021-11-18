#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<limits>
#include<fstream>
#include<cstdio>
#include "GLFW/glfw3.h"
#include "cast_ray.h"
#include "LookAt.h"
//#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
//#include "camera.h"
//#include "H:/GO ABROAD/Semester 2/Computer graphics/RayTracing-master/RayTracing-master/RayTracer/Camera.cpp"
using namespace std;


void RenderPic(const vector<Sphere>& spheres, const Tetrahedron& tehd, vector<DirectRay>& lights, LookAt& eye) {
	const int screenWidth = 1024;
	const int screenHeight = 768;
	const float fov = M_PI / 3.;
	std::vector<Vec3> framebuffer(screenWidth * screenHeight);

	/*int choice;
	cout << "please select a mode: 0--from origin£¨0 0 0£©  1--from another direction:" << endl;
	cin >> choice;*/
	int pers;
	cout << "please select a mode: 0--parallel  1--perspective:" ;
	cin >> pers;


#pragma omp parallel for
	for (size_t j = 0; j < screenHeight; j++) { // actual rendering loop
		for (size_t i = 0; i < screenWidth; i++) {
			float dir_x = (i + 0.5) - (float)screenWidth / 2.f; /// ((float)screenWidth / 2.f);// *eye.dx + eye.origin[0];
			float dir_y = -(j + 0.5) + (float)screenHeight / 2.f; /// ((float)screenHeight / 2.f);// *eye.dy + eye.origin[1];    // this flips the image at the same time
			float dir_z = -(float)screenHeight / (2.f * tan(fov / 2.));// *eye.dz + eye.origin[2];
			/*float dir_z = -(float)screenHeight / (2.f * tan(fov / 2.));
			float dir_x = ((i + 0.5) * 2 / (float)screenWidth * tan(fov / 2) - tan(fov / 2)) * (float)screenWidth / (float)screenHeight;// *eye.dx + eye.origin[0];
			float dir_y = (-2 * (j + 0.5) / (float)screenHeight - 1) * tan(fov / 2);// *eye.dy + eye.origin[1];    // this flips the image at the same time*/
			Vec3 node = Vec3(dir_x, dir_y, dir_z)/(dir_z * -1);
			Vec3 screen = eye.viewscreen(node) + eye.origin;
			//cout << screen.x << " " << screen.y << " " << screen.z << "  ";
			/*
			if (i == screenWidth - 1 && j == screenHeight - 1)
				cout << screen.x << " " << screen.y << " " << screen.z << "  " << node.x << " " << node.y << " " << node.z << endl;
			if (i == 0 && j == 0)
				cout << screen.x << " " << screen.y << " " << screen.z << "  " << node.x << " " << node.y << " " << node.z << endl;
			if (!choice)
				framebuffer[i + j * screenWidth] = Cast_Ray_Parallel(node, FORWARD, spheres, tehd);
			else
				framebuffer[i + j * screenWidth] = Cast_Ray_Parallel(screen, eye.direction, spheres, tehd);*/
			//framebuffer[i + j * screenWidth] = Cast_Ray_Shading(screen, eye.direction, spheres, tehd, lights);
			if(!pers)
				framebuffer[i + j * screenWidth] = Cast_Ray_Shading(screen, eye.direction, spheres, tehd, lights);  //record the color of the pixel
			else
				framebuffer[i + j * screenWidth] = Cast_Ray_Shading(Vec3(0,0,0), Vec3(dir_x,dir_y,dir_z).normalize(), spheres, tehd, lights);  //record the color of the pixel
		}
	}

	std::ofstream ofs; // save the framebuffer to file
	ofs.open("outshade_final.ppm", std::ios::binary);
	ofs << "P6\n" << screenWidth << " " << screenHeight << "\n255\n";
	for (Vec3& c : framebuffer) {
		float max = c[0] > c[1] ? c[0] : c[1];
		max = max > c[2] ? max : c[2];// std::max(c[0], std::max(c[1], c[2]));
		if (max > 1) c = c * (1. / max);
		//std::cout<< (255 * c[0])<<" " << (255 * c[1])<<" " << (255 * c[2])<<" ";
		ofs << (char)(255 * c[0]) << (char)(255 * c[1]) << (char)(255 * c[2]);
	}
	ofs.close();
}

int main() {
	LookAt eye;
	//eye.origin = Vec3(0.0f,3.0f,-1.0f);
	//eye.direction = Vec3(0.0f,-1.0f,-0.7f);
	vector<DirectRay> rays = {
		{{ -20, 20, 20 },1.5},
		{{ 30, 60, -15 },1.9},
		{{ 15, 20, 30 },1.4}
	};

	Tetrahedron tehd;
	Sphere sh1(Vec3(-0.25f, 0.0f, -4.7f), 0.15f);
	Sphere sh2(Vec3(0.0f, 0.05f, -5.0f), 0.12f);

	sh1.color.diffuse_color = Vec3(0.1f, 0.4f, 0.3f);
	sh1.color.specular_exponent = 100.f;
	sh1.color.albedo = Vec4(0.9f, 0.1f, 0.0f, 0.0f);
	sh1.color.refractive_index = 0.2f;// 1.5f;
	sh1.color.difintensity = directional_light.intensity;

	sh2.color.diffuse_color = Vec3(0.7f, 0.1f, 0.4f);
	sh2.color.specular_exponent = 1400.f;
	sh2.color.albedo = Vec4(0.1f, 10.0f, 1.6f, 0.0f);
	sh2.color.refractive_index = 0.1f;// 1.f;
	sh2.color.difintensity = directional_light.intensity;

	tehd.color.diffuse_color = Vec3(0.1f, 0.75f, 0.9f);
	tehd.color.specular_exponent = 10.f;
	tehd.color.albedo = Vec4(0.4f, 0.5f, 0.1f, 0.8f);
	tehd.color.refractive_index = 0.05f;//1.0f;
	tehd.color.difintensity = directional_light.intensity;

	vector<Sphere> shseq;
	shseq.push_back(sh1);
	shseq.push_back(sh2);

	RenderPic(shseq, tehd, rays, eye);

	//make a serious of picture
	bool print = false;
	cout << "Do you need a serious of continuous pictures? 1--YES  0--NO";
	cin >> print;
	if (print) {
		int num = 100, i = 0;
		vector<LookAt> eyesrot;
		//decide a camera rotate circle
		float radius = 4.7f;
		Vec3 circle = Vec3(0.0f, 0.0f, -4.7f);
		while (i < num) {
			//SYSTEMTIME sys;
			//GetLocalTime(&sys);

			float camx = sin(i * 2.f * M_PI / (float)num) * radius;
			float camz = cos(i * 2.f * M_PI / (float)num) * radius - 4.7;
			LookAt eyedow;
			eyedow.origin = Vec3(camx, 0.f, camz);
			eyedow.direction = (circle - eyedow.origin).normalize(); //cout << eyedow.direction[0] << " " << eyedow.direction[1] << " "<<eyedow.direction[2] << " " << endl;
			eyesrot.push_back(eyedow);
			i++;
		}

		const int screenWidth = 1024;
		const int screenHeight = 768;
		const float fov = M_PI / 3.;
		std::vector<Vec3> framebuffer(screenWidth * screenHeight);
		i = 0;

#pragma omp parallel for
		for (LookAt& xeye : eyesrot) {
			for (size_t j = 0; j < screenHeight; j++) { // actual rendering loop
				for (size_t k = 0; k < screenWidth; k++) {
					float dir_x = (k + 0.5) - (float)screenWidth / 2.f; /// ((float)screenWidth / 2.f);// *eye.dx + eye.origin[0];
					float dir_y = -(j + 0.5) + (float)screenHeight / 2.f; /// ((float)screenHeight / 2.f);// *eye.dy + eye.origin[1];    // this flips the image at the same time
					float dir_z = -(float)screenHeight / (2.f * tan(fov / 2.));// *eye.dz + eye.origin[2];
					Vec3 node = Vec3(dir_x, dir_y, dir_z) / (dir_z * -1);
					Vec3 screen = xeye.viewscreen(node) + xeye.origin;
					framebuffer[k + j * screenWidth] = Cast_Ray_Glazing(screen, xeye.direction, shseq, tehd, rays);
				}
			}
			
			char filename[50];
			sprintf_s(filename, "%d.jpg", i);
			std::ofstream ofs(filename, std::ios::binary); // save the framebuffer to file
			//ofs.open("outshade_final.ppm", std::ios::binary);
			ofs << "P6\n" << screenWidth << " " << screenHeight << "\n255\n";
			for (Vec3& c : framebuffer) {
				float max = c[0] > c[1] ? c[0] : c[1];
				max = max > c[2] ? max : c[2];// std::max(c[0], std::max(c[1], c[2]));
				if (max > 1) c = c * (1. / max);
				ofs << (char)(255 * c[0]) << (char)(255 * c[1]) << (char)(255 * c[2]);
			}
			ofs.close();

			i++;
		}
	}

	return 0;
}