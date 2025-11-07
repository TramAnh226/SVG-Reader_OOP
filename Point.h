#pragma once
#ifndef POINT_H
#define POINT_H
class Point {
//private: 
//	float x, y;
public:
	float x, y;
	
	Point();
	Point(float, float);
	float distanceTo(const Point&) const;
	// subsequent development
	Point& transform(Matrix* m);
};
#endif
