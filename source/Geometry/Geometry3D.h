#pragma once

#include <vector>
#include <string>
#include <array>
#include <iostream>

#include "../Tools/Tools.h"

struct Vector {
	double x = NAN;
	double y = NAN;
	double z = NAN;

	Vector(double x_ = NAN, double y_ = NAN, double z_ = NAN) : x(x_), y(y_), z(z_) {}
	
	bool is_null() const;
	double lenght() const;
	void mult_length(double scalar);
	Vector reverse();
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
	Quaternion(double const phi, Vector const &e);
	Quaternion(Quaternion const &old) : w(old.w), qvec(old.qvec) {} 
	Quaternion(Vector const &vec3D) : w(0), qvec(vec3D) {}

	Quaternion conjugate() const;
	void dump() const;
};

Quaternion operator*(Quaternion const &first, Quaternion const &second);

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

	Triangle() = default;
	Triangle(Point const &A_old, Point const &B_old, Point const &C_old) : intersect(false), A(A_old), B(B_old), C(C_old), id(0xDEADBEEF) {}

	void get_x_projection(); 
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

	Line(Surface& one, Surface& two);
	Line(Point& one, Point& two);
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

	SignDist (Surface& surf, Triangle& trian);
	void dump() const;
};

struct Projection {

	double left = NAN;
	double right = NAN;

	Projection(double left_, double right_) : left(left_), right(right_) {}
	Projection(Line& main, Triangle& trian, SignDist& sign);
};

bool intersect(Projection const &first, Projection const &second);
void reverse_normal(Surface& surf);
bool is_point(Triangle const &trian);
bool is_segment(Triangle const &trian);
bool intersect_segments(Segment const &first, Segment const &second);
bool compare_proj(Triangle const &first, Triangle const &second);

Vector cut_quat_to_vec(Quaternion quat);