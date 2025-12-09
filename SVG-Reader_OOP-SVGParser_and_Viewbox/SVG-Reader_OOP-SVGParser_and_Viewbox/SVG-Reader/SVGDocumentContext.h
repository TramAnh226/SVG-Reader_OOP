#pragma once
#include <string>
#include <unordered_map>
#include "SVGGradient.h" 

class SVGDocumentContext {
private:
    std::unordered_map<std::string, SVGGradient*> gradientDefinitions;
public:
    SVGDocumentContext();
    SVGDocumentContext(const SVGDocumentContext& other);
    SVGDocumentContext& operator=(const SVGDocumentContext& other);
    ~SVGDocumentContext();

    void addGradient(SVGGradient* gradient);

    const SVGGradient* getGradientById(const std::string& id) const;

    void clear();
};