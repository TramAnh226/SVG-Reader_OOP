#pragma once
#include "Color.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

struct Stroke {
public: 
	Color strokeColor;
	float strokeWidth;
	float strokeOpacity;

	void parse(SVGParser&);
	void render(SVGRenderer&);
};
#endif