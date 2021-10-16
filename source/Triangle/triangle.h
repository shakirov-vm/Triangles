#pragma once
#include <vector>
#include <math.h>
#include <algorithm>

bool equal_double(double first, double second);

#define eps 0.0000001

struct Vector {
	double x = NAN;
	double y = NAN;
	double z = NAN;

	Vector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

	friend Vector& operator+(Vector& first, Vector& second);
	friend Vector& operator-(Vector& first, Vector& second);
	friend double scalar_mult(Vector& first, Vector& second);
	friend Vector& vector_mult(Vector& first, Vector& second);

	double lenght() {
		return std::sqrt(x * x + y * y + z * z);
	}
}

Vector& Vector::operator+(Vector& first, Vector& second) {
	return Vector(first.x + second.x, first.y + second.y, first.z + second.z);
}
Vector& Vector::operator-(Vector& first, Vector& second) {
	return Vector(first.x - second.x, first.y - second.y, first.z - second.z);
}
double Vector::scalar_mult(Vector& first, Vector& second) {
	return (first.x * second.x + first.y * second.y + first.z * second.z);
}
Vector& Vector::vector_mult(Vector& first, Vector& second) {
	return Vector(first.y * second.z - first.z * second.y,
				  first.z * second.x - first.x * second.z,
				  first.x * second.y - first.y * second.x);
}
///    scalar mult for points??
struct Point : Vector {

	Point() {}
	Point(double x_, double y_, double z_) : Vector(x_, y_, z_) {} // i hope it true;

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

	void get_x_projection(); // Potential is rudyment
	
};



struct Surface {
	Vector surf;
	double D;

	Surface(Triangle& trian) { // ?
		// first - A, second - B, third - C
		surf.x = trian.A.y * (trian.B.z - trian.C.z) + trian.B.y * (trian.C.z - trian.A.z) + trian.C.y * (trian.A.z - trian.B.z);
		surf.y = trian.A.x * (trian.C.z - trian.B.z) + trian.B.x * (trian.A.z - trian.C.z) + trian.C.x * (trian.B.z - trian.A.z);
		surf.z = trian.A.x * (trian.B.y - trian.C.y) + trian.B.x * (trian.C.y - trian.A.y) + trian.C.x * (trian.A.y - trian.B.y);

		D = - surf.x * trian.A.x - surf.y * trian.A.y - surf.z * trian.A.z; 

		double normalize = 1 / surf.lenght();

		surf.x *= normalize;
		surf.y *= normalize;
		surf.z *= normalize;
		D      *= normalize;
	}
};

struct Line {
	Point   refer; //reference
	Vector direct; //directional

	Line(Surface& one, Surface& two) {
	//guide or direct
		direct.x = one.surf.y * two.surf.z - one.surf.z * two.surf.y;
		direct.y = one.surf.z * two.surf.x - one.surf.x * two.surf.z;
		direct.z = one.surf.x * two.surf.y - one.surf.y * two.surf.x;
//s is D
		double scalar_n_n = scalar_mult(one.surf, two.surf);
		double lenght_1 = one.surf.lenght();
		double lenght_2 = two.surf.lenght();
// SIGN OF D??
		double a = (two.D_surf * scalar_n_n - one.D_surf * lenght_2 * lenght_2) / (scalar_n_n * scalar_n_n - lenght_2 * lenght_2 * lenght_1 * lenght_1);
		double b = (one.D_surf * scalar_n_n - two.D_surf * lenght_1 * lenght_1) / (scalar_n_n * scalar_n_n - lenght_2 * lenght_2 * lenght_1 * lenght_1);
	
		refer.x = a * one.surf.x + b * two.surf.x;
		refer.y = a * one.surf.y + b * two.surf.y;
		refer.z = a * one.surf.z + b * two.surf.z;
	}

	Line(Point& one, Point& two) {
		refer.x = one.x;
		refer.y = one.y;
		refer.z = one.z;

		direct.x = two.x - one.x;
		direct.y = two.y - one.y;
		direct.z = two.z - one.z;
	}
};



struct SignDist {
	double dist_V_0;
	double dist_V_1;
	double dist_V_2;

	SignDist (Surface& surf, Triangle& trian) { // Very big QUESTION!! // Doing more smart
		dist_V_0 = trian.A.x * surf.surf.x + trian.A.y * surf.surf.y + trian.A.z * surf.surf.z + surf.D;
		dist_V_1 = trian.B.x * surf.surf.x + trian.B.y * surf.surf.y + trian.B.z * surf.surf.z + surf.D;
		dist_V_2 = trian.C.x * surf.surf.x + trian.C.y * surf.surf.y + trian.C.z * surf.surf.z + surf.D;
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

		double scalar_0 = scalar_mult(main.direct, trian.A - main.refer);
		double scalar_1 = scalar_mult(main.direct, trian.B - main.refer);
		double scalar_2 = scalar_mult(main.direct, trian.C - main.refer);

		left = scalar_0 + (scalar_2 - scalar_0) * sign.dist_V_0 / (sign.dist_V_0 - sign.dist_V_2);
		right = scalar_1 + (scalar_2 - scalar_1) * sign.dist_V_1 / (sign.dist_V_1 - sign.dist_V_2);

		if (left > right) std::swap(left, right);
	}

	friend bool intersect(Projection& first, Projection& second);
};

void take_triangles(std::vector<Triangle>& triangles, std::string input_file);
bool intersect(Projection& first, Projection& second);



