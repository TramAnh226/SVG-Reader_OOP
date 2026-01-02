#ifndef SVGRENDERER_H
#define SVGRENDERER_H

#include <windows.h>
#include <gdiplus.h>
#include <algorithm>
#include <iostream>
#include <cmath>
#include "SVGPath.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class CustomPoint;
class SVGRectangle;
class SVGSquare;
class SVGCircle;
class SVGEllipse;
class SVGText;
class SVGLine;
class SVGPolygon;
class SVGPolyline;
class SVGGroup;
class SVGLinearGradient;
class SVGRadialGradient;

class SVGRenderer {
private:
    float zoom;
    float rotate;


public:
    SVGRenderer();
    ~SVGRenderer() = default;

    //void setZoom(double z);
    //void setRotation(double deg);

    void renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect, const SVGDocumentContext& context);
    void renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle, const SVGDocumentContext& context);
    void renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse, const SVGDocumentContext& context);
    void renderLine(Gdiplus::Graphics& g, const SVGLine* line, const SVGDocumentContext& context);
    void renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon, const SVGDocumentContext& context);
    void renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline, const SVGDocumentContext& context);
    void renderText(Gdiplus::Graphics& g, const SVGText* text, const SVGDocumentContext& context);
    void renderSquare(Gdiplus::Graphics& g, const SVGSquare* square, const SVGDocumentContext& context);
    void renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup, const SVGDocumentContext& context);
    void renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup, const SVGDocumentContext& context);

    //path
    void renderGradientPathFill(Gdiplus::Graphics& g, const SVGGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds, const SVGDocumentContext& context) const;
    void renderPath(Gdiplus::Graphics&, const SVGPath*, const SVGDocumentContext&) const;
    void drawCubicBezier(Gdiplus::Graphics&, const CustomPoint&, const CustomPoint&, const CustomPoint&, const CustomPoint&) const;
    void addSvgArcToPath(Gdiplus::GraphicsPath& path, Gdiplus::PointF start, Gdiplus::PointF end, float rx, float ry, float rot, bool largeArc, bool sweep) const;

    Gdiplus::WrapMode getGdiWrapMode(const std::string& spreadMethod) const;
    void renderLinearGradientFill(Gdiplus::Graphics& g, const SVGLinearGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const;
    void renderRadialObjectBoundingBox(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const;
    void renderRadialUserSpace(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const;
    void renderRadialGradientFill(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const;
    void renderGradientFill(Gdiplus::Graphics& g, const SVGGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds, const SVGDocumentContext& context);
};
#endif