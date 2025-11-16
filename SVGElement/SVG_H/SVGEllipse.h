#pragma once
#include "CustomPoint.h"
#include "../../SVGParser/SVGParser/include/SVGParser.h"
#include "../../SVGRenderer/SVGRenderer.h"
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

	void parse(SVGParser&, XMLElement*);
	void render(SVGRenderer&, Gdiplus::Graphics&) const override;
	// void transform(Matrix*) override;
};
