#pragma once
#ifndef SVG_POLYSHAPE_BASE
#define SVG_POLYSHAPE_BASE
#include "Point.h"
#include <vector>

class SvgPolyshapeBase {
private: 
	vector<Points> list;
public: 
	vector<Point> getList();
	void setList(const vector<Point>&)
	void parse(const map<string, string>&) override;
	void render(SvgRender&) override;
	void transform(Matrix*) override;
};
#endif
