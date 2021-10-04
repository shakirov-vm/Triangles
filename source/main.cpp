
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "Sort/sort.h"
#include "Triangle/triangle.h"

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


bool UnitTest(std::string& input) { 
        
    std::ifstream input_potok(input);

    if (input_potok.is_open()) {

        size_t quantity;
        input_potok >> quantity;

		std::vector<Triangle> triangles(quantity / 9); // ???

		take_triangles(triangles, input);

		sort_triangles(triangles);

        input_potok.close();
        return false;
    }

    std::cout << "File [" << input << "] can't be open. Test failed\n";
    return false;
}


