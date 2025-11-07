#pragma once
#ifndef SVG_RECTANGLE_H
#define SVG_RECTANGLE_H
#include "Point.h"

class SvgRectangle {
private: 
	Point top_left_corner;
	float width, height;
public: 
	void parse(const map<string, string>&) override;
	void render(SvgRender&) override;
	void transform(Matrix*) override;
};
#endif
