#ifndef SVGSQUARE_H
#define SVGSQUARE_H

#include "SVGRectangle.h"

class SVGSquare : public SVGRectangle {
public:
	SVGSquare();
	SVGSquare(const CustomPoint&, float);
	SVGSquare(const SVGSquare&);
	SVGSquare& operator=(const SVGSquare&);
	~SVGSquare();
	SVGElement* clone() const override;

	void parse(SVGParser&, tinyxml2::XMLElement*) override;
	void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;
};
#endif