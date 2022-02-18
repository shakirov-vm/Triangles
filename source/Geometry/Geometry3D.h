#pragma once

#include <vector>
#include <string>
#include <array>
#include <iostream>

#include "../Tools/Tools.h"

namespace Triangles {

struct Vector {
	double x = NAN;
	double y = NAN;
	double z = NAN;

	Vector(double x_ = NAN, double y_ = NAN, double z_ = NAN) : x(x_), y(y_), z(z_) {}
	
	bool is_null() const {
		if (equal_double(x, 0) && equal_double(y, 0) && equal_double(z, 0)) return true;
		return false;
	}
	double lenght() const {
	    return std::sqrt(x * x + y * y + z * z);
	}
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

bool operator==(Vector const &first, Vector const &second);
Vector operator+(Vector const &first, Vector const &second);
Vector operator-(Vector const &first, Vector const &second);
double scalar_mult(Vector const &first, Vector const &second);
Vector vector_mult(Vector const &first, Vector const &second);
Vector operator*(Vector const &vec, double const scal);
Vector operator*(double const scal, Vector const &vec);

using Point = Vector;

struct Quaternion {

	double w;
	Vector qvec;

	Quaternion() : w(NAN), qvec() {}
	Quaternion(double const phi, Vector const &e) {
		w = cos(phi/2);
		qvec.x = e.x * sin(phi/2);
		qvec.y = e.y * sin(phi/2);
		qvec.z = e.z * sin(phi/2);
	}
	Quaternion(Quaternion const &old) : w(old.w), qvec(old.qvec) {} 
	Quaternion(Vector const &vec3D) : w(0), qvec(vec3D) {}

	
	Quaternion conjugate() const { // const, because we don't want conjigate our quaternion, we want copy

		Quaternion returned(*this); // we can do =
		returned.qvec = returned.qvec.reverse();
		return returned;
	}
	void dump() const {
	    std::cout << "(" << w << ", " << qvec.x << ", " << qvec.y << ", " << qvec.z << ")"; // Is it better?
		
	    //printf("(%lf, %lf, %lf, %lf)", w, qvec.x, qvec.y, qvec.z); // maybe will better do operator for this? but...
	}
};

Quaternion operator*(Quaternion const &first, Quaternion const &second);

struct Component { // It's like Projection
	double left;
	double right;
};

struct Triangle {
	bool intersect;

	Point A;
	Point B;
	Point C;

	size_t id;

	Component x_proj;

	Triangle() = default;
	Triangle(Point const &A_old, Point const &B_old, Point const &C_old) : intersect(false), A(A_old), B(B_old), C(C_old), id(0xDEADBEEF) {}

	void get_x_projection() {
	    x_proj.left = std::min(std::min(A.x, B.x), C.x); 
	    x_proj.right = std::max(std::max(A.x, B.x), C.x); 
	}
	bool in_triangle(Point const &P) const;
	void dump() const;
};

struct Surface {
	Vector normal;
	double D;

	Surface(Triangle const &trian);
};

struct Line {
	Point   refer; //reference
	Vector direct; //directional

	Line(Surface const &one, Surface const &two);
	Line(Point const &one, Point const &two);
};

struct Segment {
	Point P1;
	Point P2;

	Segment(Triangle const &trian);
	Segment(Point const &first, Point const &second) : P1(first), P2(second) {}

	bool in_segment(Point const &P);
};

struct SignDist {
	double dist_V_0;
	double dist_V_1;
	double dist_V_2;

	SignDist (Surface const &surf, Triangle const &trian);
	void SignDist::dump() const {
	    std::cout << "SignDist: " << dist_V_0 << ", " << dist_V_1 << ", " << dist_V_2 << std::endl;
	}
};

struct Projection {

	double left = NAN;
	double right = NAN;

	Projection(double left_, double right_) : left(left_), right(right_) {}
	Projection(Line const &main, Triangle& trian, SignDist& sign);
};

bool intersect(Projection const &first, Projection const &second);
void reverse_normal(Surface& surf);
bool is_point(Triangle const &trian);
bool is_segment(Triangle const &trian);
bool intersect_segments(Segment const &first, Segment const &second);
bool compare_proj(Triangle const &first, Triangle const &second);

Vector cut_quat_to_vec(Quaternion quat);

}