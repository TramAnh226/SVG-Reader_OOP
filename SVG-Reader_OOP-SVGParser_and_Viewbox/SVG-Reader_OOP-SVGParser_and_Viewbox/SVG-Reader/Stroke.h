#pragma once
#include "CustomColor.h"
#include "SVGGradient.h"
//#include "SVGParser.h"
//#include "SVGRenderer.h"
namespace Gdiplus {
	class Color;
}
struct Stroke {
public:
	CustomColor strokeColor;
	float strokeWidth;
	float strokeOpacity;
	std::string strokeGradId = ""; // ID tham chiếu đến Gradient
	SVGGradient* strokeGrad = nullptr;

	Stroke();
	Stroke(CustomColor, float, float);
	Stroke(const Stroke&);
	Stroke& operator=(const Stroke&);
	~Stroke();

	const std::string& getStrokeGradId() const;
	const SVGGradient* getStrokeGrad() const;
	void setStrokeGradId(const std::string& id);
	void setStrokeGrad(SVGGradient* ptr);

	// there is no getter/setter because all attributes are public

	Gdiplus::Color getGdiColor() const;

	void parse(const std::string&);
	// void render(SVGRenderer&) const;
	// void transform(Matrix*);
};
