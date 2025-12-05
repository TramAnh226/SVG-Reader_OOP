#pragma once
#include "SVGElement.h"
#include <vector>
#include "CustomPoint.h" 

namespace tinyxml2 {
	class XMLDocument;
	class XMLElement;
	class XMLAttribute; // Sửa lỗi chính tả và dùng từ khóa class
	class XMLNode;
	// ... và các lớp khác mà bạn sử dụng
}
class SVGPolyshapeBase : public SVGElement {
protected:
	std::vector<CustomPoint> points;
public:
	SVGPolyshapeBase();
	SVGPolyshapeBase(const std::vector<CustomPoint>&);
	SVGPolyshapeBase(const SVGPolyshapeBase&);
	SVGPolyshapeBase& operator=(const SVGPolyshapeBase&);
	~SVGPolyshapeBase();
	//SVGElement* clone() const;

	std::vector<CustomPoint> getPoints() const;
	void setPoints(const std::vector<CustomPoint>&);

	//void parse(tinyxml2::XMLElement*);

	// parsePolyline and parsePolygon has exclusive parse and render => don't need to override
	void parse(SVGParser&, tinyxml2::XMLElement*) override;
	//void render(SVGRenderer&, Gdiplus::Graphics&) const override;
	
	// void transform(Matrix*) override;
};
