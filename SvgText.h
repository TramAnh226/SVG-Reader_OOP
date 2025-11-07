#pragma once
#ifndef SVG_TEXT_H
#include SVG_TEXT_H
#include "Point.h"
#include "SvgRenderer.h"

class SvgText {
private: 
	float font_size;
	Point start;
	std::string content;
public: 
	void parse(const map<string, string>&) override;
	void render(SvgRender&) override;
	void transform(Matrix*) override;
};
#endif