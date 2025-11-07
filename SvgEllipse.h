#pragma once
#ifndef SVG_ELLIPSE
#define SVG_ELLIPSE
#include "Point.h"
#include "SvgRenderer.h"

class SvgEllipse {
private: 
	Point center;
	float rx, ry;
public: 
	SvgEllipse();
	SvgEllipse(Point, float, float);
	SvgEllipse(const SvgEllipse&);
	SvgEllipse& operator=(const SvgEllipse&);
	~SvgEllipse();

	Point getCenter();
	void setCenter(const Point&);
	float getRadiusX();
	void setRadiusX(const float&);
	float getRadiusY();
	void setRadiusY(const float&);

	void parse(const map<string, string>&) override;
	void render(SvgRender&) override;
	void transform(Matrix*) override;
};
#endif
