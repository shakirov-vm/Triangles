
#include <vector>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>

#include "Sort/sort.h"
#include "Geometry/Geometry3D.h"
#include "Compare/Compare.h"

using Triangles::Triangle;

bool YourTest(std::string& input);
bool E2ETest(std::string& input, std::string& answer);
bool compare_id(Triangle& first, Triangle& second);
bool ConsoleUse();

int main(int argc, char** argv) {

	if (argc == 2) {
		std::string input = std::string(argv[1]);

		if (YourTest(input)) return 0;
		return 1;
	}

	if (argc == 3) {
		std::string input = std::string(argv[1]);
		std::string answer = std::string(argv[2]);

		if (E2ETest(input, answer)) {
			if (DEBUG) printf("OK Test\n");
			return 0;
		}
		if (DEBUG) printf("Failed Test\n");
		return 1;
	}

	if (argc == 1) {
		if (ConsoleUse()) return 0; //Always true for now
		return 1;
	}

	printf("Invalid quantity of args\n");
	return 2;
}

bool YourTest(std::string& input) { 
        
    std::ifstream input_potok(input);

    if (input_potok.is_open()) {

        size_t quantity;
        input_potok >> quantity;

		std::vector<Triangle> triangles(quantity); 

		take_triangles(triangles, input_potok, quantity);
        input_potok.close();

		sort_triangles(triangles);

		std::sort(triangles.begin(), triangles.end(), compare_id);

		if (DEBUG) printf("\nAll intersecting triangles:\n");

		for (size_t i = 0; i < quantity; i++) {
			if (triangles[i].intersect && !DEBUG) std::cout << triangles[i].id - 1 << " ";
			if (triangles[i].intersect && DEBUG) std::cout << triangles[i].id << " ";	
		}
		printf("\n");

        return true;
    }

    std::cout << "File [" << input << "] can't be open. Test failed\n";
    return false;
}

bool E2ETest(std::string& input, std::string& answer) { 
        
    std::ifstream input_potok(input);
    std::ifstream answer_potok(answer);

    if (input_potok.is_open() && answer_potok.is_open()) {

        size_t quantity;
        input_potok >> quantity;

		std::vector<Triangle> triangles(quantity); 

		take_triangles(triangles, input_potok, quantity);
        input_potok.close();

		sort_triangles(triangles);

		std::sort(triangles.begin(), triangles.end(), compare_id);
		
		std::vector<size_t> input_res;
		std::vector<size_t> answer_res;

		for (size_t i = 0; i < quantity; i++) {
			if (triangles[i].intersect) input_res.push_back(triangles[i].id - 1); // we corrected 
		}

		size_t tmp = -1; // I understand

		while(true) {
			if(tmp != -1) answer_res.push_back(tmp);
			answer_potok >> tmp;
			if(answer_potok.eof()) break;
		}			

		size_t error = 0;

		if (answer_res.size() == 0 && input_res.size() == 0) {
        	answer_potok.close();
			return true;
		}

		for (size_t i = 0; i != answer_res.size(); i++) {
			if (input_res[i] != answer_res[i]) {
				error++;
			} 
		}

		if (error) {
			return false;
		}

        answer_potok.close();
        return true;
    }

    std::cout << "File [" << input << "] or [" << answer << "] can't be open. Test failed\n";
    return false;
}

bool ConsoleUse() {

    size_t quantity;
    std::cin >> quantity;

	std::vector<Triangle> triangles(quantity); 

	take_triangles(triangles, std::cin, quantity);
	sort_triangles(triangles);

	std::sort(triangles.begin(), triangles.end(), compare_id);

	if (DEBUG) printf("\nAll intersecting triangles:\n");

	for (size_t i = 0; i < quantity; i++) {
			if (triangles[i].intersect && !DEBUG) std::cout << triangles[i].id - 1 << " ";
			if (triangles[i].intersect && DEBUG) std::cout << triangles[i].id << " ";	
	}
	printf("\n");

	return true;
}

bool compare_id(Triangle& first, Triangle& second) {
	return (first.id < second.id); 
}




