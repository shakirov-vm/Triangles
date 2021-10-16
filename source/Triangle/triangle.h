#pragma once
#include <vector>
#include <math.h>

bool equal_double(double first, double second);

#define eps 0.0000001

struct Point {
	double x = NAN;
	double y = NAN;
	double z = NAN;

	Point() {}
	Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {} // i hope it true;

	//void print();
	bool is_valid();

	friend Point operator+(Point& first, Point& second);
	friend Point operator-(Point& first, Point& second);
	
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

	bool intersection = false;

	void get_x_projection(); // Potential is rudyment
	
};



struct Surface {
	double A_surf;
	double B_surf;
	double C_surf;
	double D_surf;

	Surface(Triangle& trian) { // ?
		// first - A, second - B, third - C
		A_surf = trian.A.y * (trian.B.z - trian.C.z) + trian.B.y * (trian.C.z - trian.A.z) + trian.C.y * (trian.A.z - trian.B.z);
		B_surf = trian.A.x * (trian.C.z - trian.B.z) + trian.B.x * (trian.A.z - trian.C.z) + trian.C.x * (trian.B.z - trian.A.z);
		C_surf = trian.A.x * (trian.B.y - trian.C.y) + trian.B.x * (trian.C.y - trian.A.y) + trian.C.x * (trian.A.y - trian.B.y);

		D_surf = - A_surf * trian.A.x - B_surf * trian.A.y - C_surf * trian.A.z; 

		double normalize = 1 / sqrt(A_surf * A_surf + B_surf * B_surf + C_surf * C_surf);

		A_surf *= normalize;
		B_surf *= normalize;
		C_surf *= normalize;
		D_surf *= normalize;
	}
};

struct Line {
	Point starting;

	double x_proj;
	double y_proj;
	double z_proj;

	Line(Surface& one, Surface& two) {
	//guide or direct
		x_proj = one.B_surf * two.C_surf - one.C_surf * two.B_surf;
		y_proj = one.C_surf * two.A_surf - one.A_surf * two.C_surf;
		z_proj = one.A_surf * two.B_surf - one.B_surf * two.A_surf;
//s is D
		double scalar_n_n = one.A_surf * two.A_surf + one.B_surf * two.B_surf + one.C_surf * two.C_surf;
		double lenght_1 = sqrt(one.A_surf * one.A_surf + one.B_surf * one.B_surf + one.C_surf * one.C_surf);
		double lenght_2 = sqrt(two.A_surf * two.A_surf + two.B_surf * two.B_surf + two.C_surf * two.C_surf);
// SIGN OF D??
		double a = (two.D_surf * scalar_n_n - one.D_surf * lenght_2 * lenght_2) / (scalar_n_n * scalar_n_n - lenght_2 * lenght_2 * lenght_1 * lenght_1);
		double b = (one.D_surf * scalar_n_n - two.D_surf * lenght_1 * lenght_1) / (scalar_n_n * scalar_n_n - lenght_2 * lenght_2 * lenght_1 * lenght_1);
	
		starting.x = a * one.A_surf + b * two.A_surf;
		starting.y = a * one.B_surf + b * two.B_surf;
		starting.z = a * one.C_surf + b * two.C_surf;
	}

	Line(Point& one, Point& two) {
		starting.x = one.x;
		starting.y = one.y;
		starting.z = one.z;

		x_proj = two.x - one.x;
		y_proj = two.y - one.y;
		z_proj = two.z - one.z;
	}
};



struct SignDist {
	double dist_V_0;
	double dist_V_1;
	double dist_V_2;

	SignDist (Surface& surf, Triangle& trian) { // Very big QUESTION!! // Doing more smart
		dist_V_0 = trian.A.x * surf.A_surf + trian.A.y * surf.B_surf + trian.A.z * surf.C_surf + surf.D_surf;
		dist_V_1 = trian.B.x * surf.A_surf + trian.B.y * surf.B_surf + trian.B.z * surf.C_surf + surf.D_surf;
		dist_V_2 = trian.C.x * surf.A_surf + trian.C.y * surf.B_surf + trian.C.z * surf.C_surf + surf.D_surf;
	}
};

struct Projection {

	double left = NAN;
	double right = NAN;

	Projection(double left_, double right_) : left(left_), right(right_) {}

	Projection(Line& main, Triangle& trian, SignDist& sign) {
		//C is reverse_side !! and we need recalculate signed distance!!
		if ((sign.dist_V_0 * sign.dist_V_1) < 0) { // CHECK THIS INE  MORE TIME!!
			if (sign.dist_V_2 < 0) {
				if (sign.dist_V_0 > 0) {
					std::swap(sign.dist_V_0, sign.dist_V_2);
					std::swap(trian.A, trian.C);
				}
				else {
					std::swap(sign.dist_V_1, sign.dist_V_2);
					std::swap(trian.B, trian.C);
				}
			}
			else {
				if (sign.dist_V_0 < 0) {
					std::swap(sign.dist_V_0, sign.dist_V_2);
					std::swap(trian.A, trian.C);
				}
				else {
					std::swap(sign.dist_V_1, sign.dist_V_2);
					std::swap(trian.B, trian.C);
				}
			}

			//std::swap(const_cast<double&>(std::min(sign.dist_V_0, sign.dist_V_1)), sign.dist_V_2); // ??????????????????????????????????????????????
		}

		double scalar_0 = main.x_proj * (trian.A.x - main.starting.x) + main.y_proj * (trian.A.y - main.starting.y) + main.z_proj * (trian.A.z - main.starting.z);
		double scalar_1 = main.x_proj * (trian.B.x - main.starting.x) + main.y_proj * (trian.B.y - main.starting.y) + main.z_proj * (trian.B.z - main.starting.z);
		double scalar_2 = main.x_proj * (trian.C.x - main.starting.x) + main.y_proj * (trian.C.y - main.starting.y) + main.z_proj * (trian.C.z - main.starting.z);

		left = scalar_0 + (scalar_2 - scalar_0) * sign.dist_V_0 / (sign.dist_V_0 - sign.dist_V_2);
		right = scalar_1 + (scalar_2 - scalar_1) * sign.dist_V_1 / (sign.dist_V_1 - sign.dist_V_2);

		if (left > right) std::swap(left, right);
	}

	friend bool intersect(Projection& first, Projection& second);
};

void take_triangles(std::vector<Triangle>& triangles, std::string input_file);
bool intersect(Projection& first, Projection& second);



