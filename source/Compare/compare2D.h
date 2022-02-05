
//maybe new namespace?

struct Vector2D {
	double x = NAN;
	double y = NAN;

	Vector2D(double x_ = NAN, double y_ = NAN) : x(x_), y(y_) {}
	Vector2D(Vector ThreeD) : x(ThreeD.x), y(ThreeD.y) {}
};

Vector cut_quat_to_vec(Quaternion quat);
Vector2D cut_vec_to_2D(Vector vec);

using Point2D = Vector2D;

struct Triangle2D {
	Point2D A;
	Point2D B;
	Point2D C;
	
	Triangle2D(Point2D& A_, Point2D& B_, Point2D& C_) : A(A_), B(B_), C(C_) {}
	Triangle2D(Triangle& trian3D, Quaternion& quat, Vector& shift) : // maybe there move semantic?
				A(cut_vec_to_2D(cut_quat_to_vec((quat * Quaternion(trian3D.A - shift)) * quat.conjugate()))), 
			  	B(cut_vec_to_2D(cut_quat_to_vec((quat * Quaternion(trian3D.B - shift)) * quat.conjugate()))),
			  	C(cut_vec_to_2D(cut_quat_to_vec((quat * Quaternion(trian3D.C - shift)) * quat.conjugate()))) {}
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
	double SD_A;
	double SD_B;
	double SD_C;

	SignDist2D(Triangle2D& trian, Segment2D& seg);

	bool check_intersect();
};

struct Segment2DTriangle {
	Segment2D seg_A;
	Segment2D seg_B;
	Segment2D seg_C;

	Segment2DTriangle(Triangle2D& trian) : seg_A(trian.A, trian.B), seg_B(trian.B, trian.C), seg_C(trian.C, trian.A) {
		seg_A.set_normal(trian.C);
		seg_B.set_normal(trian.A);
		seg_C.set_normal(trian.B);
	}
};

struct SignDist2DTriangle { //From points to 3 segment
	SignDist2D to_A;
	SignDist2D to_B;
	SignDist2D to_C;

	SignDist2DTriangle(Triangle2D& to, Segment2D& first, Segment2D& second, Segment2D& third) : to_A(to, first), to_B(to, second), to_C(to, third) {}
};
// The name of triangle is to what triangle distances

bool check_intersect_triangle(SignDist2DTriangle& first_from, SignDist2DTriangle& second_from, Segment2DTriangle& seg_first, Segment2DTriangle& seg_second);
bool check_internal(SignDist2DTriangle& tr);
bool check_segments(double first_left, double first_right, double second_left, double second_right, Segment2D& first, Segment2D& second);
void handle2D (Triangle& first, Triangle& second, Surface& surf);
bool Compare2D (Triangle2D& first, Triangle2D& second);
double count_sign_dist(Point2D& point, Line2D& line);