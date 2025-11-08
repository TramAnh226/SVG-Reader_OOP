#pragma once
#include "Point.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

class SVGEllipse {
private: 
	Point center;
	float rx, ry;
public: 
	SVGEllipse();
	SVGEllipse(Point, float, float);
	SVGEllipse(const SVGEllipse&);
	SVGEllipse& operator=(const SVGEllipse&);
	~SVGEllipse();

	Point getCenter();
	void setCenter(const Point&);
	float getRadiusX();
	void setRadiusX(const float&);
	float getRadiusY();
	void setRadiusY(const float&);

	void parse(SVGParser&) override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};
