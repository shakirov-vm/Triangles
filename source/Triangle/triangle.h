#pragma once

#include <vector>

#include "double_tools.h"

struct Vector {
	double x = NAN;
	double y = NAN;
	double z = NAN;

	Vector () {}
	Vector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

	friend Vector operator+(Vector const &first, Vector const &second);
	friend Vector operator-(Vector const &first, Vector const &second);
	friend double scalar_mult(Vector const &first, Vector const &second);
	friend Vector vector_mult(Vector const &first, Vector const &second);

	double lenght();
};
///    scalar mult for points??
struct Point : Vector {
	Point() {}
	Point(double x_, double y_, double z_) : Vector(x_, y_, z_) {}

	friend Point operator+(Point const &first, Point const &second);
	friend Point operator-(Point const &first, Point const &second);
	
};    

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

	void get_x_projection(); // Potential is rudyment
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

	friend bool intersect(Projection& first, Projection& second);
};

void take_triangles(std::vector<Triangle>& triangles, std::string input_file);
