#ifndef DOCUMENTCONTEXT_H
#define DOCUMENTCONTEXT_H

#include <string>
#include <unordered_map>

class SVGGradient;

class SVGDocumentContext {
public:
    std::unordered_map<std::string, SVGGradient*> gradientDefinitions;

    SVGDocumentContext();
    SVGDocumentContext(const SVGDocumentContext&);
    SVGDocumentContext& operator=(const SVGDocumentContext&);
    ~SVGDocumentContext();

    void addGradient(SVGGradient* gradient);

    const SVGGradient* getGradientById(const std::string&) const;

    void clear();
};
#endif