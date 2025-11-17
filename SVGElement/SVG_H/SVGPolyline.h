#pragma once
#include "SVGPolyshapeBase.h"

class SVGPolyline : public SVGPolyshapeBase {
public: 
	SVGPolyline();
	SVGPolyline(const std::vector<CustomPoint>&);
	SVGPolyline(const SVGPolyline&);
	SVGPolyline& operator=(const SVGPolyline&);
	~SVGPolyline();
	SVGElement* clone() const override;
	
	void parse(tinyxml2::XMLElement*) override;
	void render(SVGRenderer&, Gdiplus::Graphics&) const override;
	// void transform(Matrix*) override;
};