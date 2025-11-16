#include "SVGDocument/SVG_H/SVGDocument.h"

SVGDocument::SVGDocument(std::string fileInput){
    this->fileName = fileInput;
    this->rootGroup = nullptr;
}

SVGDocument::~SVGDocument(){
    delete rootGroup;
    rootGroup = nullptr;
}

void SVGDocument::parseSVGImage(SVGParser& parser){
    this->rootGroup = parser.readXML(this->fileName);
}

void SVGDocument::renderSVGImage(SVGRenderer& render, Gdiplus::Graphics& graphics){
    render.renderFigure(graphics, this->rootGroup);
}