
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
			printf("Need write Compare2D\n");
			//Compare2D;
		}
	}

	Line main(zero_surf, first_surf);
	// There need SORT POINTS!!!
	Projection zero_proj (main, zero.A, zero.B, zero.C, V_0);
	Projection first_proj (main, first.A, first.B, first.C, V_1);

	if (intersect(zero_proj, first_proj)) {
		zero.intersect = true;
		first.intersect = true;
	}
	return;
}
