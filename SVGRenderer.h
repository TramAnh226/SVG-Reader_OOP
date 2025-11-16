#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <utility>  
using namespace Gdiplus;
using namespace std;

class SVGRectangle;
class SVGCircle;
class SVGEllipse;
class SVGLine;
class SVGPolygon;
class SVGPolyline;
class SVGText;

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

    void drawRectangle(Graphics& g, const SVGRectangle* rect);
    void drawCircle(Graphics& g, const SVGCircle* circle);
    void drawEllipse(Graphics& g, const SVGEllipse* ellipse);
    void drawLine(Graphics& g, const SVGLine* line);
    void drawPolygon(Graphics& g, const SVGPolygon* polygon);
    void drawPolyline(Graphics& g, const SVGPolyline* polyline);
    void drawText(Graphics& g, const SVGText* text);
    void renderFigure(Graphics& g, const std::vector<SVGElement*>& group);
};
