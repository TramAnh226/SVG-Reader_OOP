#pragma once
#include "SVGEllipse.h"
//#include "SVGParser.h"
//#include "SVGRenderer.h"

// forward declarations to ensure types are known at this point
class SVGRenderer;
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
	void render(SVGRenderer&, Gdiplus::Graphics&) const override;
	// void transform(Matrix*) override;
};