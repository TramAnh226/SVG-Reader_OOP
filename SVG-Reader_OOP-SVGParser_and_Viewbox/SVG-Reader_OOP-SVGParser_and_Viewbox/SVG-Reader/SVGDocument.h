#pragma once
#include "SVGGroup.h"
#include "SVGDocumentContext.h"
//#include "Library.h"

class SVGParser;
class SVGRenderer;

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
