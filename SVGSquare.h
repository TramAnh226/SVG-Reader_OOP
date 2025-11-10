#pragma once
#include "SVGRectangle.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVR_READER/SVGRenderer.h"

class SVGSquare : public SVGRectangle {
public: 
	SVGSquare();
	SVGSquare(const Point&, float);
	SVGSquare(const SVGSquare&);
	SVGSquare& operator=(const SVGSquare&);
	~SVGSquare();
	SVGElement* clone() const override;
	
	void parse(SVGParser&) override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};
