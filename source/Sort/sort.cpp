#include <vector>
#include <iterator>
#include <iostream>

#include "../Triangle/triangle.h"
#include "../Compare/compare.h"
#include "../Triangle/double_tools.h"
#include "sort.h"

void sort_triangles(std::vector<Triangle>& triangles) {
	using VectIt = std::vector<Triangle>::iterator;

	std::sort(triangles.begin(), triangles.end(), compare_proj);

	for (VectIt left = triangles.begin(); left != std::prev(triangles.end()); left++) {
		for (VectIt right = std::next(left); right != triangles.end(); right++) {
			if (BE_double(left->x_proj.right, right->x_proj.left)) { // we can overload operator > for double?
				compare_triangles (*left, *right);
				continue;
			}
			break;
		}
	}

}

bool compare_proj(Triangle& first, Triangle& second) {
	return LE_double(first.x_proj.left, second.x_proj.left); // ??
}

