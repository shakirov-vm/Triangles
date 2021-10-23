#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "../Triangle/triangle.h"

//There don't need Class::func!

//Vector
Vector operator+(Vector const &first, Vector const &second)  {
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
double Vector::lenght() {
    return std::sqrt(x * x + y * y + z * z);
}
//Point
Point operator+(Point const &first, Point const &second) {
    return Point(first.x + second.x, first.y + second.y, first.z + second.z);
}
Point operator-(Point const &first, Point const &second) {
    return Point(first.x - second.x, first.y - second.y, first.z - second.z);
}
//Triangle
void Triangle::get_x_projection() {
    x_proj.left = std::min(std::min(A.x, B.x), C.x); //fmin??
    x_proj.right = std::max(std::max(A.x, B.x), C.x); //fmax??
}
//Surface
Surface::Surface(Triangle& trian) { 
    // first - A, second - B, third - C
    surf.x = trian.A.y * (trian.B.z - trian.C.z) + trian.B.y * (trian.C.z - trian.A.z) + trian.C.y * (trian.A.z - trian.B.z);
    surf.y = trian.A.x * (trian.C.z - trian.B.z) + trian.B.x * (trian.A.z - trian.C.z) + trian.C.x * (trian.B.z - trian.A.z);
    surf.z = trian.A.x * (trian.B.y - trian.C.y) + trian.B.x * (trian.C.y - trian.A.y) + trian.C.x * (trian.A.y - trian.B.y);

    D = - surf.x * trian.A.x - surf.y * trian.A.y - surf.z * trian.A.z; 

    double normalize = 1 / surf.lenght();

    surf.x *= normalize;
    surf.y *= normalize;
    surf.z *= normalize;
    D      *= normalize;
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
    // SIGN OF D??
    double a = (two.D * scalar_n_n - one.D * lenght_2 * lenght_2) / (scalar_n_n * scalar_n_n - lenght_2 * lenght_2 * lenght_1 * lenght_1);
    double b = (one.D * scalar_n_n - two.D * lenght_1 * lenght_1) / (scalar_n_n * scalar_n_n - lenght_2 * lenght_2 * lenght_1 * lenght_1);

    refer.x = a * one.surf.x + b * two.surf.x;
    refer.y = a * one.surf.y + b * two.surf.y;
    refer.z = a * one.surf.z + b * two.surf.z;
}
//Line
Line::Line(Point& one, Point& two) {
    refer.x = one.x;
    refer.y = one.y;
    refer.z = one.z;

    direct.x = two.x - one.x;
    direct.y = two.y - one.y;
    direct.z = two.z - one.z;
}
//SignDist
SignDist::SignDist (Surface& surf, Triangle& trian) {
    dist_V_0 = trian.A.x * surf.surf.x + trian.A.y * surf.surf.y + trian.A.z * surf.surf.z + surf.D;
    dist_V_1 = trian.B.x * surf.surf.x + trian.B.y * surf.surf.y + trian.B.z * surf.surf.z + surf.D;
    dist_V_2 = trian.C.x * surf.surf.x + trian.C.y * surf.surf.y + trian.C.z * surf.surf.z + surf.D;
}
//Projection
Projection::Projection(Line& main, Triangle& trian, SignDist& sign) {

    if ((sign.dist_V_0 * sign.dist_V_1) < 0) {
        if (sign.dist_V_2 < 0) {
            if (sign.dist_V_0 > 0) {
                std::swap(sign.dist_V_0, sign.dist_V_2);
                std::swap(trian.A, trian.C);
            }
            else {
                std::swap(sign.dist_V_1, sign.dist_V_2);
                std::swap(trian.B, trian.C);
            }
        }
        else {
            if (sign.dist_V_0 < 0) {
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

    if (left > right)
        std::swap(left, right);
}
bool intersect(Projection& first, Projection& second) {
    if (first.right < second.left) return false;
    if (second.right < first.left) return false;

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////
                                                      // ??
void take_triangles(std::vector<Triangle>& triangles, std::string input_file) {

    std::ifstream input_potok(input_file);

    if (input_potok.is_open()) {

        size_t quantity;
        input_potok >> quantity;

        //quantity /= 9; // I hope it divide

        if (DEBUG) printf("vector size - ");
        if (DEBUG) std::cout << triangles.size() << "\n";

        for (size_t i = 0; i < quantity; i++) {
            
            input_potok >> triangles[i].A.x >> triangles[i].A.y >> triangles[i].A.z;
            input_potok >> triangles[i].B.x >> triangles[i].B.y >> triangles[i].B.z;
            input_potok >> triangles[i].C.x >> triangles[i].C.y >> triangles[i].C.z;
            triangles[i].id = i + 1;
            triangles[i].get_x_projection();
        }

        input_potok.close();
        return;
    }

    std::cout << "File [" << input_file << "] can't be open. Failed\n"; // Exception?
}
//////////////////////////////////////////////////////////////////////////////////////////

bool equal_double(double zero, double first) {
    return std::abs(zero - first) < eps;
}

bool LE_double(double first, double second) {
    if (std::abs(first - second) < eps) return true;
    if (first < second) return true;
    return false;
}

bool BE_double(double first, double second) {
    if (std::abs(first - second) < eps) return true;
    if (first > second) return true;
    return false;
}