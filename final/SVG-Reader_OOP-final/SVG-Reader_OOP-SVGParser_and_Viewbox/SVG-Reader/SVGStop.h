#ifndef SVGSTOP_H
#define SVGSTOP_H

#include "CustomColor.h"

class SVGStop {
private:
	CustomColor stopColor;
	float offset;
	float stopOpacity;
public:
	SVGStop();
	SVGStop(CustomColor, float, float);

	void setStopColor(CustomColor);
	void setOffset(float);
	void setStopOpacity(float);

	CustomColor getStopColor() const;
	float getOffset() const;
	float getStopOpacity() const;

	Gdiplus::Color getGdiColor() const;
};
#endif