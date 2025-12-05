#pragma once
#include <windows.h>
#include <gdiplus.h>
namespace Gdiplus {
	struct PointF; // Forward declare PointF
	struct Point;  // Forward declare Point
}
struct CustomPoint {
	float x, y;
	CustomPoint();
	CustomPoint(float, float);
	float distanceTo(const CustomPoint&) const;
	// subsequent development
	// CustomPoint& transform(Matrix* m);

	operator Gdiplus::PointF() const;
	operator Gdiplus::Point() const;
};

