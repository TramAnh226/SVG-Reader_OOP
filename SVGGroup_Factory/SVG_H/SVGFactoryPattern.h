#ifndef _SVGFACTORYPATTERN_H_
#define _SVGFACTORYPATTERN_H


#include <string>
#include <unordered_map>
#include "../../SVGElement/SVG_H/SVGElement.h"

class SVGFactoryPattern{
    private:


    std::unordered_map<std::string, int> ElementID;


    public:


    SVGFactoryPattern();
    SVGElement* getElement(std::string tagname);
    const std::unordered_map<std::string, int>& getElementID() const;


};


#endif
