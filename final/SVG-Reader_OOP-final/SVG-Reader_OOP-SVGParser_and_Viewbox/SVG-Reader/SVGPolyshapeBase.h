#ifndef SVGPOLYSHAPEBASE_H
#define SVGPOLYSHAPEBASE_H

#include <vector>
#include "SVGElement.h"
#include "CustomPoint.h" 

class SVGPolyshapeBase : public SVGElement {
protected:
	std::vector<CustomPoint> points;
public:
	SVGPolyshapeBase();
	SVGPolyshapeBase(const std::vector<CustomPoint>&);
	SVGPolyshapeBase(const SVGPolyshapeBase&);
	SVGPolyshapeBase& operator=(const SVGPolyshapeBase&);
	~SVGPolyshapeBase();
	//SVGElement* clone() const;

	std::vector<CustomPoint> getPoints() const;
	void setPoints(const std::vector<CustomPoint>&);

	void parse(SVGParser&, tinyxml2::XMLElement*) override;
};
#endif