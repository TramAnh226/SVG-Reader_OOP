//#pragma once
//#include <string>
//#include <unordered_map>
////#include "SVGGradient.h" 
//
//class SVGGradient;
//
//class SVGDocumentContext {
////private:
//public:
//    std::unordered_map<std::string, SVGGradient*> gradientDefinitions;
////public:
//    SVGDocumentContext();
//    SVGDocumentContext(const SVGDocumentContext& other);
//    SVGDocumentContext& operator=(const SVGDocumentContext& other);
//    ~SVGDocumentContext();
//
//    void addGradient(SVGGradient* gradient);
//
//    const SVGGradient* getGradientById(const std::string& id) const;
//
//    void clear();
//};
#pragma once
#include <string>
#include <unordered_map>
//#include "SVGGradient.h" 

class SVGGradient;

class SVGDocumentContext {
    //private:
public:
    std::unordered_map<std::string, SVGGradient*> gradientDefinitions;
    //public:
    SVGDocumentContext();
    SVGDocumentContext(const SVGDocumentContext& other);
    SVGDocumentContext& operator=(const SVGDocumentContext& other);
    ~SVGDocumentContext();

    void addGradient(SVGGradient* gradient);

    const SVGGradient* getGradientById(const std::string& id) const;

    void clear();
};