#ifndef VEC3_H
#define VEC3_H

#include<iostream>
#include<cmath>
#include<cassert>
#include<math.h>
using namespace std;

class Vec3 {
public:
	float x, y, z;  //coordinates of a pixel
	Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {  }
	Vec3() : x(0), y(0), z(0){ }
	Vec3 operator * (const float& f) const { return Vec3(x * f, y * f, z * f);}
	float operator * (const Vec3& v) const { return float(x * v.x + y * v.y + z * v.z);	}
	Vec3 operator / (const float& f) const { return Vec3(x / f, y / f, z / f); }
	Vec3 operator - (const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
	Vec3 operator + (const Vec3& v) const {	return Vec3(x + v.x, y + v.y, z + v.z);	}
	bool operator == (const Vec3& v) const { return x == v.x && y == v.y && z == v.z; }
	float operator[](const size_t i) {
		switch (i)
		{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			return float(0.0);
			break;
		}
	}
	ostream& operator << (ostream& out) {
		out << x << " " << y << " " << z << " ";
		return out;
	}
	Vec3& normalize() {
		float normal = 1 / magnitude();
		x *= normal;
		y *= normal;
		z *= normal;
		return *this;
	}
	Vec3 cross(const Vec3& v) const {
		float nx = (y * v.z) - z * (v.y);
		float ny = (z * v.x) - x * (v.z);
		float nz = (x * v.y) - y * (v.x);
		return Vec3(nx, ny, nz);
	}
	float dotproduct(const Vec3& v) const {
		return v.x * x + v.y * y + v.z * z;
	}
	float magnitude() {
		float mag = x * x + y * y + z * z;
		if (mag > 0) {
			return sqrtf(mag);
		}
		return 0.0f;
	}
	float Distance(Vec3& u) {
		float dx = x - u.x;
		float dy = y - u.y;
		float dz = z - u.z;
		return sqrtf(dx * dx + dy * dy + dz * dz);
	}
};

using point3 = Vec3;

class Vec4 {
public:
	float x, y, z, w;  //coordinates of a pixel
	Vec4(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W) {  }
	Vec4() : x(0), y(0), z(0), w(0) { }
	Vec4 operator * (const float& f) const { return Vec4(x * f, y * f, z * f, w * f); }
	float operator * (const Vec4& v) const { return float(x * v.x + y * v.y + z * v.z + w * v.w); }
	Vec4 operator / (const float& f) const { return Vec4(x / f, y / f, z / f, w / f); }
	Vec4 operator - (const Vec4& v) const { return Vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
	Vec4 operator + (const Vec4& v) const { return Vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
	float operator[](const size_t i) {
		switch (i)
		{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		case 3:
			return w;
			break;
		default:
			return float(0.0);
			break;
		}
	}
	ostream& operator << (ostream& out) {
		out << x << " " << y << " " << z << " "<< w<<" ";
		return out;
	}
	Vec4& normalize() {
		float normal = 1 / magnitude();
		x *= normal;
		y *= normal;
		z *= normal;
		w *= normal;
		return *this;
	}
	float dotproduct(const Vec4& v) const {
		return v.x * x + v.y * y + v.z * z + v.w * w;
	}
	float magnitude() {
		float mag = x * x + y * y + z * z + w * w;
		if (mag > 0) {
			return sqrtf(mag);
		}
		return 0.0f;
	}
	float Distance(Vec4& u) {
		float dx = x - u.x;
		float dy = y - u.y;
		float dz = z - u.z;
		float dw = w - u.w;
		return sqrtf(dx * dx + dy * dy + dz * dz + dw * dw);
	}
};

#endif // !1
