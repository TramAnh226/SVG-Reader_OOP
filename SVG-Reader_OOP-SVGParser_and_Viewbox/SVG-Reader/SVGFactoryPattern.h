#ifndef SVGFACTORYPATTERN_H
#define SVGFACTORYPATTERN_H

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>

class SVGElement;
class SVGGradient;

class SVGFactoryPattern {
private:
    std::unordered_map<std::string, int> ElementID;
public:
    SVGFactoryPattern();
    SVGElement* getElement(std::string tagname);
    const std::unordered_map<std::string, int>& getElementID() const;

    SVGGradient* getGradientType(const std::string& tagName);
};
#endif

