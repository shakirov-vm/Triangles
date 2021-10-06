
#include <iostream>

#include "../Triangle/triangle.h"
#include "compare.h"

void compare_triangles (Triangle& zero, Triangle& first) {

	// Check that triangles is true triangle

	Surface zero_surf(zero);
	SignDist V_1(zero_surf, first);
	if (std::max({V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2}) < 0 || std::min({V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2}) > 0) return; //false

	Surface first_surf(first);
	SignDist V_0(first_surf, zero);
	if (std::max({V_0.dist_V_0, V_0.dist_V_1, V_0.dist_V_2}) < 0 || std::min({V_0.dist_V_0, V_0.dist_V_1, V_0.dist_V_2}) > 0) return; //false

	if ((zero_surf.A_surf == first_surf.A_surf) && (zero_surf.B_surf == first_surf.B_surf) && (zero_surf.C_surf == first_surf.C_surf)) {
		if (zero_surf.D_surf != first_surf.D_surf) return; // false
		if (zero_surf.D_surf == first_surf.D_surf) {
			//Compare2D;
		}
	}

	Line main(zero_surf, first_surf);
										// ???
	Projection proj_from_zero (zero, main, V_0);
	Projection proj_from_first (first, main, V_1);

	//Compare projection
}

bool equal_double(double zero, double first) {
	return std::abs(zero - first) < eps;
}

bool intesect_seg()