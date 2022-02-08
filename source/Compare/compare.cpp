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
	
	if (is_point(zero) && is_point(first)) {
		handle_2_point(zero, first);
		return;
	} 

	if (is_point(zero) && !is_point(first)) {
		if (first_surf.surf.is_null()) {
			handle_seg_n_point(first, zero);
			return;
		}

		handle_seg_n_trian(zero, first, first_surf);
		return;
	}

	if (!is_point(zero) && is_point(first)) {
		if (zero_surf.surf.is_null()) {
			handle_seg_n_point(zero, first);
			return;
		}

		handle_seg_n_trian(first, zero, zero_surf);
		return;
	}

	if (first_surf.surf.is_null() && zero_surf.surf.is_null()) {
		handle_2_seg(zero, first);
		return;
	}

	if (zero_surf.surf.is_null() && !first_surf.surf.is_null()) {
		handle_seg_n_trian(zero, first, first_surf);
		return;
	}
	if (first_surf.surf.is_null() && !zero_surf.surf.is_null()) {
		handle_seg_n_trian(first, zero, zero_surf);
		return;
	}

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

void reverse_normal(Surface& surf) {
	surf.surf.x = -surf.surf.x;
	surf.surf.y = -surf.surf.y;
	surf.surf.z = -surf.surf.z;
}

void handle_seg_n_trian(Triangle& segment_tr, Triangle& trian, Surface& trian_surf) {

	Segment segment(segment_tr);
	if (equal_double(scalar_mult(segment.P1 - segment.P2, trian_surf.surf), 0)) { // Parallel
		if (equal_double(trian_surf.surf.x * (segment.P1.x - segment.P2.x) + trian_surf.surf.y * (segment.P1.y - segment.P2.y) + trian_surf.surf.z * (segment.P1.z - segment.P2.z), 0)) {
			
			if (intersect_segments(segment, Segment(trian.A, trian.B))) {
				segment_tr.intersect = true;
				trian.intersect = true;
			}
			if (intersect_segments(segment, Segment(trian.A, trian.C))) {
				segment_tr.intersect = true;
				trian.intersect = true;
			}
			if (intersect_segments(segment, Segment(trian.C, trian.B))) {
				segment_tr.intersect = true;
				trian.intersect = true;
			}
			if (trian.in_triangle(segment.P1)) {
				segment_tr.intersect = true;
				trian.intersect = true;
			}
		}
		else return;
	}
	//Not parallel
	double mu = (trian_surf.D + trian_surf.surf.x * segment.P1.x + trian_surf.surf.y * segment.P1.y + trian_surf.surf.z * segment.P1.z) / 
					(trian_surf.surf.x * (segment.P1.x - segment.P2.x) + trian_surf.surf.y * (segment.P1.y - segment.P2.y) + trian_surf.surf.z * (segment.P1.z - segment.P2.z));

	if (mu < 0 || mu > 1) return; // intersect point out of segment

	Point P = segment.P1 + mu * (segment.P2 - segment.P1);
																			
	if (trian.in_triangle(P)) {
		segment_tr.intersect = true;
		trian.intersect = true;
	}	
}

void handle_2_seg(Triangle& zero, Triangle& first) {
	Segment zero_segment(zero);
	Segment first_segment(first);

	Surface zero_surf(Triangle(zero_segment.P1, zero_segment.P2, first_segment.P1));
	Surface first_surf(Triangle(zero_segment.P1, zero_segment.P2, first_segment.P2));

	if (!(zero_surf.surf == first_surf.surf)) return; // crossing straight lines

// Hope that intersect correctly work for parallel and coinciding 

	if (intersect_segments(zero_segment, first_segment)) {
		zero.intersect = true;
		first.intersect = true;
	}
}

void handle_2_point(Triangle& P1, Triangle& P2) {
	if (P1.A == P2.A) {
		P1.intersect = true;
		P2.intersect = true;
	}
}

void handle_seg_n_point(Triangle& segment_tr, Triangle& point_tr) {
	
	Segment segment(segment_tr);
	Point point = point_tr.A;

	if (segment.in_segment(point)) {
		segment_tr.intersect = true;
		point_tr.intersect = true;
	}
}

void handle_trian_n_point(Triangle& trian, Triangle& point_tr) {

	Point point = point_tr.A;
}

bool is_point(Triangle& trian) {
	return(trian.A == trian.B && trian.A == trian.C);
}

bool intersect_segments(Segment const &first, Segment const &second) {
	
	if (!intersect(Projection(first.P1.x, first.P2.x), Projection(second.P1.x, second.P2.x))) return false;
	if (!intersect(Projection(first.P1.y, first.P2.y), Projection(second.P1.y, second.P2.y))) return false;
	if (!intersect(Projection(first.P1.z, first.P2.z), Projection(second.P1.z, second.P2.z))) return false;

	if ((scalar_mult(vector_mult(second.P1 - first.P1, first.P2 - first.P1), vector_mult(second.P2 - first.P1, first.P2 - first.P1)) <= 0) 
	 && (scalar_mult(vector_mult(first.P1 - second.P1, second.P2 - second.P1), vector_mult(first.P2 - second.P1, second.P2 - second.P1)) <= 0)) {
		return true;
	}	
	return false;
}