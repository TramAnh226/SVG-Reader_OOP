#pragma once

struct CustomPoint {
	float x, y;
	CustomPoint();
	CustomPoint(float, float);
	float distanceTo(const CustomPoint&) const;
	// subsequent development
	// CustomPoint& transform(Matrix* m);

	operator Gdiplus::PointF() const {
        return Gdiplus::PointF(x, y); 
    }
};

