#pragma once
#include "SVGPolyshapeBase.h"

class SVGPolyline : public SVGPolyshapeBase {
public: 
	SVGPolyline();
	SVGPolyline(const std::vector<Point>&);
	SVGPolyline(const SVGPolyline& other);
	SVGPolyline& operator=(const SVGPolyline& other);
	~SVGPolyline();

	void parse(SVGParser&) override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};