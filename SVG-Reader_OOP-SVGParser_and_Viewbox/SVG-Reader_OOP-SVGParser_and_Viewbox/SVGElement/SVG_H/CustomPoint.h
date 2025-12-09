#pragma once
#include <gdiplus.h>

class CustomPoint {
public:
	float x, y;
	CustomPoint();
	CustomPoint(float, float);
	float distanceTo(const CustomPoint&) const;
	// subsequent development
	// CustomPoint& transform(Matrix* m);

	operator Gdiplus::PointF() const {
        return Gdiplus::PointF(x, y); 
    }
	operator Gdiplus::Point() const {
		return Gdiplus::Point(static_cast<int>(x), static_cast<int>(y)); 
	}
};

