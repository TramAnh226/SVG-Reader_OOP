#pragma once
struct Point {
	float x, y;
	Point();
	Point(float, float);
	float distanceTo(const Point&) const;
	// subsequent development
	// Point& transform(Matrix* m);
};

