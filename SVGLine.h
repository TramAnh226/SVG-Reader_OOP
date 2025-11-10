#pragma once
#include "Point.h"
#include "SVGElement.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

class SVGLine : public SVGElement {
private: 
	Point start_point, end_point;
public: 
	SVGLine();
	SVGLine(Point, Point);
	SVGLine(const SVGLine&);
	SVGLine& operator=(const SVGLine&);
	~SVGLine();
	SVGElement* clone() const override;
	
	Point getStartPoint() const;
	void setStartPoint(const Point&);
	Point getEndPoint() const;
	void setEndPoint(const Point&);

	void parse(SVGParser&) override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};
