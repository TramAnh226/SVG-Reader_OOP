#ifndef _SVGGROUP_H_
#define _SVGGROUP_H_


#include <vector>
#include <iostream>
#include "SVGElement.h"

class SVGParser;

class SVGGroup : public SVGElement{
    private:


    std::vector<SVGElement*> ElementArray;
    SVGGroup* parent;


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

    // them parse va render
    void parse(SVGParser&) const override;
    void render(SVGRenderer&) const override;

};


#endif
