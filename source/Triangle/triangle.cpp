


#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "../Triangle/triangle.h"
                                                      // ??
void take_triangles(std::vector<Triangle>& triangles, std::string input_file) {

    std::ifstream input_potok(input_file);

    if (input_potok.is_open()) {

        size_t quantity;
        input_potok >> quantity;

        quantity /= 9; // I hope it divide

        printf("vector size - ");
            std::cout << triangles.size() << "\n";

        for (size_t i = 0; i < quantity; i++) {
            
            input_potok >> triangles[i].A.x >> triangles[i].A.y >> triangles[i].A.z;
            input_potok >> triangles[i].B.x >> triangles[i].B.y >> triangles[i].B.z;
            input_potok >> triangles[i].C.x >> triangles[i].C.y >> triangles[i].C.z;
            triangles[i].id = i;
            triangles[i].get_x_projection();
        }

        input_potok.close();
        return;
    }

    std::cout << "File [" << input_file << "] can't be open. Failed\n"; // Exception?
}


void Triangle::get_x_projection() {
    x_proj.left = std::min(std::min(A.x, B.x), C.x); //fmin??
    x_proj.right = std::max(std::max(A.x, B.x), C.x); //fmax??
}
