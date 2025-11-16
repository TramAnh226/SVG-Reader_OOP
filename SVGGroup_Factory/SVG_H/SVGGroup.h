#ifndef _SVGGROUP_H_
#define _SVGGROUP_H_


#include <vector>
#include <iostream>
#include "SVGElement/SVG_H/SVGElement.h"
#include "SVGParser/SVGParser/SVGParser.h"


class SVGGroup : public SVGElement{
    private:


    std::vector<SVGElement*> ElementArray;
    SVGGroup* parent;

    // float width = 0.0f;     // Mặc định
    // float height = 0.0f;    // Mặc định
    // std::string viewBox;    // Mặc định là chuỗi rỗng
    // std::string transform;  // Mặc định là chuỗi rỗng

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

    void parse(SVGParser&) override;
	void render(SVGRenderer&) override;

    // void setWidth(float w) { width = w; }
    // void setHeight(float h) { height = h; }
    // void setViewBox(const std::string& vb) { viewBox = vb; }
    // void setTransform(const std::string& t) { transform = t; }
};


#endif
