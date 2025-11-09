#ifndef _SVGFACTORYPATTERN_H_
#define _SVGFACTORYPATTERN_H

#include <string>
#include <unordered_map>
#include "SvgElement.h"

class SvgFactoryPattern{
    private:

    std::unordered_map<std::string, int> ElementID;

    public:

    SvgFactoryPattern();
    SvgElement* getElement(std::string tagname);
    const std::unordered_map<std::string, int>& getElementID() const;

};

#endif