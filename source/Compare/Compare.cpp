
//#include <cmath>
#include "Compare.h"

/*
This module contains the logic of basic actions when finding intersections.
The algorithm is as follows: 
1) we check the degenerate cases, 
2) calculate the signed distances from one triangle to the plane of another, and vice versa; 
if the triangles intersect, then the distances have different signs.
3) Next comes the processing of the case of parallel planes.
4) Finally, in the case of non-parallelism: we find the line of intersection of the triangles, 
then the projections of the triangles onto this line - and then we look at their intersection.

The 3D case intersection algorithm is taken from the book "Geometric Tools for Computer Graphics" by Schneider P., Eberly D.H.
*/
namespace Triangles {

void compare_triangles (Triangle& zero, Triangle& first) {

	if (is_degenerative(zero, first)) return;

	Surface zero_surf(zero);
	Surface first_surf(first);

	SignDist V_1(zero_surf, first);
	if (less_double(std::max({V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2}), 0) || great_double(std::min({V_1.dist_V_0, V_1.dist_V_1, V_1.dist_V_2}), 0)) return; //false
	
	SignDist V_0(first_surf, zero);
	if (less_double(std::max({V_0.dist_V_0, V_0.dist_V_1, V_0.dist_V_2}), 0) || great_double(std::min({V_0.dist_V_0, V_0.dist_V_1, V_0.dist_V_2}), 0)) return; //false
	
	if(is_parallel(zero, first, zero_surf, first_surf)) return;

	Line main(zero_surf, first_surf);

	Projection zero_proj  (main,  zero, V_0);
	Projection first_proj (main, first, V_1);

	if (intersect(zero_proj, first_proj)) {
		zero.intersect = true;
		first.intersect = true;
	}
	return;
}

bool is_degenerative(Triangle& zero, Triangle& first) {

	if (is_point(zero) && is_point(first)) {
		handle_2_point(zero, first);
		return true;
	} 

	if (is_point(zero) && !is_point(first)) {
		if (is_segment(first)) {
			handle_seg_n_point(first, zero);
			return true;
		}
		handle_trian_n_point(first, zero);
		return true;
	}

	if (!is_point(zero) && is_point(first)) {
		if (is_segment(zero)) {
			handle_seg_n_point(zero, first);
			return true;
		}
		handle_trian_n_point(zero, first);
		return true;
	}

	if (is_segment(zero) && is_segment(first)) {
		handle_2_seg(zero, first);
		return true;
	}

	Surface zero_surf(zero);
	Surface first_surf(first);

	if (is_segment(zero) && !is_segment(first)) {
		handle_seg_n_trian(zero, first, first_surf);
		return true;
	}
	if (is_segment(first) && !is_segment(zero)) {
		handle_seg_n_trian(first, zero, zero_surf);
		return true;
	}
	return false;
}

bool is_parallel(Triangle& zero, Triangle& first, Surface const &zero_surf, Surface const &first_surf) {

	if (vector_mult(zero_surf.normal, first_surf.normal).is_null()) { 
		
		if (!equal_double(zero_surf.D, first_surf.D)) return true;
		
		if (equal_double(zero_surf.D, first_surf.D)) {

			handle2D (zero, first, zero_surf);
			return true;
		}
	}
	return false;
}

void handle2D(Triangle& first, Triangle& second, Surface const &surf) { 

	Vector old_normal(surf.normal.x, surf.normal.y, surf.normal.z);
	Vector new_normal(0, 0, 1);

	double normal_mult = old_normal.lenght() * new_normal.lenght();

	Vector e = vector_mult(old_normal, new_normal);
	if (!equal_double(e.lenght(), 0)) e.mult_length(1/(e.lenght()));

	double phi = acos(scalar_mult(old_normal, new_normal) / normal_mult);
	
	Quaternion quaternion(phi, e);

	double k = -surf.D / (surf.normal.x * surf.normal.x + surf.normal.y * surf.normal.y + surf.normal.z * surf.normal.z);
	Vector shift(k * surf.normal.x, k * surf.normal.y, k * surf.normal.z);

	Triangle2D first2D(first, quaternion, shift);
	Triangle2D second2D(second, quaternion, shift); 

	if (Compare2D(first2D, second2D)) {

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

    for (size_t i = 0; i < quantity; i++) {
        
        input_potok >> triangles[i].A.x >> triangles[i].A.y >> triangles[i].A.z;
        input_potok >> triangles[i].B.x >> triangles[i].B.y >> triangles[i].B.z;
        input_potok >> triangles[i].C.x >> triangles[i].C.y >> triangles[i].C.z;
        triangles[i].id = i + 1;
        triangles[i].get_x_projection();
    }
}

}