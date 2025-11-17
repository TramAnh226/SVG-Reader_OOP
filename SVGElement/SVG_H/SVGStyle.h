#pragma once
#include "CustomColor.h"
#include "Stroke.h"
#include "../../SVGParser/SVGParser/include/SVGParser.h"
// #include "../../SVGRenderer/SVGRenderer.h"
// #include "Library.h"

class Stroke;

class SVGStyle {
private: 
	Stroke stroke;	
	CustomColor fillColor;
	float fillOpacity;
	// subsequent development
	// Gradient fillGradient
public: 
	SVGStyle();
	SVGStyle(CustomColor, float, const Stroke&);
	SVGStyle(const SVGStyle&);
	SVGStyle& operator=(const SVGStyle&);
	~SVGStyle();

	CustomColor getFillColor() const;
	void setFillColor(const CustomColor&);
	float getFillOpacity() const;	
	void setFillOpacity(const float&);
	Stroke getStroke() const;
	void setStroke(const Stroke&);

	void parse(const std::string&) ;
	// void render(SVGRenderer&, Gdiplus::Graphics&) const override;
	// void transform(Matrix*);
};

