#pragma once
#include "Point.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"
#include "SVGElement.h"

class SVGEllipse : public SVGElement {
private: 
	Point center;
	float rx, ry;
public: 
	SVGEllipse();
	SVGEllipse(Point, float, float);
	SVGEllipse(const SVGEllipse&);
	SVGEllipse& operator=(const SVGEllipse&);
	~SVGEllipse();
	
	Point getCenter() const;
	void setCenter(const Point&);
	float getRadiusX() const;
	void setRadiusX(const float&);
	float getRadiusY() const;
	void setRadiusY(const float&);

	void parse(SVGParser&) override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};
