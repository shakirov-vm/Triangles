
struct Point {
    int x;
    int y;
    int z;

    Point ();
    Point (Line& first, Line& second) {}
};

struct Line {
    Point start;
    Point vec;

    Line(Point& first, Point& second) {}
}