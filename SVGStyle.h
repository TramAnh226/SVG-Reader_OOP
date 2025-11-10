#pragma once
#include "CustomColor.h"
#include "Stroke.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

class SVGStyle {
private: 
	CustomColor fillColor;
	float fillOpacity;
	Stroke stroke;
	// subsequent development
	// Gradient fillGradient
public: 
	SVGStyle();
	SVGStyle(CustomColor, float, Stroke);
	SVGStyle(const SVGStyle&);
	SVGStyle& operator=(const SVGStyle&);
	~SVGStyle();

	CustomColor getFillColor() const;
	void setFillColor(const CustomColor&);
	float getFillOpacity() const;	
	void setFillOpacity(const float&);
	Stroke getStroke() const;
	void setStroke(const Stroke&);

	void parse(SVGParser&);
	void render(SVGRenderer&);
	// void transform(Matrix*);
};

