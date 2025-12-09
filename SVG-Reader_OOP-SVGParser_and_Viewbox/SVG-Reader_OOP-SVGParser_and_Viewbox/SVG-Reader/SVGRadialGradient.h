#pragma once
#include "SVGGradient.h"
#include "SVGParser.h"

class SVGRadialGradient : public SVGGradient {
private:
	float cx, cy;
	float r;
	float fx, fy;
public:
	SVGRadialGradient();
	SVGRadialGradient(float, float, float, float, float);
	SVGRadialGradient(const SVGRadialGradient& other); 
	SVGRadialGradient& operator=(const SVGRadialGradient& other);

	SVGGradient* clone() const override;

	float getCX() const;
	float getCY() const;
	float getR() const;
	float getFX() const;
	float getFY() const;

	void setCX(float);
	void setCY(float);
	void setR(float);
	void setFX(float);
	void setFY(float);

	void parse(SVGParser&, tinyxml2::XMLElement* node) override;

};