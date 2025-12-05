#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <utility>  
#include "../SVGElement/SVG_H/Library.h"
#include "../SVGElement/SVG_H/SVGRectangle.h"
#include "../SVGElement/SVG_H/SVGCircle.h"
#include "../SVGElement/SVG_H/SVGEllipse.h" 
#include "../SVGElement/SVG_H/SVGLine.h"
#include "../SVGElement/SVG_H/SVGPolygon.h"
#include "../SVGElement/SVG_H/SVGPolyline.h"
#include "../SVGElement/SVG_H/SVGText.h"
#include "../SVGElement/SVG_H/SVGSquare.h"
#include "../SVGGroup_Factory/SVG_H/SVGGroup.h"
#include "../SVGElement/SVG_H/SVGPath.h"

// using namespace Gdiplus;
// using namespace std;

class SVGGroup;

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

    void renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect);
    void renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle);
    void renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse);
    void renderLine(Gdiplus::Graphics& g, const SVGLine* line);
    void renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon);
    void renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline);
    void renderText(Gdiplus::Graphics& g, const SVGText* text);
    void renderSquare(Gdiplus::Graphics& g, const SVGSquare* square);
    void renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup);
    void renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup);

    //path
    void renderPath(Gdiplus::Graphics& g, SVGPath* path, vector<PathCommand>) const;
    void drawCubicBezier(Gdiplus::Graphics&, const CustomPoint&, const CustomPoint&, const CustomPoint&, const CustomPoint&) const;
};