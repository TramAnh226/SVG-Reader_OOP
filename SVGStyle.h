#pragma once
#include "Color.h"
#include "Stroke.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

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

	Color getFillColor() const;
	void setFillColor(const Color&);
	float getFillOpacity() const;	
	void setFillOpacity(const float&);
	Stroke getStroke() const;
	void setStroke(const Stroke&);

	void parse(SVGParser&);
	void render(SVGRenderer&);
	// void transform(Matrix*);
};

