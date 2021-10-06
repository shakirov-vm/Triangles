
void compare_triangles (Triangle& first, Triangle& second);
bool equal_double(double first, double second);
double get_dist(Point& first, Point& second);

#include <math.h>

#define eps 0.0000001

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
	}
};

struct Line {
	Point starting;

	double x_proj;
	double y_proj;
	double z_proj;

	//tau is always 1

	Line(Surface& one, Surface& two) {
		
		if (!equal_double(one.A_surf, 0) && !equal_double(two.A_surf * one.B_surf - one.A_surf * two.B_surf, 0)) {
			starting.x = (one.B_surf * two.D_surf - two.B_surf * one.D_surf) / (two.A_surf * one.B_surf - one.A_surf * two.B_surf);
			starting.y = (two.A_surf * one.D_surf - one.A_surf * two.D_surf) / (two.A_surf * one.B_surf - one.A_surf * two.B_surf);
			starting.z = 0;
			
		} else if (!equal_double(one.A_surf, 0) && equal_double(two.A_surf * one.B_surf - one.A_surf * two.B_surf, 0) &&
													!equal_double(two.A_surf * one.C_surf - one.A_surf * two.C_surf, 0)) {
			starting.x = (one.C_surf * two.D_surf - two.C_surf * one.D_surf) / (two.A_surf * one.C_surf - one.A_surf * two.C_surf);
			starting.y = 0;
			starting.z = (two.A_surf * one.D_surf - one.A_surf * two.D_surf) / (two.A_surf * one.C_surf - one.A_surf * two.C_surf);

		} else if (equal_double(one.A_surf, 0) && !equal_double(two.A_surf, 0) && !equal_double(one.B_surf, 0)) {
			starting.x = (one.B_surf * two.D_surf - two.B_surf * one.D_surf) / (two.A_surf * one.B_surf);
			starting.y = one.D_surf / one.B_surf;
			starting.z = 0;

		} else if (equal_double(one.A_surf, 0) && !equal_double(two.A_surf, 0) && equal_double(one.B_surf, 0) && !equal_double(one.C_surf, 0)) {
			starting.x = (one.C_surf * two.D_surf - two.C_surf * one.D_surf) / (two.A_surf * one.C_surf);
			starting.y = 0;
			starting.z = one.D_surf / one.C_surf;

		} else if (equal_double(one.A_surf, 0) && equal_double(two.A_surf, 0) && !equal_double(one.B_surf, 0) && 
													!equal_double(two.B_surf * one.C_surf - one.B_surf * two.C_surf, 0)) {
			starting.x = 0;
			starting.y = (one.C_surf * two.D_surf - two.C_surf * one.D_surf) / (two.B_surf * one.C_surf - one.B_surf * two.C_surf);
			starting.z = (two.B_surf * one.D_surf - one.B_surf * two.D_surf) / (two.B_surf * one.C_surf - one.B_surf * two.C_surf);

		} else if (equal_double(one.A_surf, 0) && equal_double(two.A_surf, 0) && equal_double(one.B_surf, 0) && !equal_double(two.B_surf, 0) &&
													!equal_double(one.C_surf, 0)) {
			starting.x = 0;
			starting.y = (one.C_surf * two.D_surf - two.C_surf * one.D_surf) / (two.B_surf * one.C_surf);
			starting.z = one.D_surf / two.B_surf;
		
		} else {
			printf("That can't be!\n");
			exit(666);
		}

		x_proj = one.B_surf * two.C_surf - two.B_surf * one.C_surf;
		y_proj = one.C_surf * two.A_surf - one.A_surf * two.C_surf;
		z_proj = one.A_surf * two.B_surf - two.A_surf * one.B_surf; 
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

struct Edge {
	Point begin;
	Point end;

	double dist;

	Edge(Point& first, Point& second) {
		begin = first;
		end = second;

		dist = get_dist(first, second);
	}
};

double get_dist(Point& first, Point& second) {
	
	return sqrt((first.x - second.x) * (first.x - second.x) + (first.y - second.y) * (first.y - second.y) + (first.z - second.z) * (first.z - second.z));
}