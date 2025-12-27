//#pragma once
//#include "CustomColor.h"
////#include "SVGGradient.h"
////#include "SVGParser.h"
////#include "SVGRenderer.h"
//
//class SVGGradient;
//namespace Gdiplus {
//	class Color;
//}
//struct Stroke {
//public:
//	CustomColor strokeColor;
//	float strokeWidth;
//	float strokeOpacity;
//	std::string strokeGradId = ""; // ID tham chiếu đến Gradient
//	SVGGradient* strokeGrad = nullptr;
//
//	Stroke();
//	Stroke(CustomColor, float, float);
//	Stroke(const Stroke&);
//	Stroke& operator=(const Stroke&);
//	~Stroke();
//
//	const std::string& getStrokeGradId() const;
//	const SVGGradient* getStrokeGrad() const;
//	void setStrokeGradId(const std::string& id);
//	void setStrokeGrad(SVGGradient* ptr);
//
//	// there is no getter/setter because all attributes are public
//
//	Gdiplus::Color getGdiColor() const;
//
//	void parse(const std::string&);
//	// void render(SVGRenderer&) const;
//	// void transform(Matrix*);
//};
#pragma once
#include "CustomColor.h"

class SVGGradient;

namespace Gdiplus {
	class Color;
}

enum class LineJoins {
	Miter, // Tương đương Gdiplus::LineJoinMiter
	Round, // Tương đương Gdiplus::LineJoinRound
	Bevel  // Tương đương Gdiplus::LineJoinBevel
};

struct Stroke {
public:
	CustomColor strokeColor;
	float strokeWidth;
	float strokeOpacity;
	float strokeMiterLimit;

	std::string strokeGradId = ""; // ID tham chiếu đến Gradient
	SVGGradient* strokeGrad = nullptr;

	Stroke();
	Stroke(CustomColor, float, float,float);
	Stroke(const Stroke&);
	Stroke& operator=(const Stroke&);
	~Stroke();
	// Trong file Stroke.h hoặc Stroke.cpp
	float getStrokeOpacity() const;
	const std::string& getStrokeGradId() const;
	const SVGGradient* getStrokeGrad() const;
	void setStrokeGradId(const std::string& id);
	void setStrokeGrad(SVGGradient* ptr);

	float getStrokeMiterLimit() const { return strokeMiterLimit; }
	void setStrokeMiterLimit(float limit) { strokeMiterLimit = limit; }

	// there is no getter/setter because all attributes are public
	Gdiplus::Color getGdiColor() const;

	void parse(const std::string&);
	// void render(SVGRenderer&) const;
	// void transform(Matrix*);
};
