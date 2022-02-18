#pragma once

#include <cmath>

#include "../Geometry/Geometry3D.h"

/*
This module defines the basic 2D structures: 
vector, triangle, line, segment, signed distance from points to line; 
and operations on them (as well as constructors) 
*/

namespace Triangles {

struct Vector2D {
	double x = NAN;
	double y = NAN;

	Vector2D(double x_ = NAN, double y_ = NAN) : x(x_), y(y_) {}
	Vector2D(Vector ThreeD) : x(ThreeD.x), y(ThreeD.y) {}
};

using Point2D = Vector2D;

Vector2D cut_vec_to_2D(Vector vec);

struct Triangle2D {
	Point2D A;
	Point2D B;
	Point2D C;
	
	Triangle2D(Point2D const &A_, Point2D const &B_, Point2D const &C_) : A(A_), B(B_), C(C_) {}
	Triangle2D(Triangle const &trian3D, Quaternion const &quat, Vector const &shift) :
				A(cut_vec_to_2D(cut_quat_to_vec((quat * Quaternion(trian3D.A - shift)) * quat.conjugate()))), 
			  	B(cut_vec_to_2D(cut_quat_to_vec((quat * Quaternion(trian3D.B - shift)) * quat.conjugate()))),
			  	C(cut_vec_to_2D(cut_quat_to_vec((quat * Quaternion(trian3D.C - shift)) * quat.conjugate()))) {}
};

struct Line2D {
	double a;
	double b;
	double c;

	Line2D (Point2D const &first, Point2D const &second);
};

struct Segment2D {
	Point2D   start;
	Point2D     end;
	Line2D equation;
	Vector2D normal;

	Segment2D(Point2D const &start_, Point2D const &end_) : start(start_), end(end_), equation(start_, end_) {
		normal.x = equation.a;
		normal.y = equation.b;
	}
	void set_normal(Point2D const &point);
};

struct SignDist2D {
	double SD_A;
	double SD_B;
	double SD_C;

	SignDist2D(Triangle2D const &trian, Segment2D const &seg);

	bool check_intersect();
};

struct Segment2DTriangle {
	Segment2D seg_A;
	Segment2D seg_B;
	Segment2D seg_C;

	Segment2DTriangle(Triangle2D const &trian) : seg_A(trian.A, trian.B), seg_B(trian.B, trian.C), seg_C(trian.C, trian.A) {
		seg_A.set_normal(trian.C);
		seg_B.set_normal(trian.A);
		seg_C.set_normal(trian.B);
	}
};

struct SignDist2DTriangle { //From points to 3 segment
	SignDist2D to_A;
	SignDist2D to_B;
	SignDist2D to_C;

	// The name of triangle is to what triangle distances
	SignDist2DTriangle(Triangle2D& to, Segment2D& first, Segment2D& second, Segment2D& third) :
		to_A(to, first), to_B(to, second), to_C(to, third) {}
};

bool check_internal(SignDist2DTriangle const &tr);
double count_sign_dist(Point2D const &point, Line2D const &line);
bool check_intersect_triangle(SignDist2DTriangle const &first_from, SignDist2DTriangle const &second_from, Segment2DTriangle const &seg_first, Segment2DTriangle const &seg_second);
bool check_segments(double first_left, double first_right, double second_left, double second_right, Segment2D const &first, Segment2D const &second);

}