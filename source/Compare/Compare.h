#pragma once

#include "../Geometry/Geometry3D.h"
#include "../Geometry/Geometry2D.h"
#include "../Degenerative/Degenerative.h"

namespace Triangles {

void compare_triangles(Triangle& first, Triangle& second);
bool is_degenerative(Triangle& zero, Triangle& first);
bool is_parallel(Triangle& zero, Triangle& first, Surface const &zero_surf, Surface const &first_surf);
void handle2D (Triangle& first, Triangle& second, Surface const &surf);
bool Compare2D (Triangle2D& first, Triangle2D& second);
void take_triangles(std::vector<Triangle>& triangles, std::istream& input_potok, size_t quantity);

}