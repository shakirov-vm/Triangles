
#include "../Triangle/triangle.h"
#include "compare2D.h"
// It so slow
void handle2D (Triangle& first, Triangle& second) { //Need better handle

	Point2D first_A(first.A.x, first.A.y);
	Point2D first_B(first.B.x, first.B.y);
	Point2D first_C(first.C.x, first.C.y);

	Triangle2D new_first(first_A, first_B, first_C);

	Point2D second_A(second.A.x, second.A.y);
	Point2D second_B(second.B.x, second.B.y);
	Point2D second_C(second.C.x, second.C.y);

	Triangle2D new_second(second_A, second_B, second_C);

	if (Compare2D(new_first, new_second)) {
		printf("2D Triangles intersect: (%d; %d)\n", first.id, second.id);
		first.intersect = true;
		second.intersect = true;
	}
	return;
	//return Compare2D(new_first, new_second);
}

bool Compare2D (Triangle2D& first, Triangle2D second) {

	Segment2D seg_first_A(first.A, first.B);
	Segment2D seg_first_B(first.B, first.C);
	Segment2D seg_first_C(first.C, first.A);

	seg_first_A.set_normal(first.C);
	seg_first_B.set_normal(first.A);
	seg_first_C.set_normal(first.B);

	Segment2D seg_second_A(second.A, second.B);
	Segment2D seg_second_B(second.B, second.C);
	Segment2D seg_second_C(second.C, second.A);

	seg_second_A.set_normal(second.C);
	seg_second_B.set_normal(second.A);
	seg_second_C.set_normal(second.B);

	SignDist2D first_A_second_A(seg_first_A, seg_second_A);
	if (first_A_second_A.check_intersect()) return true;
	SignDist2D first_A_second_B(seg_first_A, seg_second_B);
	if (first_A_second_B.check_intersect()) return true;
	SignDist2D first_A_second_C(seg_first_A, seg_second_C);
	if (first_A_second_C.check_intersect()) return true;
	
	SignDist2D first_B_second_A(seg_first_B, seg_second_A);
	if (first_B_second_A.check_intersect()) return true;
	SignDist2D first_B_second_B(seg_first_B, seg_second_B);
	if (first_B_second_B.check_intersect()) return true;
	SignDist2D first_B_second_C(seg_first_B, seg_second_C);
	if (first_B_second_C.check_intersect()) return true;
	
	SignDist2D first_C_second_A(seg_first_C, seg_second_A);
	if (first_C_second_A.check_intersect()) return true;
	SignDist2D first_C_second_B(seg_first_C, seg_second_B);
	if (first_C_second_B.check_intersect()) return true;
	SignDist2D first_C_second_C(seg_first_C, seg_second_C);
	if (first_C_second_C.check_intersect()) return true;

	if (check_internal( first_A_second_A.fir_start_to_second,
						first_A_second_B.fir_start_to_second,
						first_A_second_C.fir_start_to_second,
						first_B_second_A.fir_start_to_second,
						first_B_second_B.fir_start_to_second,
						first_B_second_C.fir_start_to_second,
						first_C_second_A.fir_start_to_second,
						first_C_second_B.fir_start_to_second,
						first_C_second_C.fir_start_to_second))
		return true;
	if (check_internal( first_A_second_A.sec_start_to_first,
						first_A_second_B.sec_start_to_first,
						first_A_second_C.sec_start_to_first,
						first_B_second_A.sec_start_to_first,
						first_B_second_B.sec_start_to_first,
						first_B_second_C.sec_start_to_first,
						first_C_second_A.sec_start_to_first,
						first_C_second_B.sec_start_to_first,
						first_C_second_C.sec_start_to_first))
		return true;
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
	if ((equation.a * point.x + equation.b * point.y + equation.c) < 0) {
		normal.x = -(normal.x);
		normal.y = -(normal.y);
	}
}

double count_sign_dist(Point2D& point, Line2D& line) {
	return (line.a * point.x + line.b * point.y + line.c);
}
  //VERY BAD
SignDist2D::SignDist2D(Segment2D& first, Segment2D& second) {
	sec_start_to_first = count_sign_dist(second.start, first.equation);
	sec_end_to_first = count_sign_dist(second.end, first.equation);
	fir_start_to_second = count_sign_dist(first.start, second.equation);
	fir_end_to_second = count_sign_dist(first.end, second.equation);

	//IF ANYTHING SIGN DISTANCE IS NULL????
}

bool SignDist2D::check_intersect() { // LESS OR EQUAL
	if (((sec_start_to_first  *  sec_end_to_first) <= 0) && ((fir_start_to_second * fir_end_to_second) <= 0)) return true;
	return false;
}

bool check_internal(double a, double b, double c, double e, double f, double g, double h, double i, double j) {
	if (a > 0 && b > 0 && c > 0 && e > 0 && f > 0 && g > 0 && h > 0 && i > 0 && j > 0) return true;
	return false;
}