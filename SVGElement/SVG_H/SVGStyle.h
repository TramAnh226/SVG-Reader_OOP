#pragma once
#include "CustomColor.h"
#include "Stroke.h"
#include "../../SVGParser/SVGParser/include/SVGParser.h"
#include "../../SVGRenderer/SVGRenderer.h"

class Stroke; // forward declaration to avoid requiring full type in this header

class SVGStyle {
private: 
	CustomColor fillColor;
	float fillOpacity;
	Stroke* stroke;
	// subsequent development
	// Gradient fillGradient
public: 
	SVGStyle();
	SVGStyle(CustomColor, float, Stroke*);
	SVGStyle(const SVGStyle&);
	SVGStyle& operator=(const SVGStyle&);
	~SVGStyle();

	CustomColor getFillColor() const;
	void setFillColor(const CustomColor&);
	float getFillOpacity() const;	
	void setFillOpacity(const float&);
	Stroke* getStroke() const;
	void setStroke(Stroke*);

	void parse(const std::string&) ;
	// void render(SVGRenderer&, Gdiplus::Graphics&) const override;
	// void transform(Matrix*);
};

