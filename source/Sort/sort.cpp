#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

#include "../Triangle/triangle.h"
#include "../Compare/compare.h"
#include "sort.h"


bool compare_proj(Triangle& first, Triangle& second); // ???
void compare_triangles (Triangle& zero, Triangle& first);

void sort_triangles(std::vector<Triangle>& triangles) {
	using VectIt = std::vector<Triangle>::iterator;


/*	for (VectIt i = triangles.begin(); i != triangles.end(); i++)
		std::cout << "left: " << i->x_proj.left << ", id: " << i->id << "\n";
	printf("That was before sort\n\n");
*/
	std::sort(triangles.begin(), triangles.end(), compare_proj);

	for (VectIt i = triangles.begin(); i != triangles.end(); i++)
		std::cout << "left: " << i->x_proj.left << ", right: " << i->x_proj.right << ", id: " << i->id << "\n";
	printf("\n");

	for (VectIt left = triangles.begin(); left != std::prev(triangles.end()); left++) {
		for (VectIt right = std::next(left); right != triangles.end(); right++) {
			if (BE_double(left->x_proj.right, right->x_proj.left)) { // we can overload operator > for double?
				compare_triangles (*left, *right);
				std::cout << left->id << " - " << right->id << "\n";
				continue;
			}
			break;
		}
	}

}

bool compare_proj(Triangle& first, Triangle& second) {
	return LE_double(first.x_proj.left, second.x_proj.left); // ??
}

