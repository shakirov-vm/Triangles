#include <iostream>
#include <algorithm>

#include "../Triangle/triangle.h"
#include "../Triangle/double_tools.h"
#include "compare.h"
#include "compare2D.h"

void compare_triangles (Triangle& zero, Triangle& first) {
	// Check that triangles is true triangle
// Need reverse surface sign!
	Surface zero_surf(zero);
	SignDist V_1(zero_surf, first);
	if (std::max({V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2}) < 0 || std::min({V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2}) > 0) return; //false

	Surface first_surf(first);
	SignDist V_0(first_surf, zero);
	if (std::max({V_0.dist_V_0, V_0.dist_V_1, V_0.dist_V_2}) < 0 || std::min({V_0.dist_V_0, V_0.dist_V_1, V_0.dist_V_2}) > 0) return; //false
printf("So, D is: [%lf, %lf] from (%d, %d)\n", zero_surf.D, first_surf.D, zero.id, first.id);
printf("First surface : [%lf, %lf, %lf]; Second surface : [%lf, %lf, %lf]\n", zero_surf.surf.x, zero_surf.surf.y, zero_surf.surf.z, first_surf.surf.x, first_surf.surf.y, first_surf.surf.z);
	if ((zero_surf.surf.x == first_surf.surf.x) && (zero_surf.surf.y == first_surf.surf.y) && (zero_surf.surf.z == first_surf.surf.z)) {
		if (!equal_double(zero_surf.D, first_surf.D)) return; // false
		
		if (equal_double(zero_surf.D, first_surf.D)) {
			printf("Need write Compare2D\n");
			handle2D (zero, first);
			printf("WHATT??\n");
			return;
			//Compare2D;
		}
	}

	Line main(zero_surf, first_surf);

	Projection zero_proj  (main,  zero, V_0);
	Projection first_proj (main, first, V_1);

	if (intersect(zero_proj, first_proj)) {
		printf("3D Triangles intersect: (%d; %d)\n", zero.id, first.id);
		zero.intersect = true;
		first.intersect = true;
	}
	return;
}

