#ifndef SVGLINE_H
#define SVGLINE_H

#include "CustomPoint.h"
#include "SVGElement.h"

class SVGLine : public SVGElement {
private:
	CustomPoint start_point, end_point;
public:
	SVGLine();
	SVGLine(CustomPoint, CustomPoint);
	SVGLine(const SVGLine&);
	SVGLine& operator=(const SVGLine&);
	~SVGLine();

	SVGElement* clone() const override;

	CustomPoint getStartPoint() const;
	void setStartPoint(const CustomPoint&);
	CustomPoint getEndPoint() const;
	void setEndPoint(const CustomPoint&);

	void parse(SVGParser&, tinyxml2::XMLElement*) override;
	void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;
};
#endif
