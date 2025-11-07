#pragma once
#ifndef SVG_LINE_H
#define SVG_LINE_H
#include "Point.h"

class SvgLine {
private: 
	Point start_point, end_point;
public: 
	Point getStartPoint();
	void setStartPoint(const Point&);
	Point getEndPoint();
	void setEndPoint(const Point&);
	void parse(const map<string, string>&) override;
	void render(SvgRender&) override;
	void transform(Matrix*) override;

};
#endif
