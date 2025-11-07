#pragma once
#ifndef SVG_SQUARE_H
#define SVG_SQUARE_H
#include "SvgRectangle.h"

class SvgSquare : public SvgRectangle {
public: 
	SvgSquare();
	~SvgSquare();

	void parse(const map<string, string>&) override;
	void render(SvgRender&) override;
	void transform(Matrix*) override;
};
#endif
