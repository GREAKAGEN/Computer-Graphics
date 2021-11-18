#ifndef LOOKAT
#define LOOKAT

#include"vec3.h"
#include<cmath>
#include<math.h>

#define M_PI 3.141592
#define UP Vec3(0.f,1.f,0.f)
#define RIGHT Vec3(1.f,0.f,0.f)
#define FORWARD Vec3(0.f,0.f,-1.f)

struct LookAt {
	Vec3 origin;
	Vec3 direction;
	LookAt() : origin(Vec3(0.f, 0.f, 0.f)), direction(Vec3(0.f, 0.f, -1.f)) {}

	Vec3 viewscreen(Vec3& node) {  //rotate around axis y, then around axis z
		Vec3 projectionxz = direction - UP * (direction * UP);  //vector projection on plain XZ

		float rotroundy = projectionxz.normalize() * FORWARD;  //cos between z and new direction projection on XZ
		float angley = direction.normalize() * UP;

		Vec3 newnode, xaxis;
		xaxis.x = RIGHT.x * rotroundy + RIGHT.z * sin(acos(rotroundy));
		xaxis.z = RIGHT.x * -sin(acos(rotroundy)) + RIGHT.z * rotroundy;
		xaxis.y = RIGHT.y;

		newnode.x = node.x * rotroundy + node.z * sin(acos(rotroundy));
		newnode.z = node.x * -sin(acos(rotroundy)) + node.z * rotroundy;
		newnode.y = node.y;

		//second rotate to raise up from XZ
		newnode = newnode * sin(acos(angley)) + xaxis.cross(newnode) * angley + xaxis * (xaxis * newnode) * (1 - sin(acos(angley)));

		return newnode;
	}
};


LookAt eye;
//eye.origin = Vec3(0.0f,3.0f,-1.0f);
//eye.direction = Vec3(0.0f,-1.0f,-0.7f);

#endif // ! LOOKAT

