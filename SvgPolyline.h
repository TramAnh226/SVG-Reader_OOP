#pragma once
#ifndef SVG_POLYLINE
#define SVG_POLYLINE
#include "SvgPolyShapeBase.h"

class SvgPolyline : public SvgPolyshapeBase {
public: 
	void parse(const map<string, string>&) override;
	void render(SvgRender&) override;
	void transform(Matrix*) override;
};
#endif