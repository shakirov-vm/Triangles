
#include <iostream>

#include "../Triangle/triangle.h"

void compare_triangles (Triangle& first, Triangle& second) {
	Surface first_surf(first);
	Surface second_surf(second);

	//if (first_surf.D_surf == second_surf.D_surf) return; // Dont intersect
	//if (first_surf.D_surf )

	Line main(first_surf, second_surf);

	Line first_AB(first.A, first.B);
	Line first_AC(first.A, first.C);
	Line first_BC(first.B, first.C);

	Line second_AB(second.A, second.B);
	Line second_AC(second.A, second.C);
	Line second_BC(second.B, second.C);

	Point ()

}

bool equal_double(double first, double second) {
	return std::abs(first - second) < eps;
}