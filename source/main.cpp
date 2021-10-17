
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "Sort/sort.h"
#include "Triangle/triangle.h"
#include "Triangle/double_tools.h"

bool UnitTest(std::string& input);

int main(int argc, char** argv) {

	if (argc == 2) {
		std::string input = std::string(argv[1]);

		if (UnitTest(input)) return 0;
		return 1;
	}
	if (argc == 1) {
		printf("1 argc\n");
	}
	printf("Invalid quantity of args\n");
	return 2;
}

bool compare_id(Triangle& first, Triangle& second) {
	return (first.id < second.id); // ??
}

bool UnitTest(std::string& input) { 
        
    std::ifstream input_potok(input);

    if (input_potok.is_open()) {

        size_t quantity;
        input_potok >> quantity;
        quantity /= 9;

		std::vector<Triangle> triangles(quantity); // ???

		take_triangles(triangles, input);

		sort_triangles(triangles);

		std::sort(triangles.begin(), triangles.end(), compare_id);
		printf("\nAll intersecting triangles:\n");
		for (size_t i = 0; i < quantity; i++) {
			if (triangles[i].intersect) std::cout << triangles[i].id << " ";
		}
		printf("\n");

        input_potok.close();
        return false;
    }

    std::cout << "File [" << input << "] can't be open. Test failed\n";
    return false;
}


