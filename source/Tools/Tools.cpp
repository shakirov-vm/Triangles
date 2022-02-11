
#include "Tools.h"

bool equal_double(double one, double two) {
    return std::abs(one - two) < eps; 
}

bool great_double(double one, double two) { // one > two
    if (equal_double(one, two)) return false;
    return one > two;
}

bool less_double(double one, double two) { // one < two
    if (equal_double(one, two)) return false;
    return one < two;
}

bool GE_double(double one, double two) { // one >= two
    return (equal_double(one, two) || great_double(one, two));
}

bool LE_double(double one, double two) { // one <= two
    return (equal_double(one, two) || less_double(one, two));
}

