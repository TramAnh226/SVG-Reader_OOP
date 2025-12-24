#ifndef SVGPATH_H
#define SVGPATH_H

#include <windows.h> 
#include <gdiplus.h>
#include "SVGElement.h"
#include <vector>

using namespace std;

class SVGParser;
class SVGRenderer;

enum class PathCommandType {
    MoveTo,
    LineTo,
    HLineTo,
    VLineTo,
    ClosePath,
    CubicBezier,
    SmoothCubicBezier,
    Arc
};

struct PathCommand {
    PathCommandType type;
    vector<float> params;
};

class SVGPath : public SVGElement {
private:
    vector<PathCommand> commands;

public:
    SVGPath();
    SVGElement* clone() const override;

    void parse(SVGParser&, tinyxml2::XMLElement*) override;
    void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;

    void setCommands(const PathCommand& pc);
    const vector<PathCommand>& getCommands() const;
};
#endif