#include <cmath>
#include "../Triangle/triangle.h"
#include "compare2D.h"

void handle2D (Triangle& first, Triangle& second, Surface& surf) { 

	Vector old_normal(surf.surf.x, surf.surf.y, surf.surf.z);
	Vector new_normal(0, 0, 1);

	double normal_mult = old_normal.lenght() * new_normal.lenght();

	Vector e = vector_mult(old_normal, new_normal);
	e.extend(normal_mult);
	double phi = acos(scalar_mult(old_normal, new_normal) / normal_mult);

	std::array<double, 4> quaternion;
	quaternion[0] = cos(phi/2);
	quaternion[1] = e.x * sin(phi/2);
	quaternion[2] = e.y * sin(phi/2);
	quaternion[3] = e.z * sin(phi/2);
//Sign?
	double k = -surf.D / (surf.surf.x * surf.surf.x + surf.surf.y * surf.surf.y + surf.surf.z * surf.surf.z);
	matrix3 matrix(quaternion);
	Vector shift(k * surf.surf.x, k * surf.surf.y, k * surf.surf.z);
	Triangle2D first2D(first, matrix, shift);
	Triangle2D second2D(second, matrix, shift); 

	printf("quaternion is (%lf, %lf, %lf, %lf)\n", quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
	printf("normal is (%lf, %lf, %lf)\n", surf.surf.x, surf.surf.y, surf.surf.z);
	printf("first trian, point B in start: (%lf, %lf, %lf)\n", first.B.x, first.B.y, first.B.z);
	printf("first trian, point B in end  : (%lf, %lf, %lf)\n", (first.B - shift).left_matrix_mult(matrix).x, (first.B - shift).left_matrix_mult(matrix).y, (first.B - shift).left_matrix_mult(matrix).z);

	printf("first 2D is : A(%lf, %lf), B(%lf, %lf)\n", first.A.x, first.A.y, first.B.x, first.B.y);
	printf("second 2D is : A(%lf, %lf), B(%lf, %lf)\n", second.A.x, second.A.y, second.B.x, second.B.y);

	if (Compare2D(first2D, second2D)) {
		if (DEBUG) printf("2D Triangles intersect: (%ld; %ld)\n", first.id, second.id);
		first.intersect = true;
		second.intersect = true;
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