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
	
	if ((is_null(zero_surf.surf) && !is_null(first_surf.surf))) {
		handle_seg_n_trian(zero, first);
		return;
	}
	if ((is_null(first_surf.surf) && !is_null(zero_surf.surf))) {
		handle_seg_n_trian(first, zero);
		return;
	}
	if ((is_null(first_surf.surf) && is_null(zero_surf.surf))) {
		handle_2_seg(zero, first);
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

void reverse_normal (Surface& surf) {
	surf.surf.x = -surf.surf.x;
	surf.surf.y = -surf.surf.y;
	surf.surf.z = -surf.surf.z;
}

void handle_seg_n_trian(Triangle& segment_tr, Triangle& trian, Surface& trian_surf) {

	Segment segment(segment_tr);
	if (equal_double(scalar_mult(segment.P1 - segment.P2, trian_surf.surf), 0)) { // Parallel
		if (equal_double(trian_surf.surf.x * (P1.x - P2.x) + trian_surf.surf.y * (P1.y - P2.y) + trian_surf.surf.z * (P1.z - P2.z), 0)) {
//??????????????????????????????????????????????????????
		}
		else return;
	}
	//Not parallel
	double mu = (D + trian_surf.surf.x * P1.x + trian_surf.surf.y * P1.y + trian_surf.surf.z * P1.z) / 
					(trian_surf.surf.x * (P1.x - P2.x) + trian_surf.surf.y * (P1.y - P2.y) + trian_surf.surf.z * (P1.z - P2.z));

	if (mu < 0 || mu > 1) return; // intersect point out of segment

	Point intersect_point = P1 + mu * (P2 - P1);

	Vector AP = (trian.A - P) / (trian.A - P).lenght();
	Vector BP = (trian.B - P) / (trian.B - P).lenght();
	Vector CP = (trian.C - P) / (trian.C - P).lenght();

	double alphaC = acos(scalar_mult(AP, BP));
	double alphaB = acos(scalar_mult(BP, CP));
	double alphaA = acos(scalar_mult(CP, AP)); //?
																									// It's, make const
	if (equal_double(acos(scalar_mult(AP, BP)) + acos(scalar_mult(BP, CP)) + acos(scalar_mult(CP, AP)), 3.1415926535)) {
		segment_tr.intersect = true;
		trian.intersect = true;
	}	
}

void handle_2_seg(Triangle& zero, Triangle& first) {
	Segment zero_segment(zero);
	Segment first_segment(first);

	Surface zero_surf((zero_segment.P1, zero_segment.P2, first_segment.P1));
	Surface first_surf((zero_segment.P1, zero_segment.P2, first_segment.P2));

	if (!(zero_surf.surf == first_surf.surf)) return; // crossing straight lines


	
}

void intersect_segments() {

}

double sign_dist_3D(Point& point, Segment& seg) {
	return (vector_mult(seg.P1 - seg.P2, point - seg.P1))  
}

bool check_segments(double first_left, double first_right, double second_left, double second_right, Segment2D& first, Segment2D& second) {
	if (equal_double(first_left, 0) && equal_double(first_right, 0) && equal_double(second_left, 0) && equal_double(second_right, 0)) {

		if (!equal_double(first.start.x, first.end.x)) { 
			first_left = first.start.x;
			first_right = first.end.x;

			second_left = second.start.x;
			second_right = second.end.x;
		} else { 
			first_left = first.start.y;
			first_right = first.end.y;

			second_left = second.start.y;
			second_right = second.end.y;
		}

		if (great_double(first_left, first_right)) std::swap(first_left, first_right);
		if (great_double(second_left, second_right)) std::swap(second_left, second_right);
		
		if (less_double(first_right, second_left)) return false;
    	if (less_double(second_right, first_left)) return false;

		return true;
	}
	if (LE_double(first_left * first_right, 0) && LE_double(second_left * second_right, 0)) return true;	
	return false;
}