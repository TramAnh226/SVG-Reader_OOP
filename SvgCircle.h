#pragma once
#ifndef SVG_CIRCLE_H
#define SVG_CIRCLE_H
#include "SvgEllipse.h"

class Circle : public SvgEllipse {
public: 
	void parse(const map<string, string>&) override;
	void render(SvgRender&) override;
	void transform(Matrix*) override;
};
#endif