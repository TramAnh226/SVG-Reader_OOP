#ifndef DOCUMENTCONTEXT_H
#define DOCUMENTCONTEXT_H

#include <string>
#include <unordered_map>

class SVGGradient;

class SVGDocumentContext {
public:
    std::unordered_map<std::string, SVGGradient*> gradientDefinitions;

    SVGDocumentContext();
    SVGDocumentContext(const SVGDocumentContext& other);
    SVGDocumentContext& operator=(const SVGDocumentContext& other);
    ~SVGDocumentContext();

    void addGradient(SVGGradient* gradient);

    const SVGGradient* getGradientById(const std::string& id) const;

    void clear();
};
#endif