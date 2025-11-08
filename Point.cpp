#include "Point.h"
#include <cmath>
Point::Point() {
    x = 0.0f;
    y = 0.0f;
}
Point::Point(float temp_x, float temp_y) {
    x = temp_x;
    y = temp_y;
};
float Point::distanceTo(const Point&) const {
    return std::sqrt(x * x + y * y);
}
// subsequent development
// Point& transform(Matrix* m);

