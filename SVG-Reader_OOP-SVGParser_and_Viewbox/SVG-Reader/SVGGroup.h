#ifndef SVGGROUP_H
#define SVGGROUP_H

#include "tinyxml2.h" 
#include "SVGElement.h"
#include <vector>
#include <algorithm>

class SVGGroup : public SVGElement {
private:


    std::vector<SVGElement*> ElementArray;
    SVGGroup* parent;
    
    // Kích thước vật lý/Viewport của thẻ <svg>
    float width = 0.0f;
    float height = 0.0f;

    std::string viewBox;    // Mặc định là chuỗi rỗng

    float viewBoxW = 0.0f;     // Mặc định
    float viewBoxH = 0.0f;    // Mặc định
    // Tọa độ bắt đầu của ViewBox
    float viewBoxX = 0.0f;
    float viewBoxY = 0.0f;

    std::unordered_map<std::string, std::string> inheritedAttributes;

public:


    SVGGroup();
    SVGGroup(const SVGGroup&);
    SVGGroup& operator= (SVGGroup) noexcept;
    ~SVGGroup();


    friend void swap(SVGGroup& first, SVGGroup& second) noexcept;


    void setParent(SVGGroup*);
    void setElementArray(const std::vector<SVGElement*>&);
    void addElement(SVGElement*);


    SVGGroup* getParent();
    const std::vector<SVGElement*>& getSVGElementArray() const;

    SVGElement* clone() const override;

    void parse(SVGParser&, tinyxml2::XMLElement*) override;
    void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;

    float getViewBoxX() { return this->viewBoxX; };
    float getViewBoxY() { return this->viewBoxY; };
    float getViewBoxH() { return this->viewBoxH; };
    float getViewBoxW() { return this->viewBoxW; };


    float getWidth();
    float getHeight();
    void setWidth(float);
    void setHeight(float);

    void setViewBoxX(float x) { viewBoxX = x; }
    void setViewBoxY(float y) { viewBoxY = y; }
    void setViewBoxW(float w) { this->viewBoxW = w; }
    void setViewBoxH(float h) { this->viewBoxH = h; }
    void setViewBox(const std::string& vb);

    const std::unordered_map<std::string, std::string>& getInheritedAttributes() const;

    // Hàm hợp nhất các thuộc tính kế thừa từ cha vào Group hiện tại
    void mergeInheritedAttributes(const std::unordered_map<std::string, std::string>& parentMap);

    void resolve(const SVGDocumentContext& context) override;
};
#endif