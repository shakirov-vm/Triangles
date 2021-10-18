
//maybe new namespace?

struct Vector2D {
	double x = NAN;
	double y = NAN;

	Vector2D() {}
	Vector2D(double x_, double y_) : x(x_), y(y_) {}
};

struct Point2D : Vector2D {
	Point2D(double x_, double y_) : Vector2D(x_, y_) {}
};

struct Triangle2D {
	Point2D A;
	Point2D B;
	Point2D C;
	
	Triangle2D(Point2D& A_, Point2D& B_, Point2D& C_) : A(A_), B(B_), C(C_) {}
};

struct Line2D {
	double a;
	double b;
	double c;

	Line2D (Point2D& first, Point2D& second);
};

struct Segment2D {
	Point2D   start;
	Point2D     end;
	Line2D equation;
	Vector2D normal;	

	Segment2D(Point2D& start_, Point2D& end_);
	void set_normal(Point2D& point);
};

struct SignDist2D {
	double sec_start_to_first;
	double sec_end_to_first;
	double fir_start_to_second;
	double fir_end_to_second;

	SignDist2D(Point2D& point, Line2D& line);
	SignDist2D(Segment2D& first, Segment2D& second);

	bool check_intersect();
};

bool check_internal(double a, double b, double c, double e, double f, double g, double h, double i, double j);
void handle2D (Triangle& first, Triangle& second);
bool Compare2D (Triangle2D& first, Triangle2D second);
double count_sign_dist(Point2D& point, Line2D& line);