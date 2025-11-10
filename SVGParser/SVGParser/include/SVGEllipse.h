#pragma once
#include "CustomPoint.h"
#include "SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"
#include "SVGElement.h"

class SVGEllipse : public SVGElement {
protected:
	CustomPoint center;
	float rx, ry;
public:
	SVGEllipse();
	SVGEllipse(CustomPoint, float, float);
	SVGEllipse(const SVGEllipse&);
	SVGEllipse& operator=(const SVGEllipse&);
	~SVGEllipse();
	SVGElement* clone() const override;

	CustomPoint getCenter() const;
	void setCenter(const CustomPoint&);
	float getRadiusX() const;
	void setRadiusX(const float&);
	float getRadiusY() const;
	void setRadiusY(const float&);

	void parse(SVGParser&) const override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};