#include <vector>
#include <iterator>
#include <iostream>

#include "../Geometry/Geometry3D.h"
#include "../Compare/Compare.h"
#include "sort.h"

/*
Initially, we have n triangles, in order to compare all of them, we need to make n ^ 2 comparisons, which is a lot. 
Therefore, we perform a preliminary sort: we sort the triangles along the x-axis [by the left point], 
bearing in mind that the triangles intersect if, and only if, their projections intersect. 
Accordingly, if the projections of the triangles intersect, then they themselves can intersect, 
so we call the comparison [yes] function to check. 
This is how we reduce the algorithmic complexity for verification [on random data].
*/
namespace Triangles {

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

}