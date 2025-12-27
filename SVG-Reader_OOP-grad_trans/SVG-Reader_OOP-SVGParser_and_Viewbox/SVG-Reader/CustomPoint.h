//#pragma once
//
//#include <windows.h>
//#include <gdiplus.h>
//#include <cmath>
//
//namespace Gdiplus {
//	class PointF; // Forward declare PointF
//	class Point;  // Forward declare Point
//}
//struct CustomPoint {
//	float x, y;
//	CustomPoint();
//	CustomPoint(float, float);
//	float distanceTo(const CustomPoint&) const;
//	// subsequent development
//	// CustomPoint& transform(Matrix* m);
//
//	operator Gdiplus::PointF() const {
//		return Gdiplus::PointF(x, y);
//	}
//	operator Gdiplus::Point() const {
//		return Gdiplus::Point(static_cast<int>(x), static_cast<int>(y));
//	}
//};
//
#pragma once

#include <windows.h>
#include <gdiplus.h>
#include <cmath>

//namespace Gdiplus {
//	class PointF; // Forward declare PointF
//	class Point;  // Forward declare Point
//}
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
	operator Gdiplus::Point() const {
		return Gdiplus::Point(static_cast<int>(x), static_cast<int>(y));
	}
};

