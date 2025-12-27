//#pragma once
//#include "SVGPolyshapeBase.h"
//
//class SVGPolygon : public SVGPolyshapeBase {
//public:
//	SVGPolygon();
//	SVGPolygon(const std::vector<CustomPoint>&);
//	SVGPolygon(const SVGPolygon&);
//	SVGPolygon& operator=(const SVGPolygon&);
//	~SVGPolygon();
//	SVGElement* clone() const override;
//
//	//void parse(tinyxml2::XMLElement*);
//	void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;
//	// void transform(Matrix*) override;
//};


#pragma once
#include "SVGPolyshapeBase.h"

class SVGPolygon : public SVGPolyshapeBase {
public:
	SVGPolygon();
	SVGPolygon(const std::vector<CustomPoint>&);
	SVGPolygon(const SVGPolygon&);
	SVGPolygon& operator=(const SVGPolygon&);
	~SVGPolygon();
	SVGElement* clone() const override;

	//void parse(tinyxml2::XMLElement*);
	void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;
	// void transform(Matrix*) override;
};
