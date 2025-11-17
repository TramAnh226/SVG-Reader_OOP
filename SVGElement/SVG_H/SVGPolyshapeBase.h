#pragma once
#include "SVGElement.h"
#include "../../SVGParser/SVGParser/include/SVGParser.h"
#include "../../SVGRenderer/SVGRenderer.h"
#include "CustomPoint.h"
#include <vector>
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
	SVGElement* clone() const override;
	
	std::vector<CustomPoint> getPoints() const;
	void setPoints(const std::vector<CustomPoint>&);
	
	void parse(tinyxml2::XMLElement*);
	void render(SVGRenderer&, Gdiplus::Graphics&) const;
	// void transform(Matrix*) override;
};
