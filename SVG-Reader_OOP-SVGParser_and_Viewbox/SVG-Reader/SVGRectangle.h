#ifndef SVGRECTANGLE_H
#define SVGRECTANGLE_H

#include "SVGElement.h"
#include "CustomPoint.h"

class SVGRectangle : public SVGElement {
private: 
	CustomPoint top_left_corner;
	float width, height;
public: 
	SVGRectangle();
	SVGRectangle(const CustomPoint&, float, float);
	SVGRectangle(const SVGRectangle&);
	SVGRectangle& operator=(const SVGRectangle&);
	~SVGRectangle();
	SVGElement* clone() const override;

	CustomPoint getTopLeftCorner() const;
	void setTopLeftCorner(const CustomPoint&);
	float getWidth() const;
	void setWidth(float);
	float getHeight() const;
	void setHeight(float);

	void parse(SVGParser&, tinyxml2::XMLElement*) override;
	void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;
};
#endif