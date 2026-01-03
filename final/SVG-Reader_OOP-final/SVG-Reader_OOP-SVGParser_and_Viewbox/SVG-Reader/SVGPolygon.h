#ifndef SVGPOLYGON_H
#define SVGPOLYGON_H

#include "SVGPolyshapeBase.h"

class SVGPolygon : public SVGPolyshapeBase {
public:
	SVGPolygon();
	SVGPolygon(const std::vector<CustomPoint>&);
	SVGPolygon(const SVGPolygon&);
	SVGPolygon& operator=(const SVGPolygon&);
	~SVGPolygon();
	SVGElement* clone() const override;

	// use SVGPolyshapeBase::parse
	void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;
};
#endif
