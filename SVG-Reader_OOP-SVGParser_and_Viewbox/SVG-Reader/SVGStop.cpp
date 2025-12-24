//#include "SVGStop.h"
//
//SVGStop::SVGStop(): offset(0.0f), stopOpacity(1.0f)  {
//}
//
//SVGStop::SVGStop(CustomColor color, float offset, float opacity): stopColor(color), offset(offset), stopOpacity(opacity) {
//	if (this->offset < 0.0f) {
//		this->offset = 0.0f;
//	}
//	if (this->offset > 1.0f) {
//		this->offset = 1.0f;
//	}
//}
//
//void SVGStop::setStopColor(CustomColor color) {
//	this->stopColor = color;
//}
//
//void SVGStop::setOffset(float offset) {
//	if (offset < 0.0f)
//		this->offset = 0.0f;
//	else if (offset > 1.0f)
//		this->offset = 1.0f;
//	else
//		this->offset = offset;
//}
//
//void SVGStop::setStopOpacity(float opacity) {
//	this->stopOpacity = opacity;
//}
//
//CustomColor SVGStop::getStopColor() const {
//	return this->stopColor;
//}
//
//float SVGStop::getOffset() const {
//	return this->offset;
//}
//
//float SVGStop::getStopOpacity() const {
//	return this->stopOpacity;
//}



#include "SVGStop.h"

SVGStop::SVGStop() : offset(0.0f), stopOpacity(1.0f) {
}

SVGStop::SVGStop(CustomColor color, float offset, float opacity) : stopColor(color), offset(offset), stopOpacity(opacity) {
	if (this->offset < 0.0f) {
		this->offset = 0.0f;
	}
	if (this->offset > 1.0f) {
		this->offset = 1.0f;
	}
}

void SVGStop::setStopColor(CustomColor color) {
	this->stopColor = color;
}

void SVGStop::setOffset(float offset) {
	if (offset < 0.0f)
		this->offset = 0.0f;
	else if (offset > 1.0f)
		this->offset = 1.0f;
	else
		this->offset = offset;
}

void SVGStop::setStopOpacity(float opacity) {
	this->stopOpacity = opacity;
}

CustomColor SVGStop::getStopColor() const {
	return this->stopColor;
}

float SVGStop::getOffset() const {
	return this->offset;
}

float SVGStop::getStopOpacity() const {
	return this->stopOpacity;
}

Gdiplus::Color SVGStop::getGdiColor() const {
	// Giả sử stopColor là kiểu CustomColor có các thuộc tính r, g, b
	return Gdiplus::Color(
		static_cast<BYTE>(stopOpacity * 255),
		stopColor.r,
		stopColor.g,
		stopColor.b
	);
}