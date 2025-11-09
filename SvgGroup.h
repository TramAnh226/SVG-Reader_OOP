#ifndef _SVGGROUP_H_
#define _SVGGROUP_H_

#include <vector>
#include <iostream>
#include "SvgElement.h"

class SvgGroup : public SvgElement{
    private:

    std::vector<SvgElement*> ElementArray;
    SvgGroup* parent;

    public:

    SvgGroup();
    SvgGroup(const SvgGroup&);
    SvgGroup& operator= (SvgGroup) noexcept;
    ~SvgGroup();

    friend void swap(SvgGroup& first, SvgGroup& second) noexcept;

    void setParent(SvgGroup*);
    void setElementArray(const std::vector<SvgElement*>&);
    void addElement(SvgElement*);

    SvgGroup* getParent();
    const std::vector<SvgElement*>& getSvgElementArray() const;
};

#endif