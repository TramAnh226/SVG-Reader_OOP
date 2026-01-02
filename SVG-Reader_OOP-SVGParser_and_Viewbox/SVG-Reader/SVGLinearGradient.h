#ifndef SVGLINEARGRADIENT_H
#define SVGLINEARGRADIENT_H

#include "SVGGradient.h"

class SVGParser;

class SVGLinearGradient : public SVGGradient {
private:
	float x1, y1;
	float x2, y2;
public:
	SVGLinearGradient();
	SVGLinearGradient(float, float, float, float);
	SVGLinearGradient(const SVGLinearGradient& other);
	SVGLinearGradient& operator=(const SVGLinearGradient& other);

	SVGGradient* clone() const override;

	float getX1() const;
	float getY1() const;
	float getX2() const;
	float getY2() const;

	void setX1(float);
	void setY1(float);
	void setX2(float);
	void setY2(float);

	void parse(SVGParser&, tinyxml2::XMLElement*) override;
};
#endif