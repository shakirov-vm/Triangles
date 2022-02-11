
#include "Degenerative.h"

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
//!!!!!!
	Point point = point_tr.A;
}

void handle_2_seg(Triangle& zero, Triangle& first) {
	Segment zero_segment(zero);
	Segment first_segment(first);

	Surface zero_surf(Triangle(zero_segment.P1, zero_segment.P2, first_segment.P1));
	Surface first_surf(Triangle(zero_segment.P1, zero_segment.P2, first_segment.P2));

	if (!(zero_surf.surf == first_surf.surf)) return; // crossing straight lines

	if (intersect_segments(zero_segment, first_segment)) {
		zero.intersect = true;
		first.intersect = true;
	}
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

