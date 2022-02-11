#pragma once

#include "../Geometry/Geometry3D.h"
#include "../Geometry/Geometry2D.h"
#include "../Degenerative/Degenerative.h"

void compare_triangles(Triangle& first, Triangle& second);
void handle2D (Triangle& first, Triangle& second, Surface& surf);
bool Compare2D (Triangle2D& first, Triangle2D& second);
void take_triangles(std::vector<Triangle>& triangles, std::istream& input_potok, size_t quantity);