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

	parse(SVGParser&, XMLElement*) override;
	render(SVGRender&) override;
	// void transform(Matrix*) override;
};
