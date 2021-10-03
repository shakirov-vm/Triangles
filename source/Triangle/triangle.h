#include <vector>

void take_triangles(vector<double> triangles, char* input_file);

struct Point {
	double x = NAN;
	double y = NAN;
	double z = NAN;

	void print();
	bool is_valid();
};    

struct Triangle {
	Point A;
	Point B;
	Point C;

	segment x_seg;

	bool intersection = false;
	void get_x_segment(); // Potential is rudyment
	
};

void Triangle::get_x_segment() {
	x_seg.left = std::min(std::min(A.x, B.x), C.x); //fmin??
	x_seg.right = std::max(std:max(A.x, B.x), C.x); //fmax??
}

struct Segment {
	Point left;
	Point right;


};