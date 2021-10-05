#pragma once
#include <vector>
#include <math.h>


struct Point {
	double x = NAN;
	double y = NAN;
	double z = NAN;

	Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {} // i hope it true;

	//void print();
	//bool is_valid();
};    

struct Projection {
	double left;
	double right;
};

struct Triangle {
	Point A;
	Point B;
	Point C;

	size_t id;

	Projection x_proj;

	bool intersection = false;
	void get_x_projection(); // Potential is rudyment
	
};

void take_triangles(std::vector<Triangle>& triangles, std::string input_file);

