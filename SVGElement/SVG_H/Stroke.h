#pragma once
#include "CustomColor.h"
#include "../../SVGParser/SVGParser/include/SVGParser.h"
#include "../../SVGRenderer/SVGRenderer.h"
#include "Library.h"

class Stroke {
public: 
	CustomColor strokeColor;
	float strokeWidth;
	float strokeOpacity;

	Stroke();
	Stroke(CustomColor, float, float);
	Stroke(const Stroke&);
	Stroke& operator=(const Stroke&);
	~Stroke();

	// there is no getter/setter because all attributes are public

	void parse(const std::string&);
	// void render(SVGRenderer&) const;
	// void transform(Matrix*);
};
