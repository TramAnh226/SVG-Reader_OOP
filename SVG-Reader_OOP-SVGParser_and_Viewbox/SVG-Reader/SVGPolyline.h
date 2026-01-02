#ifndef SVGPOLYLINE_H
#define SVGPOLYLINE_H

#include "SVGPolyshapeBase.h"

class SVGPolyline : public SVGPolyshapeBase {
public:
	SVGPolyline();
	SVGPolyline(const std::vector<CustomPoint>&);
	SVGPolyline(const SVGPolyline&);
	SVGPolyline& operator=(const SVGPolyline&);
	~SVGPolyline();
	SVGElement* clone() const override;

	// use SVGPolyshapeBase::parse
	void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;
};
#endif