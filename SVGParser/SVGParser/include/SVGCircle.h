#pragma once
#include "SVGEllipse.h"
#include "SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

class SVGCircle : public SVGEllipse {
public: 
	SVGCircle();
	SVGCircle(Point, float);	
	SVGCircle(const SVGCircle&);
	SVGCircle& operator=(const SVGCircle&);
	~SVGCircle();

	Point getCenter() const;
	void setCenter(const Point&);
	float getRadius() const;
	void setRadius(const float&);
	
	void parse(SVGParser&) override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};