#pragma once
#include "SVGEllipse.h"
#include "SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

class SVGCircle : public SVGEllipse {
public:
	SVGCircle();
	SVGCircle(CustomPoint, float);
	SVGCircle(const SVGCircle&);
	SVGCircle& operator=(const SVGCircle&);
	~SVGCircle();
	SVGElement* clone() const override;

	CustomPoint getCenter() const;
	void setCenter(const CustomPoint&);
	float getRadius() const;
	void setRadius(const float&);

	void parse(SVGParser&) override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};