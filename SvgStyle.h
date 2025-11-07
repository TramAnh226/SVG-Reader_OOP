#pragma once
#ifndef SVG_STYLE_H
#define SVG_STYLE_H
#include "Color.h"

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

	void parseStyle(const map<string, string>&);
	void renderStyle(SvgRenderer* r);
};
#endif