#pragma once
#include "CustomColor.h"
#include "Stroke.h"
#include "SVGGradient.h"
#include "SVGDocumentContext.h"

namespace Gdiplus {
	class Graphics;
}
// class SVGRenderer; // Forward Declare

class SVGStyle {
private:
//public:
	CustomColor fillColor;
	float fillOpacity;
	Stroke* stroke;

	// subsequent development
	// Gradient fillGradient
	SVGGradient* grad = nullptr;
	std::string gradId = "";
public:
	SVGStyle();
	SVGStyle(const CustomColor& color, float opacity, Stroke* newStroke);
	SVGStyle(const SVGStyle&);
	SVGStyle& operator=(const SVGStyle&);
	~SVGStyle();

	CustomColor getFillColor() const;
	void setFillColor(const CustomColor&);
	float getFillOpacity() const;
	void setFillOpacity(const float&);
	Stroke* getStroke() const;
	void setStroke(Stroke*);

	const std::string& getGradId() const { return gradId; }
	const SVGGradient* getGrad() const { return grad; }

	void setGradId(const std::string& id) { gradId = id; }
	void setGrad(SVGGradient* ptr);
	bool hasGradientFill() const { return grad != nullptr; }

	Gdiplus::Color getGdiFillColor() const;

	void parse(const std::string&);

	// Khai báo hàm render (S? d?ng Forward Declaration Gdiplus::Graphics)
	// void render(SVGRenderer&, Gdiplus::Graphics&) const; 
	// void transform(Matrix*);

	void resolveGradient(const SVGDocumentContext& context);

};