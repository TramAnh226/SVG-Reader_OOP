#pragma once
#include "SVGElement.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVGRenderer/SVGRenderer/SVGRenderer.h"
#include "Point.h"
#include <vector>

class SVGPolyshapeBase : public SVGElement {
protected: 
	std::vector<Point> points;
public: 
	SVGPolyshapeBase();
	SVGPolyshapeBase(const std::vector<Point>&);
	SVGPolyshapeBase(const SVGPolyshapeBase&);
	SVGPolyshapeBase& operator=(const SVGPolyshapeBase&);
	~SVGPolyshapeBase();

	std::vector<Point> getPoints() const;
	void setPoints(const std::vector<Point>&);
	
	void parse(SVGParser&) override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};
