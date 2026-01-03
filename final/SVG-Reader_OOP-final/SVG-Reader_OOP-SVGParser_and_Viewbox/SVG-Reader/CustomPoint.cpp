#include "CustomPoint.h"

CustomPoint::CustomPoint() {
    x = 0.0f;
    y = 0.0f;
}
CustomPoint::CustomPoint(float temp_x, float temp_y) {
    x = temp_x;
    y = temp_y;
};
float CustomPoint::distanceTo(const CustomPoint&) const {
    return std::sqrt(x * x + y * y);
}
