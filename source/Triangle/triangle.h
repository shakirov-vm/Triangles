#pragma once

#include <vector>
#include <string>

#include "double_tools.h"

#define DEBUG 1
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
	void extend(double scalar) {
		if (equal_double(scalar, 0)) return; //maybe exception?
		x /= scalar;
		y /= scalar;
		z /= scalar;
	}
	void left_matrix_mult(struct matrix3& matrix);
};

Vector operator+(Vector const &first, Vector const &second);
Vector operator-(Vector const &first, Vector const &second);
double scalar_mult(Vector const &first, Vector const &second);
Vector vector_mult(Vector const &first, Vector const &second);

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

struct matrix3 {
	std::array<std::array<double, 3>, 3> matrix;

	matrix3(std::array<double, 4> quaternion) {
		matrix[1][1] = 2 * (quaternion[0] * quaternion[0] + quaternion[1] * quaternion[1]) - 1;
		matrix[1][2] = 2 * (quaternion[1] * quaternion[2] - quaternion[0] * quaternion[3]);
		matrix[1][3] = 2 * (quaternion[1] * quaternion[3] + quaternion[0] * quaternion[2]);

		matrix[2][1] = 2 * (quaternion[1] * quaternion[2] + quaternion[0] * quaternion[3]);
		matrix[2][2] = 2 * (quaternion[0] * quaternion[0] + quaternion[2] * quaternion[2]) - 1;
		matrix[2][3] = 2 * (quaternion[2] * quaternion[3] - quaternion[0] * quaternion[1]);

		matrix[3][1] = 2 * (quaternion[1] * quaternion[3] - quaternion[0] * quaternion[2]);
		matrix[3][2] = 2 * (quaternion[2] * quaternion[3] + quaternion[0] * quaternion[1]);
		matrix[3][3] = 2 * (quaternion[0] * quaternion[0] + quaternion[3] * quaternion[3]) - 1;
	}
};
