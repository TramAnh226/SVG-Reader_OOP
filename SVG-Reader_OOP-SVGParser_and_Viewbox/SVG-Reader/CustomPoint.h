#ifndef CUSTOMPOINT_H
#define CUSTOMPOINT_H

#include <windows.h>
#include <gdiplus.h>
#include <cmath>


struct CustomPoint {
	float x, y;
	CustomPoint();
	CustomPoint(float, float);
	float distanceTo(const CustomPoint&) const;

	operator Gdiplus::PointF() const {
		return Gdiplus::PointF(x, y);
	}
	operator Gdiplus::Point() const {
		return Gdiplus::Point(static_cast<int>(x), static_cast<int>(y));
	}
};

#endif