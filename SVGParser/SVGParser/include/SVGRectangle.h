#pragma once
#include "Point.h"
#include "SVGElement.h"
#include "SVGParser.h"
#include "SVG/SVR_READER/SVGRenderer.h"

class SVGRectangle : public SVGElement {
private: 
	Point top_left_corner;
	float width, height;
public: 
	SVGRectangle();
	SVGRectangle(const Point&, float, float);
	SVGRectangle(const SVGRectangle&);
	SVGRectangle& operator=(const SVGRectangle&);
	~SVGRectangle();

	Point getTopLeftCorner() const;
	void setTopLeftCorner(const Point&);
	float getWidth() const;
	void setWidth(float);
	float getHeight() const;
	void setHeight(float);

	void parse(SVGParser&) override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};
