#ifndef SVGDOCUMENT_H
#define SVGDOCUMENT_H

#include "SVGDocumentContext.h"

class SVGGroup;
class SVGParser;
class SVGRenderer;
class SVGFactoryPattern;

namespace Gdiplus {
    class Graphics;
}

class SVGDocument {
private:
    std::string fileName;
    SVGGroup* rootGroup;

public:
    SVGDocumentContext context;

    SVGDocument(std::string);
    ~SVGDocument();

    SVGGroup* getRootGroup();
    void parseSVGImage(SVGParser&, SVGFactoryPattern&);
    void renderSVGImage(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&);

    void resolveReferences();
};
#endif