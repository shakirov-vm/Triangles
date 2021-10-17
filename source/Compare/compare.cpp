
#include <iostream>
#include <algorithm>

#include "../Triangle/triangle.h"
#include "compare.h"

void compare_triangles (Triangle& zero, Triangle& first) {
printf("\n");
	// Check that triangles is true triangle

	Surface zero_surf(zero);
	SignDist V_1(zero_surf, first);
	if (std::max({V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2}) < 0 || std::min({V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2}) > 0) {
		//printf("Trian - %d; sign_dist: [%lf, %lf, %lf]\n", zero.id, V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2);
		return; //false
	}
//printf("First check, (%d; %d)\n", zero.id, first.id);
//printf("%d normal vector: (%lf, %lf, %lf)\n", zero.id, zero_surf.surf.x, zero_surf.surf.y, zero_surf.surf.z);
//printf("Sigend dist : (%lf, %lf, %lf)\n", V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2);

	Surface first_surf(first);
	SignDist V_0(first_surf, zero);
	if (std::max({V_0.dist_V_0, V_0.dist_V_1, V_0.dist_V_2}) < 0 || std::min({V_0.dist_V_0, V_0.dist_V_1, V_0.dist_V_2}) > 0) {
		//printf("Trian - %d; sign_dist: [%lf, %lf, %lf]\n", first.id, V_0.dist_V_0, V_0.dist_V_1, V_0.dist_V_2);
		return; //false
	}
//printf("Second check, (%d; %d)\n", zero.id, first.id);
//printf("%d normal vector: (%lf, %lf, %lf)\n", first.id, first_surf.surf.x, first_surf.surf.y, first_surf.surf.z);
//printf("Sigend dist : (%lf, %lf, %lf)\n", V_0.dist_V_0, V_0.dist_V_1, V_0.dist_V_2);

//EQUAL_DOUBLE!!
	if ((zero_surf.surf.x == first_surf.surf.x) && (zero_surf.surf.y == first_surf.surf.y) && (zero_surf.surf.z == first_surf.surf.z)) {
		if (zero_surf.D != first_surf.D) {
			printf("Trian %d and %d in parallel surfaces\n", zero.id, first.id);
			return; // false
		}
		if (zero_surf.D == first_surf.D) {
			printf("Need write Compare2D\n");
			//Compare2D;
		}
	}
//printf("Planes intersect, (%d; %d)\n", zero.id, first.id);
	Line main(zero_surf, first_surf);

//printf("Main line: point : (%lf, %lf, %lf); vector : (%lf, %lf, %lf)\n", main.refer.x, main.refer.y, main.refer.z, main.direct.x, main.direct.y, main.direct.z);

	// There need SORT POINTS!!!
	Projection zero_proj  (main,  zero, V_0);
	Projection first_proj (main, first, V_1);

	if (intersect(zero_proj, first_proj)) {
printf("Triangles intersect: (%d; %d)\n", zero.id, first.id);
		zero.intersect = true;
		first.intersect = true;
	}
	return;
}

