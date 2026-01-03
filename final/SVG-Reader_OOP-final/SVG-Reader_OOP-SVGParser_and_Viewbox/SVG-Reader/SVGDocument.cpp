#include "SVGDocument.h"
#include "SVGGroup.h"
#include "SVGParser.h"
#include "SVGRenderer.h"
#include "SVGFactoryPattern.h"
#include "SVGDocumentContext.h"

SVGDocument::SVGDocument(std::string fileInput) {
    this->fileName = fileInput;
    this->rootGroup = nullptr;
}

SVGDocument::~SVGDocument() {
    delete rootGroup;
    rootGroup = nullptr;
}
SVGGroup* SVGDocument::getRootGroup() {
    return this->rootGroup;
}
void SVGDocument::parseSVGImage(SVGParser& parser, SVGFactoryPattern& factory) {
    // 1. Dọn dẹp Root Group và Context cũ (nếu có)
    if (this->rootGroup) {
        delete this->rootGroup;
        this->rootGroup = nullptr;
    }
    this->context.clear();

    // 2. Gọi parser (Bắt đầu Phase 1 Parsing)
    this->rootGroup = parser.readXML(this->fileName, factory, this->context);
}

void SVGDocument::renderSVGImage(SVGRenderer& render, Gdiplus::Graphics& graphics, const SVGDocumentContext& context) {
    render.renderFigure(graphics, this->rootGroup, context);
}

void SVGDocument::resolveReferences() {
    // Bắt đầu quá trình đệ quy từ Root Group và truyền Context xuống
    if (this->rootGroup) {
        this->rootGroup->resolve(this->context);
    }
}