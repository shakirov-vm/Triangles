
#include "../Triangle/triangle.h"
#include "compare2D.h"

void handle2D (Triangle& first, Triangle& second, Surface& surf) { 

	if (equal_double(surf.surf.z, 0)) {
		if (equal_double(surf.surf.y, 0)) {
			Point2D first_A(first.A.z, first.A.y);
			Point2D first_B(first.B.z, first.B.y);
			Point2D first_C(first.C.z, first.C.y);

			Point2D second_A(second.A.z, second.A.y);
			Point2D second_B(second.B.z, second.B.y);
			Point2D second_C(second.C.z, second.C.y);

			Triangle2D new_first(first_A, first_B, first_C);
			Triangle2D new_second(second_A, second_B, second_C);

			if (Compare2D(new_first, new_second)) {
				if (DEBUG) printf("2D Triangles intersect: (%ld; %ld)\n", first.id, second.id);
				first.intersect = true;
				second.intersect = true;
			}
			return;
		}
		else {
			Point2D first_A(first.A.x, first.A.z);
			Point2D first_B(first.B.x, first.B.z);
			Point2D first_C(first.C.x, first.C.z);

			Point2D second_A(second.A.x, second.A.z);
			Point2D second_B(second.B.x, second.B.z);
			Point2D second_C(second.C.x, second.C.z);

			Triangle2D new_first(first_A, first_B, first_C);
			Triangle2D new_second(second_A, second_B, second_C);

			if (Compare2D(new_first, new_second)) {
				if (DEBUG) printf("2D Triangles intersect: (%ld; %ld)\n", first.id, second.id);
				first.intersect = true;
				second.intersect = true;
			}
			return;
		}
	}

	else {
		Point2D first_A(first.A.x, first.A.y);
		Point2D first_B(first.B.x, first.B.y);
		Point2D first_C(first.C.x, first.C.y);

		Point2D second_A(second.A.x, second.A.y);
		Point2D second_B(second.B.x, second.B.y);
		Point2D second_C(second.C.x, second.C.y);

		Triangle2D new_first(first_A, first_B, first_C);
		Triangle2D new_second(second_A, second_B, second_C);

		if (Compare2D(new_first, new_second)) {
			if (DEBUG) printf("2D Triangles intersect: (%ld; %ld)\n", first.id, second.id);
			first.intersect = true;
			second.intersect = true;
		}
		return;
	}
}

bool Compare2D (Triangle2D& first, Triangle2D& second) {

	Segment2DTriangle segment_first(first);
	Segment2DTriangle segment_second(second);

	SignDist2DTriangle SD_from_first(second, segment_first.seg_A, segment_first.seg_B, segment_first.seg_C);
	SignDist2DTriangle SD_from_second(first, segment_second.seg_A, segment_second.seg_B, segment_second.seg_C);

	if (check_intersect_triangle(SD_from_first, SD_from_second, segment_first, segment_second)) return true;
	if (check_internal(SD_from_first)) return true;
	if (check_internal(SD_from_second)) return true;

	return false;
}

Line2D::Line2D (Point2D& first, Point2D& second) {
	a = second.y - first.y;
	b = first.x - second.x;
	c = first.y * second.x - first.x * second.y;
}

Segment2D::Segment2D(Point2D& start_, Point2D& end_) : start(start_), end(end_), equation(start_, end_) {
	normal.x = equation.a;
	normal.y = equation.b;
}

void Segment2D::set_normal(Point2D& point) {
	if (less_double(equation.a * point.x + equation.b * point.y + equation.c, 0)) {
		normal.x = -(normal.x);
		normal.y = -(normal.y);
	}
}

SignDist2D::SignDist2D(Triangle2D& trian, Segment2D& seg) { //From point to segment
	SD_A = count_sign_dist(trian.A, seg.equation);
	SD_B = count_sign_dist(trian.B, seg.equation);
	SD_C = count_sign_dist(trian.C, seg.equation);
}

bool check_internal(SignDist2DTriangle& tr) {
	if (   great_double(tr.to_A.SD_A, 0) && great_double(tr.to_A.SD_B, 0) && great_double(tr.to_A.SD_C, 0) 
		&& great_double(tr.to_B.SD_A, 0) && great_double(tr.to_B.SD_B, 0) && great_double(tr.to_B.SD_C, 0) 
		&& great_double(tr.to_C.SD_A, 0) && great_double(tr.to_C.SD_B, 0) && great_double(tr.to_C.SD_C, 0)) return true;
	return false;
}

bool check_intersect_triangle(SignDist2DTriangle& first_from, SignDist2DTriangle& second_from, Segment2DTriangle& seg_first, Segment2DTriangle& seg_second) {

	if (check_segments(first_from.to_A.SD_A, first_from.to_A.SD_B, second_from.to_A.SD_A, second_from.to_A.SD_B, seg_second.seg_A, seg_first.seg_A)) return true;
	if (check_segments(first_from.to_A.SD_B, first_from.to_A.SD_C, second_from.to_B.SD_A, second_from.to_B.SD_B, seg_second.seg_B, seg_first.seg_A)) return true;
	if (check_segments(first_from.to_A.SD_C, first_from.to_A.SD_A, second_from.to_C.SD_A, second_from.to_C.SD_B, seg_second.seg_C, seg_first.seg_A)) return true;

	if (check_segments(first_from.to_B.SD_A, first_from.to_B.SD_B, second_from.to_A.SD_B, second_from.to_A.SD_C, seg_second.seg_A, seg_first.seg_B)) return true;
	if (check_segments(first_from.to_B.SD_B, first_from.to_B.SD_C, second_from.to_B.SD_B, second_from.to_B.SD_C, seg_second.seg_B, seg_first.seg_B)) return true;
	if (check_segments(first_from.to_B.SD_C, first_from.to_B.SD_A, second_from.to_C.SD_B, second_from.to_C.SD_C, seg_second.seg_C, seg_first.seg_B)) return true;

	if (check_segments(first_from.to_C.SD_A, first_from.to_C.SD_B, second_from.to_A.SD_C, second_from.to_A.SD_A, seg_second.seg_A, seg_first.seg_C)) return true;
	if (check_segments(first_from.to_C.SD_B, first_from.to_C.SD_C, second_from.to_B.SD_C, second_from.to_B.SD_A, seg_second.seg_B, seg_first.seg_C)) return true;
	if (check_segments(first_from.to_C.SD_C, first_from.to_C.SD_A, second_from.to_C.SD_C, second_from.to_C.SD_A, seg_second.seg_C, seg_first.seg_C)) return true;

	return false;
}

double count_sign_dist(Point2D& point, Line2D& line) {
	return (line.a * point.x + line.b * point.y + line.c);
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