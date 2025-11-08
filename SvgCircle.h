#pragma once
#include "SVGEllipse.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVG/SVG_READER/SVGRenderer.h"

class SVGCircle : public SVGEllipse {
public: 
	void parse(SVGParser&) override;
	void render(SVGRender&) override;
	// void transform(Matrix*) override;
};
#endif