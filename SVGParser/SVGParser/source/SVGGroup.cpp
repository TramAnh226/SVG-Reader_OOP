#include "SVGGroup.h"
#include "SVGFactoryPattern.h"

#include <algorithm>


SVGGroup::SVGGroup() :
    SVGElement(),
    ElementArray{},
    parent(nullptr) {
}


SVGGroup::SVGGroup(const SVGGroup& other) : SVGElement(other.getTagName(), other.getId(), other.getSVGStyle()), parent(other.parent) {
    for (auto element : other.ElementArray) {
        SVGElement* newElement = element->clone();
        if (newElement) {
            this->ElementArray.push_back(newElement);
        }
    }
}


void swap(SVGGroup& first, SVGGroup& second) noexcept {
    using std::swap;
    swap(first.ElementArray, second.ElementArray);
    swap(first.parent, second.parent);
}


SVGGroup& SVGGroup::operator= (SVGGroup other) noexcept {
    swap(*this, other);
    return *this;
}


SVGGroup::~SVGGroup() {
    for (auto element : ElementArray)
        delete element;
    ElementArray.clear();
}


void SVGGroup::setParent(SVGGroup* parent) {
    this->parent = parent;
}


void SVGGroup::setElementArray(const std::vector<SVGElement*>& SVGElementArray) {


    for (auto elem : ElementArray)
        delete elem;
    ElementArray.clear();


    for (auto element : SVGElementArray) {
        SVGElement* newElement = element->clone();
        if (newElement) {
            this->ElementArray.push_back(newElement);
        }
    }
}


void SVGGroup::addElement(SVGElement* ele) {
    this->ElementArray.push_back(ele);
}


SVGGroup* SVGGroup::getParent() {
    return this->parent;
}


const std::vector<SVGElement*>& SVGGroup::getSVGElementArray() const {
    return this->ElementArray;
}

SVGElement* SVGGroup::clone() const {
    return new SVGGroup(*this);
}


void SVGGroup::parse(SVGParser& parser) {
    parser.parseGroup(this, xmlNode);
}

void SVGGroup::render(SVGRenderer& r) {
    r.renderGroup(*this);
}
