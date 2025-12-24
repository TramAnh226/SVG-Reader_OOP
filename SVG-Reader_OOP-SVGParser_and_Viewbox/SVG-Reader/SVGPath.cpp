#include "SVGPath.h"
#include "SVGParser.h"
#include "SVGRenderer.h"
#include "tinyxml2.h"
#include <cctype>
#include <sstream>
#include <vector>

SVGPath::SVGPath() : SVGElement() {
    this->setTagName("path");
}

SVGElement* SVGPath::clone() const {
    return new SVGPath(*this);
}
void SVGPath::setCommands(const PathCommand& pc) {
    this->commands.push_back(pc);
}
const vector<PathCommand>& SVGPath::getCommands() const {
    return commands;
}
void SVGPath::parse(SVGParser& parser, tinyxml2::XMLElement* xmlNode) {
    SVGElement::parse(parser, xmlNode);
    parser.parsePath(this, xmlNode);
}

void SVGPath::render(SVGRenderer& r, Gdiplus::Graphics& g, const SVGDocumentContext& context) const {
    r.renderPath(g, this, context);
}




