#ifndef SVGSTYLE_H
#define SVGSTYLE_H

#include "CustomColor.h"
#include "SVGTransform.h"

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

	SVGTransform transform;
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

	const SVGTransform& getTransform() const;
	SVGTransform& getTransform();
	void setTransform(const SVGTransform&);

	void parse(const std::string&);

	void resolveGradient(const SVGDocumentContext& context);

};
#endif