#pragma once
#include "SVGElement.h"
#include "../../SVGParser/SVGParser/include/SVGParser.h"
#include "../../SVGRenderer/SVGRenderer.h"
#include "CustomPoint.h"
#include <vector>

class SVGPolyshapeBase : public SVGElement {
protected: 
	std::vector<CustomPoint> points;
public: 
	SVGPolyshapeBase();
	SVGPolyshapeBase(const std::vector<CustomPoint>&);
	SVGPolyshapeBase(const SVGPolyshapeBase&);
	SVGPolyshapeBase& operator=(const SVGPolyshapeBase&);
	~SVGPolyshapeBase();
	SVGElement* clone() const override;
	
	std::vector<CustomPoint> getPoints() const;
	void setPoints(const std::vector<CustomPoint>&);
	
	void parse(SVGParser&, XMLElement*) override;
	void render(SVGRenderer&) const override;
	// void transform(Matrix*) override;
};
