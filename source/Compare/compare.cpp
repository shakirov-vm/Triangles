
//#include <cmath>
#include "Compare.h"

void compare_triangles (Triangle& zero, Triangle& first) {

	if (is_point(zero) && is_point(first)) {
		handle_2_point(zero, first);
		return;
	} 

	if (is_point(zero) && !is_point(first)) {
		if (is_segment(first)) {
			handle_seg_n_point(first, zero);
			return;
		}
		handle_trian_n_point(first, zero);
		return;
	}

	if (!is_point(zero) && is_point(first)) {
		if (is_segment(zero)) {
			handle_seg_n_point(zero, first);
			return;
		}
		handle_trian_n_point(zero, first);
		return;
	}

	if (is_segment(zero) && is_segment(first)) {
		handle_2_seg(zero, first);
		return;
	}

	Surface zero_surf(zero);
	Surface first_surf(first);

	if (is_segment(zero) && !is_segment(first)) {
		handle_seg_n_trian(zero, first, first_surf);
		return;
	}
	if (is_segment(first) && !is_segment(zero)) {
		handle_seg_n_trian(first, zero, zero_surf);
		return;
	}

	SignDist V_1(zero_surf, first);
	if (less_double(std::max({V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2}), 0) || great_double(std::min({V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2}), 0)) return; //false
	
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
		zero.intersect = true;
		first.intersect = true;
	}
	return;
}

void handle2D(Triangle& first, Triangle& second, Surface& surf) { 

	Vector old_normal(surf.surf.x, surf.surf.y, surf.surf.z);
	Vector new_normal(0, 0, 1);

	double normal_mult = old_normal.lenght() * new_normal.lenght();

	Vector e = vector_mult(old_normal, new_normal);
	if (!equal_double(e.lenght(), 0)) e.mult_length(1/(e.lenght()));

	double phi = acos(scalar_mult(old_normal, new_normal) / normal_mult);
	
	Quaternion quaternion(phi, e);

	double k = -surf.D / (surf.surf.x * surf.surf.x + surf.surf.y * surf.surf.y + surf.surf.z * surf.surf.z);
	Vector shift(k * surf.surf.x, k * surf.surf.y, k * surf.surf.z);

	Triangle2D first2D(first, quaternion, shift);
	Triangle2D second2D(second, quaternion, shift); 

	if (Compare2D(first2D, second2D)) {
		if (DEBUG) printf("2D Triangles intersect: (%ld; %ld)\n", first.id, second.id);
		first.intersect = true;
		second.intersect = true;
	}
}

bool Compare2D(Triangle2D& first, Triangle2D& second) {

	Segment2DTriangle segment_first(first);
	Segment2DTriangle segment_second(second);

	SignDist2DTriangle SD_from_first(second, segment_first.seg_A, segment_first.seg_B, segment_first.seg_C);
	SignDist2DTriangle SD_from_second(first, segment_second.seg_A, segment_second.seg_B, segment_second.seg_C);

	if (check_intersect_triangle(SD_from_first, SD_from_second, segment_first, segment_second)) return true;
	if (check_internal(SD_from_first)) return true;
	if (check_internal(SD_from_second)) return true;

	return false;
}

void take_triangles(std::vector<Triangle>& triangles, std::istream& input_potok, size_t quantity) {

    if (DEBUG) printf("vector size - ");
    if (DEBUG) std::cout << triangles.size() << "\n";

    for (size_t i = 0; i < quantity; i++) {
        
        input_potok >> triangles[i].A.x >> triangles[i].A.y >> triangles[i].A.z;
        input_potok >> triangles[i].B.x >> triangles[i].B.y >> triangles[i].B.z;
        input_potok >> triangles[i].C.x >> triangles[i].C.y >> triangles[i].C.z;
        triangles[i].id = i + 1;
        triangles[i].get_x_projection();
    }
}