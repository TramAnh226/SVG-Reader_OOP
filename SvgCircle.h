#pragma once
#ifndef SVG_CIRCLE_H
#define SVG_CIRCLE_H
#include "SVGEllipse.h"
#include "SVG/SVG_READER/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

class SVGCircle : public SVGEllipse {
public: 
	void parse(SVGParser&) override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};
#endif