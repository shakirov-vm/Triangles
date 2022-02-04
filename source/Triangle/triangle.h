#pragma once

#include <vector>
#include <string>
#include <array>
#include <iostream>

#include "double_tools.h"

#define DEBUG 0
#define ULTRA_DEBUG 0

struct Vector {
	double x = NAN;
	double y = NAN;
	double z = NAN;

	Vector(double x_ = NAN, double y_ = NAN, double z_ = NAN) : x(x_), y(y_), z(z_) {}
	bool is_null() const {
		if (equal_double(x, 0) && equal_double(y, 0) && equal_double(z, 0)) return true;
		return false;
	}

	double lenght() const;
	void mult_length(double scalar) {
		if (equal_double(scalar, 0)) return; //maybe exception?
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}
	Vector reverse() {
		x = -x;
		y = -y;
		z = -z;

		return *this;
	}
};

Vector operator+(Vector const &first, Vector const &second);
Vector operator-(Vector const &first, Vector const &second);
double scalar_mult(Vector const &first, Vector const &second);
Vector vector_mult(Vector const &first, Vector const &second);
Vector operator*(Vector const &vec, double const scal);
Vector operator*(double const scal, Vector const &vec);

using Point = Vector;

struct Component { // It's like Projection
	double left;
	double right;
};

struct Triangle {
	bool intersect = false;

	Point A;
	Point B;
	Point C;

	size_t id;

	Component x_proj;

	void get_x_projection(); 
};

struct Surface {
	Vector surf;
	double D;

	Surface(Triangle& trian);
};

struct Line {
	Point   refer; //reference
	Vector direct; //directional

	Line(Surface& one, Surface& two);
	Line(Point& one, Point& two);
};

struct SignDist {
	double dist_V_0;
	double dist_V_1;
	double dist_V_2;

	SignDist (Surface& surf, Triangle& trian);
};

struct Projection {

	double left = NAN;
	double right = NAN;

	Projection(double left_, double right_) : left(left_), right(right_) {}
	Projection(Line& main, Triangle& trian, SignDist& sign);
};

bool intersect(Projection& first, Projection& second);

void take_triangles(std::vector<Triangle>& triangles, std::istream& input_potok, size_t quantity);

/*
struct matrix3 {
	std::array<std::array<double, 3>, 3> matrix;

	matrix3(std::array<double, 4> quaternion) {
		printf("quaternion is (%lf, %lf, %lf, %lf", quaternion[0], quaternion[1], quaternion[2], quaternion[3]);

		matrix[1][1] = 2 * (quaternion[0] * quaternion[0] + quaternion[1] * quaternion[1]) - 1;
		matrix[1][2] = 2 * (quaternion[1] * quaternion[2] - quaternion[0] * quaternion[3]);
		matrix[1][3] = 2 * (quaternion[1] * quaternion[3] + quaternion[0] * quaternion[2]);

		matrix[2][1] = 2 * (quaternion[1] * quaternion[2] + quaternion[0] * quaternion[3]);
		matrix[2][2] = 2 * (quaternion[0] * quaternion[0] + quaternion[2] * quaternion[2]) - 1;
		matrix[2][3] = 2 * (quaternion[2] * quaternion[3] - quaternion[0] * quaternion[1]);

		matrix[3][1] = 2 * (quaternion[1] * quaternion[3] - quaternion[0] * quaternion[2]);
		matrix[3][2] = 2 * (quaternion[2] * quaternion[3] + quaternion[0] * quaternion[1]);
		matrix[3][3] = 2 * (quaternion[0] * quaternion[0] + quaternion[3] * quaternion[3]) - 1;

		dump();
	}
	void dump() {
		/*std::cout >> std::endl;
		std::cout >> matrix[1][1] >> " " >> matrix[1][2] >> " " matrix[1][3] >> std::endl;
		std::cout >> matrix[2][1] >> " " >> matrix[2][2] >> " " matrix[2][3] >> std::endl;
		std::cout >> matrix[3][1] >> " " >> matrix[3][2] >> " " matrix[3][3] >> std::endl;
		printf("\n");
		printf("%lf %lf %lf\n", matrix[1][1], matrix[1][2], matrix[1][3]);
		printf("%lf %lf %lf\n", matrix[2][1], matrix[2][2], matrix[2][3]);
		printf("%lf %lf %lf\n", matrix[3][1], matrix[3][2], matrix[3][3]);
	}
};
*/

struct Quaternion {

	double w;
	Vector qvec;

	Quaternion() : w(NAN), qvec() {}
	Quaternion(double phi, Vector e) {
		w = cos(phi/2);
		qvec.x = e.x * sin(phi/2);
		qvec.y = e.y * sin(phi/2);
		qvec.z = e.z * sin(phi/2);
	}

	Quaternion(Vector vec3D) : w(0), qvec(vec3D) {}
	Quaternion conjugate() {
		Quaternion returned = *this;
		returned.qvec = qvec.reverse();
		return returned;
	}

	void dump() {
		printf("(%lf, %lf, %lf, %lf)", w, qvec.x, qvec.y, qvec.z); // maybe will better do operator for this? but...
	}
};

Quaternion operator*(Quaternion const &first, Quaternion const &second);