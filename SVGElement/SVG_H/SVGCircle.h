#pragma once
#include "SVGEllipse.h"
#include "../../SVGParser/SVGParser/include/SVGParser.h"
#include "../../SVGRenderer/SVGRenderer.h"

class SVGCircle : public SVGEllipse {
public: 
	SVGCircle();
	SVGCircle(CustomPoint, float);	
	SVGCircle(const SVGCircle&);
	SVGCircle& operator=(const SVGCircle&);
	~SVGCircle();
	SVGElement *clone() const override;
	
	CustomPoint getCenter() const;
	void setCenter(const CustomPoint&);
	float getRadius() const;
	void setRadius(const float&);
	
	void parse(SVGParser&, tinyxml2::XMLElement*) override;
	void render(SVGRenderer&, Gdiplus::Graphics&) const override;
	// void transform(Matrix*) override;
};