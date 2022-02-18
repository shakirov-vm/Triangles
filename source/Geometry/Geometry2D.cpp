
#include "Geometry2D.h"

namespace Triangles {

Line2D::Line2D (Point2D const &first, Point2D const &second) {
	a = second.y - first.y;
	b = first.x - second.x;
	c = first.y * second.x - first.x * second.y;
}

void Segment2D::set_normal(Point2D const &point) {
	if (less_double(equation.a * point.x + equation.b * point.y + equation.c, 0)) {
		normal.x = -(normal.x);
		normal.y = -(normal.y);
	}
}

SignDist2D::SignDist2D(Triangle2D const &trian, Segment2D const &seg) { //From point to segment
	SD_A = count_sign_dist(trian.A, seg.equation);
	SD_B = count_sign_dist(trian.B, seg.equation);
	SD_C = count_sign_dist(trian.C, seg.equation);
}

bool check_internal(SignDist2DTriangle const &tr) {
	if (   great_double(tr.to_A.SD_A, 0) && great_double(tr.to_A.SD_B, 0) && great_double(tr.to_A.SD_C, 0) 
		&& great_double(tr.to_B.SD_A, 0) && great_double(tr.to_B.SD_B, 0) && great_double(tr.to_B.SD_C, 0) 
		&& great_double(tr.to_C.SD_A, 0) && great_double(tr.to_C.SD_B, 0) && great_double(tr.to_C.SD_C, 0)) return true;
	return false;
}
double count_sign_dist(Point2D const &point, Line2D const &line) {
	return (line.a * point.x + line.b * point.y + line.c);
}
bool check_intersect_triangle(SignDist2DTriangle const &first_from, SignDist2DTriangle const &second_from, Segment2DTriangle const &seg_first, Segment2DTriangle const &seg_second) {

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
bool check_segments(double first_left, double first_right, double second_left, double second_right, Segment2D const &first, Segment2D const &second) {

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

Vector2D cut_vec_to_2D(Vector vec) {
	Vector2D ret(vec.x, vec.y);
	return ret;
}

}