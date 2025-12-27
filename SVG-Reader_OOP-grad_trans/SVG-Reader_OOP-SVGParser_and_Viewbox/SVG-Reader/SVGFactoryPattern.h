//#pragma once
//#include <string>
//#include <unordered_map>
//#include <stdexcept>
//#include <algorithm>
////#include "SVGElement.h"
////#include "SVGLinearGradient.h" 
////#include "SVGRadialGradient.h"
////#include "SVGPath.h"
//
//class SVGElement;
//class SVGGradient;
//
//class SVGFactoryPattern {
//private:
//    std::unordered_map<std::string, int> ElementID;
//public:
//    SVGFactoryPattern();
//    SVGElement* getElement(std::string tagname);
//    const std::unordered_map<std::string, int>& getElementID() const;
//
//    SVGGradient* getGradientType(const std::string& tagName);
//};
//
#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
//#include "SVGElement.h"
//#include "SVGLinearGradient.h" 
//#include "SVGRadialGradient.h"
//#include "SVGPath.h"

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

