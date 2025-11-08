#pragma once
#include "SVGPolyshapeBase.h"

class SVGPolygon : public SVGPolyshapeBase {
public: 
	SVGPolygon();
	SVGPolygon(const std::vector<Point>&);
	SVGPolygon(const SVGPolygon& other);
	SVGPolygon& operator=(const SVGPolygon& other);
	~SVGPolygon();

	void parse(SVGParser&) override;
	void render(SVGRender&) override;
	// void transform(Matrix*) override;
};
