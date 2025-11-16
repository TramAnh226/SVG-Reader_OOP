#ifndef _SVG_DOCUMENT_H_
#define _SVG_DOCUMENT_H_

#include "SVGGroup_Factory/SVG_H/SVGGroup.h"

class SVGDocument{
    private:

    std::string fileName;
    SVGGroup* rootGroup;

    public:

    SVGDocument(std::string);
    ~SVGDocument();

    void parseSVGImage(SVGParser&);
    void renderSVGImage(SVGRenderer&, Graphics&);
};

#endif