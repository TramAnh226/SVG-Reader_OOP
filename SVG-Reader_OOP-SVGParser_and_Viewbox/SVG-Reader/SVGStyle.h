//#pragma once
//#include "CustomColor.h"
////#include "Stroke.h"
////#include "SVGGradient.h"
////#include "SVGDocumentContext.h"
//
//class Stroke;
//class SVGGradient;
//class SVGDocumentContext;
//
////namespace Gdiplus {
////	class Graphics;
////}
//// class SVGRenderer; // Forward Declare
//
//class SVGStyle {
//private:
//	CustomColor fillColor;
//	float fillOpacity;
//	Stroke* stroke;
//
//	// subsequent development
//	// Gradient fillGradient
//	SVGGradient* grad = nullptr;
//	std::string gradId = "";
//public:
//	SVGStyle();
//	SVGStyle(const CustomColor& color, float opacity, Stroke* newStroke);
//	SVGStyle(const SVGStyle&);
//	SVGStyle& operator=(const SVGStyle&);
//	~SVGStyle();
//
//	CustomColor getFillColor() const;
//	void setFillColor(const CustomColor&);
//	float getFillOpacity() const;
//	void setFillOpacity(const float&);
//	Stroke* getStroke() const;
//	void setStroke(Stroke*);
//
//	const std::string& getGradId() const;
//	const SVGGradient* getGrad() const;
//
//	void setGradId(const std::string& id);
//	void setGrad(SVGGradient* ptr);
//	bool hasGradientFill() const;
//
//	Gdiplus::Color getGdiFillColor() const;
//
//	void parse(const std::string&);
//
//	// Khai báo hàm render (S? d?ng Forward Declaration Gdiplus::Graphics)
//	// void render(SVGRenderer&, Gdiplus::Graphics&) const; 
//	// void transform(Matrix*);
//
//	void resolveGradient(const SVGDocumentContext& context);
//
//};

#pragma once
#include "CustomColor.h"

class Stroke;
class SVGGradient;
class SVGDocumentContext;

enum class FillRule {
	NonZero,
	EvenOdd
};


class SVGStyle {
private:
	CustomColor fillColor;
	float fillOpacity;
	Stroke* stroke;

	// subsequent development
	SVGGradient* grad = nullptr;
	std::string gradId = "";

	FillRule fillRule = FillRule::NonZero;

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

	FillRule getFillRule() const { return fillRule; } 
	void setFillRule(FillRule rule) { fillRule = rule; }

	const std::string& getGradId() const;
	const SVGGradient* getGrad() const;

	void setGradId(const std::string& id);
	void setGrad(SVGGradient* ptr);
	bool hasGradientFill() const;

	Gdiplus::Color getGdiFillColor() const;

	void parse(const std::string&);

	// Khai báo hàm render (S? d?ng Forward Declaration Gdiplus::Graphics)
	// void render(SVGRenderer&, Gdiplus::Graphics&) const; 
	// void transform(Matrix*);

	void resolveGradient(const SVGDocumentContext& context);

};