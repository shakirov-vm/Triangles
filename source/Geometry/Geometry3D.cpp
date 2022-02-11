
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "Geometry3D.h"
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

bool Vector::is_null() const {
	if (equal_double(x, 0) && equal_double(y, 0) && equal_double(z, 0)) return true;
	return false;
}
double Vector::lenght() const {
    return std::sqrt(x * x + y * y + z * z);
}
void Vector::mult_length(double scalar) {
	if (equal_double(scalar, 0)) return; //maybe exception?
	x *= scalar;
	y *= scalar;
	z *= scalar;
}
Vector Vector::reverse() {
	x = -x;
	y = -y;
	z = -z;

	return *this;
}

//Quaternion

Quaternion operator*(Quaternion const &first, Quaternion const &second) {
    Quaternion returned; //we can't do second constructor
	//do returned because we have constructor of phi and e vector
    returned.w = first.w * second.w - scalar_mult(first.qvec, second.qvec);
    returned.qvec = second.w * first.qvec + first.w * second.qvec + vector_mult(first.qvec, second.qvec);

    return returned;
}

Quaternion::Quaternion(double const phi, Vector const &e) {
	w = cos(phi/2);
	qvec.x = e.x * sin(phi/2);
	qvec.y = e.y * sin(phi/2);
	qvec.z = e.z * sin(phi/2);
}
Quaternion Quaternion::conjugate() const { // const, because we don't want conjigate our quaternion, we want copy

	Quaternion returned(*this); // we can do =
	returned.qvec = returned.qvec.reverse();
	return returned;
}
void Quaternion::dump() const {
	printf("(%lf, %lf, %lf, %lf)", w, qvec.x, qvec.y, qvec.z); // maybe will better do operator for this? but...
}

//Triangle

void Triangle::get_x_projection() {
    x_proj.left = std::min(std::min(A.x, B.x), C.x); 
    x_proj.right = std::max(std::max(A.x, B.x), C.x); 
}
bool Triangle::in_triangle(Point const &P) const { //or point is point of trian        !!!!!!!!!!!!

	Vector PA = (P - A) * (1 / (P - A).lenght());
	Vector PB = (P - B) * (1 / (P - B).lenght());
	Vector PC = (P - C) * (1 / (P - C).lenght());

	const double pi = 3.1415926535;
										
	if (equal_double(acos(scalar_mult(PA, PB)) + acos(scalar_mult(PB, PC)) + acos(scalar_mult(PC, PA)), 2 * pi)) return true;
	return false;
}
void Triangle::dump() const {
	printf("Triangle %ld: A(%lf, %lf, %lf), B(%lf, %lf, %lf), C(%lf, %lf, %lf)\n", id, A.x, A.y, A.z, B.x, B.y, B.z, C.x, C.y, C.z);
}

//Surface

Surface::Surface(Triangle const &trian) { 
    // first - A, second - B, third - C
    surf.x = trian.A.y * (trian.B.z - trian.C.z) + trian.B.y * (trian.C.z - trian.A.z) + trian.C.y * (trian.A.z - trian.B.z);
    surf.y = trian.A.x * (trian.C.z - trian.B.z) + trian.B.x * (trian.A.z - trian.C.z) + trian.C.x * (trian.B.z - trian.A.z);
    surf.z = trian.A.x * (trian.B.y - trian.C.y) + trian.B.x * (trian.C.y - trian.A.y) + trian.C.x * (trian.A.y - trian.B.y);

    D = - surf.x * trian.A.x - surf.y * trian.A.y - surf.z * trian.A.z; 

    double normalize = 1 / surf.lenght();

    if (D < 0) normalize = -normalize;

    surf.x *= normalize;
    surf.y *= normalize;
    surf.z *= normalize;
    D      *= normalize;

    if(ULTRA_DEBUG) printf("%ld surface: %lf * x + %lf * y + %lf * z + %lf = 0\n", trian.id, surf.x, surf.y, surf.z, D);
}

//Line

Line::Line(Surface& one, Surface& two) {
    //guide or direct
    direct.x = one.surf.y * two.surf.z - one.surf.z * two.surf.y;
    direct.y = one.surf.z * two.surf.x - one.surf.x * two.surf.z;
    direct.z = one.surf.x * two.surf.y - one.surf.y * two.surf.x;
    //s is D
    double scalar_n_n = scalar_mult(one.surf, two.surf);
    double lenght_1 = one.surf.lenght();
    double lenght_2 = two.surf.lenght();

    double a = (two.D * scalar_n_n - one.D * lenght_2 * lenght_2) / (scalar_n_n * scalar_n_n - lenght_2 * lenght_2 * lenght_1 * lenght_1);
    double b = (one.D * scalar_n_n - two.D * lenght_1 * lenght_1) / (scalar_n_n * scalar_n_n - lenght_2 * lenght_2 * lenght_1 * lenght_1);

    refer.x = a * one.surf.x + b * two.surf.x;
    refer.y = a * one.surf.y + b * two.surf.y;
    refer.z = a * one.surf.z + b * two.surf.z;

    if (ULTRA_DEBUG) printf("Line:\n\tx = %lf + t * %lf\n\ty = %lf + t * %lf\n\tz = %lf + t * %lf\n", refer.x, direct.x, refer.y, direct.y, refer.z, direct.z);
}
Line::Line(Point& one, Point& two) {
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

SignDist::SignDist (Surface& surf, Triangle& trian) {
    dist_V_0 = trian.A.x * surf.surf.x + trian.A.y * surf.surf.y + trian.A.z * surf.surf.z + surf.D;
    dist_V_1 = trian.B.x * surf.surf.x + trian.B.y * surf.surf.y + trian.B.z * surf.surf.z + surf.D;
    dist_V_2 = trian.C.x * surf.surf.x + trian.C.y * surf.surf.y + trian.C.z * surf.surf.z + surf.D;
}
void SignDist::dump() const {
	printf("SignDist: %lf, %lf, %lf\n", dist_V_0, dist_V_1, dist_V_2);
}

//Projection

Projection::Projection(Line& main, Triangle& trian, SignDist& sign) {

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

    if (ULTRA_DEBUG) printf("Projection: (%lf, %lf)\n", left, right);
}

//Other

bool intersect(Projection const &first, Projection const &second) {
    if (ULTRA_DEBUG) printf("Intersect projections: one - (%lf, %lf), two - (%lf, %lf)\n", first.left, first.right, second.left, second.right);
    
    if (equal_double(first.right, second.left)) return true;
    if (equal_double(second.right, first.left)) return true;

    if (less_double(first.right, second.left)) return false;
    if (less_double(second.right, first.left)) return false;

    return true;
}
void reverse_normal(Surface& surf) {
	surf.surf.x = -surf.surf.x;
	surf.surf.y = -surf.surf.y;
	surf.surf.z = -surf.surf.z;
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