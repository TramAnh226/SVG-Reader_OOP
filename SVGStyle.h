#pragma once
#include "Color.h"
#include "Stroke.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

using namespace std;

class SVGStyle {
private: 
	Color fillColor;
	float fillOpacity;
	Stroke stroke;
	// subsequent development
	// Gradient fillGradient
public: 
	SVGStyle();
	SVGStyle(Color, float, Stroke);
	SVGStyle(const SVGStyle&);
	SVGStyle& operator=(const SVGStyle&);
	~SVGStyle();

	Color getFillColor();
	void setFillColor(const Color&);
	float getFillOpacity();	
	void setFillOpacity(const float&);
	Stroke getStroke();
	void setStroke(const Stroke&);

	void parse(SVGParser&);
	void render(SVGRenderer&);
};

