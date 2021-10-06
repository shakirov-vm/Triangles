
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
// It is V hatch
	Point first_0(main, first.A);
	Point first_1(main, first.B);
	Point first_2(main, first.C);

	Point second_0(main, second.A);
	Point second_1(main, second.B);
	Point second_2(main, second.C);
// It is t							   // ???????????????????????????????????
	Point first_t_begin(first_0, first_2, get_dist(first.A), get_dist(first.C)); 
	Point first_t_end(first_1, first_2, get_dist(first.B), get_dist(first.C));

	Point second_t_begin(second_0, second_2, get_dist(second.A), get_dist(second.C));
	Point second_t_end(second_0, second_2, get_dist(second.A), get_dist(second.C));

}

bool equal_double(double first, double second) {
	return std::abs(first - second) < eps;
}

bool intesect_seg()