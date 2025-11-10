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
	
	void parse(SVGParser&) override;
	void render(SVGRender&) override;
	// void transform(Matrix*) override;
};
