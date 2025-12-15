//#pragma once
//#include "CustomColor.h"
//
//class SVGStop {
//private:
//	CustomColor stopColor;
//	float offset;
//	float stopOpacity;
//public:
//	SVGStop();
//	SVGStop(CustomColor, float, float);
//
//	void setStopColor(CustomColor);
//	void setOffset(float);
//	void setStopOpacity(float);
//
//	CustomColor getStopColor() const;
//	float getOffset() const;
//	float getStopOpacity() const;
//};


#pragma once
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
};