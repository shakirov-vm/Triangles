#include <iostream>
#include <algorithm>

#include "../Triangle/triangle.h"
#include "../Triangle/double_tools.h"
#include "compare.h"
#include "compare2D.h"

void compare_triangles (Triangle& zero, Triangle& first) {

	Surface zero_surf(zero);
	SignDist V_1(zero_surf, first);
	if (less_double(std::max({V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2}), 0) || great_double(std::min({V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2}), 0)) return; //false

	Surface first_surf(first);
	SignDist V_0(first_surf, zero);
	if (less_double(std::max({V_0.dist_V_0, V_0.dist_V_1, V_0.dist_V_2}), 0) || great_double(std::min({V_0.dist_V_0, V_0.dist_V_1, V_0.dist_V_2}), 0)) return; //false
	
	if (vector_mult(zero_surf.surf, first_surf.surf).is_null()) { 
		if (!equal_double(zero_surf.D, first_surf.D)) return; // false
		
		if (equal_double(zero_surf.D, first_surf.D)) {
			handle2D (zero, first, zero_surf);
			return;
		}
	}

	Line main(zero_surf, first_surf);

	Projection zero_proj  (main,  zero, V_0);
	Projection first_proj (main, first, V_1);

	if (intersect(zero_proj, first_proj)) {
		if (DEBUG) printf("3D Triangles intersect: (%ld; %ld)\n", zero.id, first.id);
		zero.intersect = true;
		first.intersect = true;
	}
	return;
}

void reverse_normal (Surface& surf) {
	surf.surf.x = -surf.surf.x;
	surf.surf.y = -surf.surf.y;
	surf.surf.z = -surf.surf.z;
}
