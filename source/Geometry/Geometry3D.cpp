
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "Geometry3D.h"

/*
This module defines the basic 3D structures: 
vector, quaternion, triangle, surface, line, segment, signed distance from points to the plane;
and operations on them (as well as constructors) 
*/

namespace Triangles {

//Vector

//There don't need Class::func!
Vector operator+(Vector const &first, Vector const &second) {
    return Vector(first.x + second.x, first.y + second.y, first.z + second.z);
}
Vector operator-(Vector const &first, Vector const &second) {
    return Vector(first.x - second.x, first.y - second.y, first.z - second.z);
}
double scalar_mult(Vector const &first, Vector const &second) {
    return (first.x * second.x + first.y * second.y + first.z * second.z);
}
Vector vector_mult(Vector const &first, Vector const &second) {
    return Vector(first.y * second.z - first.z * second.y,
                  first.z * second.x - first.x * second.z,
                  first.x * second.y - first.y * second.x);
}
Vector operator*(Vector const &vec, double const scal) {
    return Vector(scal * vec.x, scal * vec.y, scal * vec.z);
}
Vector operator*(double const scal, Vector const &vec) {
    return Vector(scal * vec.x, scal * vec.y, scal * vec.z);
}
bool operator==(Vector const &first, Vector const &second) {
    return (equal_double(first.x, second.x) && equal_double(first.y, second.y) && equal_double(first.z, second.z));
}

//Quaternion

Quaternion operator*(Quaternion const &first, Quaternion const &second) {
    Quaternion returned; //we can't do second constructor
	//do returned because we have constructor of phi and e vector
    returned.w = first.w * second.w - scalar_mult(first.qvec, second.qvec);
    returned.qvec = second.w * first.qvec + first.w * second.qvec + vector_mult(first.qvec, second.qvec);

    return returned;
}

//Triangle

bool Triangle::in_triangle(Point const &P) const {

    if ((A == P) || (B == P) || (C == P)) return true;

	Vector PA = (P - A) * (1 / (P - A).lenght());
	Vector PB = (P - B) * (1 / (P - B).lenght());
	Vector PC = (P - C) * (1 / (P - C).lenght());

	const double pi = 3.1415926535;
										
	if (equal_double(acos(scalar_mult(PA, PB)) + acos(scalar_mult(PB, PC)) + acos(scalar_mult(PC, PA)), 2 * pi)) return true;
	return false;
}
void Triangle::dump() const {

    std::cout << "Triangle " << id << ": ";
    std::cout << "A(" << A.x << ", " << A.y << ", " << A.z << "), ";
    std::cout << "B(" << B.x << ", " << B.y << ", " << B.z << "), ";
    std::cout << "C(" << C.x << ", " << C.y << ", " << C.z << ")" << std::endl;
}

//Surface

Surface::Surface(Triangle const &trian) { 
    // first - A, second - B, third - C
    normal.x = trian.A.y * (trian.B.z - trian.C.z) + trian.B.y * (trian.C.z - trian.A.z) + trian.C.y * (trian.A.z - trian.B.z);
    normal.y = trian.A.x * (trian.C.z - trian.B.z) + trian.B.x * (trian.A.z - trian.C.z) + trian.C.x * (trian.B.z - trian.A.z);
    normal.z = trian.A.x * (trian.B.y - trian.C.y) + trian.B.x * (trian.C.y - trian.A.y) + trian.C.x * (trian.A.y - trian.B.y);

    D = - normal.x * trian.A.x - normal.y * trian.A.y - normal.z * trian.A.z; 

    double normalize = 1 / normal.lenght();

    if (D < 0) normalize = -normalize;

    normal.x *= normalize;
    normal.y *= normalize;
    normal.z *= normalize;
    D        *= normalize;
}

//Line

Line::Line(Surface const &one, Surface const &two) {
    //guide or direct
    direct.x = one.normal.y * two.normal.z - one.normal.z * two.normal.y;
    direct.y = one.normal.z * two.normal.x - one.normal.x * two.normal.z;
    direct.z = one.normal.x * two.normal.y - one.normal.y * two.normal.x;
    //s is D
    double scalar_n_n = scalar_mult(one.normal, two.normal);
    double lenght_1 = one.normal.lenght();
    double lenght_2 = two.normal.lenght();

    double a = (two.D * scalar_n_n - one.D * lenght_2 * lenght_2) / (scalar_n_n * scalar_n_n - lenght_2 * lenght_2 * lenght_1 * lenght_1);
    double b = (one.D * scalar_n_n - two.D * lenght_1 * lenght_1) / (scalar_n_n * scalar_n_n - lenght_2 * lenght_2 * lenght_1 * lenght_1);

    refer.x = a * one.normal.x + b * two.normal.x;
    refer.y = a * one.normal.y + b * two.normal.y;
    refer.z = a * one.normal.z + b * two.normal.z;
}
Line::Line(Point const &one, Point const &two) {
    refer.x = one.x;
    refer.y = one.y;
    refer.z = one.z;

    direct.x = two.x - one.x;
    direct.y = two.y - one.y;
    direct.z = two.z - one.z;
}

//Segment

Segment::Segment(Triangle const &trian) { // we think that triangle is segment
	// if triangle isn't segment, we must check that scalar_mult is lenght_1 * lenght_2

	if (less_double(scalar_mult(trian.B - trian.A, trian.C - trian.A), 0)) {
		P1 = trian.B;
		P2 = trian.C;
		return;
	}
	if (less_double(scalar_mult(trian.A - trian.B, trian.C - trian.B), 0)) {
		P1 = trian.A;
		P2 = trian.C;
		return;
	}
	if (less_double(scalar_mult(trian.A - trian.C, trian.B - trian.C), 0)) {
		P1 = trian.A;
		P2 = trian.B;
		return;
	}
	if (trian.A == trian.B) {
		P1 = trian.A;
		P2 = trian.C;
		return;
	}
	if (trian.A == trian.C) {
		P1 = trian.A;
		P2 = trian.B;
		return;
	}
	if (trian.B == trian.C) {
		P1 = trian.A;
		P2 = trian.C;
		return;
	}
}
bool Segment::in_segment(Point const &P) { // We think that segment isn't point
	
	double dist = vector_mult(P1 - P2, P - P1).lenght() / (P1 - P2).lenght();
	if (!equal_double(dist, 0)) return false;

	double mu;
		 if (!equal_double(P1.x - P2.x, 0)) mu = (P1.x - P.x) / (P1.x - P2.x);
	else if (!equal_double(P1.y - P2.y, 0)) mu = (P1.y - P.y) / (P1.y - P2.y);
	else if (!equal_double(P1.z - P2.z, 0)) mu = (P1.z - P.z) / (P1.z - P2.z); 

	if (mu < 0 || mu > 1) return false;

	return true;
}

//SignDist

SignDist::SignDist (Surface const &surf, Triangle const &trian) {
    dist_V_0 = trian.A.x * surf.normal.x + trian.A.y * surf.normal.y + trian.A.z * surf.normal.z + surf.D;
    dist_V_1 = trian.B.x * surf.normal.x + trian.B.y * surf.normal.y + trian.B.z * surf.normal.z + surf.D;
    dist_V_2 = trian.C.x * surf.normal.x + trian.C.y * surf.normal.y + trian.C.z * surf.normal.z + surf.D;
}

//Projection

Projection::Projection(Line const &main, Triangle& trian, SignDist& sign) {

    if (less_double(sign.dist_V_0 * sign.dist_V_1, 0)) {
        if (less_double(sign.dist_V_2, 0)) {
            if (great_double(sign.dist_V_0, 0)) {
                std::swap(sign.dist_V_0, sign.dist_V_2);
                std::swap(trian.A, trian.C);
            }
            else {
                std::swap(sign.dist_V_1, sign.dist_V_2);
                std::swap(trian.B, trian.C);
            }
        }
        else {
            if (less_double(sign.dist_V_0, 0)) {
                std::swap(sign.dist_V_0, sign.dist_V_2);
                std::swap(trian.A, trian.C);
            }
            else {
                std::swap(sign.dist_V_1, sign.dist_V_2);
                std::swap(trian.B, trian.C);
            }
        }
     }

    double scalar_0 = scalar_mult(main.direct, trian.A - main.refer);
    double scalar_1 = scalar_mult(main.direct, trian.B - main.refer);
    double scalar_2 = scalar_mult(main.direct, trian.C - main.refer);

    left = scalar_0 + (scalar_2 - scalar_0) * sign.dist_V_0 / (sign.dist_V_0 - sign.dist_V_2);
    right = scalar_1 + (scalar_2 - scalar_1) * sign.dist_V_1 / (sign.dist_V_1 - sign.dist_V_2);

    if (great_double(left, right))
        std::swap(left, right);
}

//Other

bool intersect(Projection const &first, Projection const &second) {
    
    if (equal_double(first.right, second.left)) return true;
    if (equal_double(second.right, first.left)) return true;

    if (less_double(first.right, second.left)) return false;
    if (less_double(second.right, first.left)) return false;

    return true;
}
void reverse_normal(Surface& surf) {
	surf.normal.x = -surf.normal.x;
	surf.normal.y = -surf.normal.y;
	surf.normal.z = -surf.normal.z;
}
bool is_point(Triangle const &trian) {
	return(trian.A == trian.B && trian.A == trian.C);
}
bool is_segment(Triangle const &trian) { // is it the best way? we can check any phi, if phi equal pi => segment
	if (is_point(trian)) return false;
	if (vector_mult(trian.A - trian.B, trian.A - trian.C).is_null()) return true;
	return false;
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
bool compare_proj(Triangle const &first, Triangle const &second) {
    return less_double(first.x_proj.left, second.x_proj.left); 
}

Vector cut_quat_to_vec(Quaternion quat) {
    Vector ret(quat.qvec.x, quat.qvec.y, quat.qvec.z);
    return ret;
}

}