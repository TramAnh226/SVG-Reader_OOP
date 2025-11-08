#pragma once
#include "Color.h"
#include "Stroke.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

using namespace std;

class SvgStyle {
private: 
	Color fillColor;
	float fillOpacity;
	Stroke stroke;
	// subsequent development
	// Gradient fillGradient
public: 
	SvgStyle();
	SvgStyle(Color, float, Stroke);
	//SvgStyle(const SvgStyle&);
	//SvgStyle& operator=(const SvgStyle&);
	//~SvgStyle();
	void parseStyle(SVGParser&);
	void renderStyle(SVGRenderer&);
};

