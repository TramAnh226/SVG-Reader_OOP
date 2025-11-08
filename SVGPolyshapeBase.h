#pragma once
#include "SVGElement.h"
#include "SVGParser/SVGParser/SVGParser.h"
#include "SVGRenderer/SVGRenderer/SVGRenderer.h"
#include "Point.h"
#include <vector>

class SVGPolyshapeBase : public SVGElement {
private: 
	std::vector<Point> list;
public: 
	SVGPolyshapeBase();
	SVGPolyshapeBase(const std::vector<Point>&);
	SVGPolyshapeBase(const SVGPolyshapeBase& other);
	SVGPolyshapeBase& operator=(const SVGPolyshapeBase& other);
	~SVGPolyshapeBase();

	std::vector<Point> getList() const;
	void setList(const std::vector<Point>&);
	
	void parse(SVGParser&) override;
	void render(SVGRenderer&) override;
	// void transform(Matrix*) override;
};
#endif
