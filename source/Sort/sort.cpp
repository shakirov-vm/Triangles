#include <vector>
#include <iterator>
#include <iostream>

#include "../Geometry/Geometry3D.h"
#include "../Compare/Compare.h"
#include "sort.h"

void sort_triangles(std::vector<Triangle>& triangles) {
	using VectIt = std::vector<Triangle>::iterator;

	std::sort(triangles.begin(), triangles.end(), compare_proj);

	for (VectIt left = triangles.begin(); left != std::prev(triangles.end()); left++) {
		for (VectIt right = std::next(left); right != triangles.end(); right++) {
			if (GE_double(left->x_proj.right, right->x_proj.left)) { 
				compare_triangles (*left, *right);
				continue;
			}
			break;
		}
	}
}

