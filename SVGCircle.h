#pragma once
#include "SVGEllipse.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

class SVGCircle : public SVGEllipse {
public: 
	SVGCircle();
	SVGCircle(Point, float);	
	SVGCircle(const SVGCircle&);
	SVGCircle& operator=(const SVGCircle&);
	~SVGCircle();	
	Point getCenter();
	void setCenter(const Point&);
	float getRadius();
	void setRadius(const float&);
	
	void parse(SVGParser&) override;
	void render(SVGRender&) override;
	// void transform(Matrix*) override;
};