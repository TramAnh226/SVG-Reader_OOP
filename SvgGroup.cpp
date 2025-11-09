#include "SvgGroup.h"
#include "SvgFactoryPattern.h"

#include <algorithm>

SvgGroup::SvgGroup() : 
    SvgElement(), 
    ElementArray{}, 
    parent(nullptr){
}

SvgGroup::SvgGroup(const SvgGroup& other): SvgElement(other), parent(other.parent){
    SvgFactoryPattern factory;
    for(auto element: other.ElementArray){
        SvgElement* newElement = factory.getElement(element->getTagName());
        if(newElement){
            *newElement = *element;
            this->ElementArray.push_back(newElement);
        }
    }
}

void swap(SvgGroup& first, SvgGroup& second) noexcept{
    using std::swap;
    swap(first.ElementArray, second.ElementArray);
    swap(first.parent, second.parent);
}

SvgGroup& SvgGroup::operator= (SvgGroup other) noexcept{
    swap(*this, other);
    return *this;
}

SvgGroup::~SvgGroup(){
    for (auto element : ElementArray)
        delete element;
    ElementArray.clear();
}

void SvgGroup::setParent(SvgGroup* parent){
    this->parent = parent;
}

void SvgGroup::setElementArray(const std::vector<SvgElement*>& SvgElementArray){

    for (auto elem : ElementArray)
        delete elem;
    ElementArray.clear();

    SvgFactoryPattern factory;
    for(auto element: SvgElementArray){
        SvgElement* newElement = factory.getElement(element->getTagName());
        if(newElement){
            *newElement = *element;
            this->ElementArray.push_back(newElement);
        }
    }
}

void SvgGroup::addElement(SvgElement* ele){
    this->ElementArray.push_back(ele);
}

SvgGroup* SvgGroup::getParent(){
    return this->parent;
}

const std::vector<SvgElement*>& SvgGroup::getSvgElementArray() const {
    return this->ElementArray;
}

