#pragma once
#include <vector>
#include <math.h>


struct Point {
	double x = NAN;
	double y = NAN;
	double z = NAN;

	Point() {}
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
	Point(Point& const old, Line& const main) {
		x = main.y_proj * (old.z - main.starting.z) - main.z_proj * (old.y - main.starting.y);
		y = main.z_proj * (old.x - main.starting.x) - main.x_proj * (old.z - main.starting.z);
		z = main.x_proj * (old.y - main.starting.y) - main.y_proj * (old.x - main.starting.x);
	}
	//void print();
	bool is_valid();

	friend Point operator+(Point& first, Point& second);
	friend Point operator+(Point& first, Point& second);
	
};    

// maybe ref?
Point operator+(Point& first, Point& second) {
	return Point(first.x + second.x, first.y + second.y, first.z + second.z);
}
Point operator-(Point& first, Point& second) {
	return Point(first.x - second.x, first.y - second.y, first.z - second.z);
}


struct Projection {
	//double??
	double left = NAN;
	double right = NAN;

	Projection(double left_, double right_) : left(left_), right(right_) {}
	Projection(Triangle& trian, Line& const main, SignDist& dist) {
		if ((dist.dist_V_0 * dist.dist_V_1) < 0) { // CHECK THIS INE  MORE TIME!!
			if (dist.dist_V_2 < 0) {
				if (dist.dist_V_0 > 0) {
					std::swap(dist.dist_V_0, dist.dist_V_2);
					std::swap(trian.A, trian.C);
				}
				else {
					std::swap(dist.dist_V_1, dist.dist_V_2);
					std::swap(trian.B, trian.C);
				}
			}
			else {
				if (dist.dist_V_0 < 0) {
					std::swap(dist.dist_V_0, dist.dist_V_2);
					std::swap(trian.A, trian.C);
				}
				else {
					std::swap(dist.dist_V_1, dist.dist_V_2);
					std::swap(trian.B, trian.C);
				}
			}

			//std::swap(const_cast<double&>(std::min(dist.dist_V_0, dist.dist_V_1)), dist.dist_V_2); // ??????????????????????????????????????????????
		}

		Point V_0_hatch(trian.A, main);
		Point V_1_hatch(trian.B, main);
		Point V_2_hatch(trian.C, main);

		left = V_0_hatch + (V_2_hatch - V_0_hatch) * dist.dist_V_0 / (dist.dist_V_0 - dist.dist_V_2);
		right = V_1_hatch + (V_2_hatch - V_1_hatch) * dist.dist_V_1 / (dist.dist_V_1 - dist.dist_V_2);
	}
	Projection(Line& main, Point& first, Point& second, Point& reverse_side, SignDist& sign) {
		double scalar_0 = main.x_proj * (first.x - main.starting.x) + main.y_proj * (first.y - main.starting.y) + main.z_proj * (first.z - main.starting.z);
		double scalar_0 = main.x_proj * (second.x - main.starting.x) + main.y_proj * (second.y - main.starting.y) + main.z_proj * (second.z - main.starting.z);
		double scalar_0 = main.x_proj * (reverse_side.x - main.starting.x) + main.y_proj * (reverse_side.y - main.starting.y) + main.z_proj * (reverse_side.z - main.starting.z);

		left = scalar_0 + (scalar_2 - scalar_0) * sign.dist_V_0 / (sign.dist_V_0 - sign.dist_V_2);
		right = scalar_1 + (scalar_2 - scalar_1) * sign.dist_V_1 / (sign.dist_V_1 - sign.dist_V_2);

		if (left > right) std::swap(left, right);
	}
	bool intersect(Projection& first, Projection& second) {
		if (first.left > second.right || second.left > first.right) return true;

		return false;
	}
};

struct Triangle {
	bool intersect = false;

	Point A;
	Point B;
	Point C;

	size_t id;

	Projection x_proj;

	bool intersection = false;
	void get_x_projection(); // Potential is rudyment
	
};

void take_triangles(std::vector<Triangle>& triangles, std::string input_file);

