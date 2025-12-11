#pragma once
#include "CustomColor.h"
// #include "Library.h"
// #include "../../SVGParser/SVGParser/include/SVGParser.h"
// #include "../../SVGRenderer/SVGRenderer.h"

class Stroke {
public: 
	CustomColor strokeColor;
	float strokeWidth;
	float strokeOpacity;
	std::string linejoin;
	float miterlimit;

	Stroke();
	Stroke(CustomColor, float, float, string, float);
	Stroke(const Stroke&);
	Stroke& operator=(const Stroke&);
	~Stroke();
	void setLineJoin(LineJoin lj) {
		linejoin = lj;
	}
	void setMiterLimit(float limit) {
		miterlimit = limit;
	}
	// there is no getter/setter because all attributes are public

	void parse(const std::string&);
	// void render(SVGRenderer&) const;
	// void transform(Matrix*);
};
