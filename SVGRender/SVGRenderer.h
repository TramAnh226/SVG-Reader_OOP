#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <utility>  
#include "Library.h"
using namespace Gdiplus;
using namespace std;

class SVGRenderer {
private:
    float zoom;
    float rotate;

public:
    SVGRenderer();
    ~SVGRenderer() = default;

    //void setZoom(double z);
    //void setRotation(double deg);

    //pair<double, double> transformPoint(double x, double y) const;

    void renderRectangle(Graphics& g, const SVGRectangle* rect);
    void renderCircle(Graphics& g, const SVGCircle* circle);
    void renderEllipse(Graphics& g, const SVGEllipse* ellipse);
    void renderLine(Graphics& g, const SVGLine* line);
    void renderPolygon(Graphics& g, const SVGPolygon* polygon);
    void renderPolyline(Graphics& g, const SVGPolyline* polyline);
    void renderText(Graphics& g, const SVGText* text);
    void renderSquare(Graphics& g, const SVGSquare* square);
    void renderFigure(Graphics& g, const SVGGroup* rootGroup);
    void renderGroup(const SVGGroup& group);
};