#pragma once
#include "Color.h"
#include "SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

struct Stroke {
public: 
	Color strokeColor;
	float strokeWidth;
	float strokeOpacity;

	Stroke();
	Stroke(Color, float, float);
	Stroke(const Stroke&);
	Stroke& operator=(const Stroke&);
	~Stroke();

	// there is no getter/setter because all attributes are public

	void parse(SVGParser&);
	void render(SVGRenderer&);
	// void transform(Matrix*);
};
