#pragma once
#ifndef SVG_POLYGON_H
#define SVG_POLYGON_H
#include "SvgPolyShapeBase.h"

class SvgPolygon : public SvgPolyshapeBase {
public: 
	void parse(const map<string, string>&) override;
	void render(SvgRender&) override;
	void transform(Matrix*) override;
};
#endif