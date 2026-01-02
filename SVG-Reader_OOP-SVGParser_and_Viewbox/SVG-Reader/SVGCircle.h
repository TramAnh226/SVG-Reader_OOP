#ifndef SVGCIRCLE_H
#define SVGCIRCLE_H
#include "SVGEllipse.h"

// forward declarations to ensure types are known at this point
namespace Gdiplus { class Graphics; }

class SVGCircle : public SVGEllipse {
public:
	SVGCircle();
	SVGCircle(CustomPoint, float);
	SVGCircle(const SVGCircle&);
	SVGCircle& operator=(const SVGCircle&);
	~SVGCircle();
	SVGElement* clone() const override;

	CustomPoint getCenter() const;
	void setCenter(const CustomPoint&);
	float getRadius() const;
	void setRadius(const float&);

	//void parse(tinyxml2::XMLElement*);
	void parse(SVGParser&, tinyxml2::XMLElement*) override;
	void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;
	// void transform(Matrix*) override;
};
#endif