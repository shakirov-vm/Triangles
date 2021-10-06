#pragma once
#include <vector>
#include <math.h>


struct Point {
	double x = NAN;
	double y = NAN;
	double z = NAN;

	Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {} // i hope it true;
	Point(Line& main, Line& segment) {
		printf("Dont to this ctor\n");
	}
	Point(Line& main, Point& mother) {
		x = main.x_proj * (mother.x - main.starting.x);
		y = main.y_proj * (mother.y - main.starting.y);
		z = main.z_proj * (mother.z - main.starting.z);
	}
	Point(Point& i_dash, Point& two_dash, double i_dist, double two_dist) {
		x = i_dash.x + (two_dash.x - i_dash.x) * i_dist / (i_dist - two_dist);
		y = i_dash.y + (two_dash.y - i_dash.y) * i_dist / (i_dist - two_dist);	
		z = i_dash.z + (two_dash.z - i_dash.z) * i_dist / (i_dist - two_dist);
	}
	//void print();
	bool is_valid();
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

