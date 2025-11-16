#pragma once
#include "SVGPolyshapeBase.h"

class SVGPolygon : public SVGPolyshapeBase {
public: 
	SVGPolygon();
	SVGPolygon(const std::vector<CustomPoint>&);
	SVGPolygon(const SVGPolygon&);
	SVGPolygon& operator=(const SVGPolygon&);
	~SVGPolygon();
	SVGElement* clone() const override;

	void parse(SVGParser&, tinyxml2::XMLElement*) override;
	void render(SVGRenderer&, Gdiplus::Graphics&) const override;
	// void transform(Matrix*) override;
};
