#pragma once
#include "CustomColor.h"
#include "Stroke.h"

namespace Gdiplus {
	class Graphics;
}
// class SVGRenderer; // Forward Declare

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

	Gdiplus::Color getGdiFillColor() const;

	void parse(const std::string&);

	// Khai báo hàm render (S? d?ng Forward Declaration Gdiplus::Graphics)
	// void render(SVGRenderer&, Gdiplus::Graphics&) const; 
	// void transform(Matrix*);
};