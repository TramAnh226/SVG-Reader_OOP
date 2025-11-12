#include "SVGRenderer.h"
#include <cmath>
#include "SVGElement.h"
#include "Library.h"
#include <cmath>

SVGRenderer::SVGRenderer() : zoom(1.0f), rotate(0.0f) {}

//void SVGRenderer::setZoom(double z) {
//    zoom = static_cast<float>(z);
//}
//
//void SVGRenderer::setRotation(double deg) {
//    rotate = static_cast<float>(deg);
//}

//pair<double, double> SVGRenderer::transformPoint(double x, double y) const {
//    double rad = rotate * M_PI / 180.0;
//    double newX = (x * cos(rad) - y * sin(rad)) * zoom;
//    double newY = (x * sin(rad) + y * cos(rad)) * zoom;
//    return { newX, newY };
//}

void SVGRenderer::drawRectangle(Graphics& g, const SVGRectangle* rect) {
    if (!rect) return;

    Gdiplus::Point p = rect->getTopLeftCorner();
    Gdiplus::PointF topLeft(static_cast<float>(p.X), static_cast<float>(p.Y));

    float w = rect->getWidth();
    float h = rect->getHeight();

    SolidBrush brush(rect->getFillColor());
    Pen pen(rect->getStrokeColor(), rect->getStrokeWidth());

    g.FillRectangle(&brush, topLeft.X, topLeft.Y, w, h);    
    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, w, h);
}


void SVGRenderer::drawCircle(Graphics& g, const SVGCircle* circle) {
    if (!circle) return;
    PointF center = circle->getCenter();
    float radius = circle->getRadius();
    Gdiplus::Color fill = circle->getFillColor();
    Gdiplus::Color stroke = circle->getStrokeColor();
    float strokeW = circle->getStrokeWidth();
    SolidBrush brush(fill);
    Pen pen(stroke, strokeW);
    g.FillEllipse(&brush, center.X - radius, center.Y - radius, radius * 2, radius * 2);
    g.DrawEllipse(&pen, center.X - radius, center.Y - radius, radius * 2, radius * 2);
}


void SVGRenderer::drawEllipse(Graphics& g, const SVGEllipse* ellipse) {
    if (!ellipse) return;
    PointF center = ellipse->getCenter();
    float rx = ellipse->getRadiusX();
    float ry = ellipse->getRadiusY();
    SolidBrush brush(ellipse->getFillColor());
    Pen pen(ellipse->getStrokeColor(), ellipse->getStrokeWidth());
    g.FillEllipse(&brush, center.X - rx, center.Y - ry, rx * 2, ry * 2);
    g.DrawEllipse(&pen, center.X - rx, center.Y - ry, rx * 2, ry * 2);
}

void SVGRenderer::drawLine(Graphics& g, const SVGLine* line) {
    if (!line) return;
    PointF start = line->getStartPoint();
    PointF end = line->getEndPoint();
    Pen pen(line->getStrokeColor(), line->getStrokeWidth());
    g.DrawLine(&pen, start, end);
}


void SVGRenderer::drawPolygon(Graphics& g, const SVGPolygon* polygon) {
    if (!polygon) return;
    const std::vector<PointF>& points = polygon->getPoints();
    if (points.empty()) return;
    SolidBrush brush(polygon->getFillColor());
    Pen pen(polygon->getStrokeColor(), polygon->getStrokeWidth());
    g.FillPolygon(&brush, points.data(), static_cast<INT>(points.size()));
    g.DrawPolygon(&pen, points.data(), static_cast<INT>(points.size()));
}

void SVGRenderer::drawPolyline(Graphics& g, const SVGPolyline* polyline) {
    if (!polyline) return;
    const std::vector<PointF>& points = polyline->getPoints();
    if (points.size() < 2) return;
    Pen pen(polyline->getStrokeColor(), polyline->getStrokeWidth());
    g.DrawLines(&pen, points.data(), static_cast<INT>(points.size()));
}

void SVGRenderer::drawText(Graphics& g, const SVGText* text) {
    if (!text) return;
    PointF pos = text->getPosition();
    std::wstring content = text->getContent();
    Gdiplus::Color color = text->getFillColor();
    float fontSize = text->getFontSize();
    FontFamily fontFamily(L"Arial");
    Font font(&fontFamily, fontSize, FontStyleRegular, UnitPixel);
    SolidBrush brush(color);
    g.DrawString(content.c_str(), -1, &font, pos, &brush);
}

void SVGRenderer::renderFigure(Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;
    rootGroup->render(*this);
}

void SVGRenderer::renderGroup(const SVGGroup& group) {
    //this->applyGroupContext(group);

    // 2. Duyệt ĐỆ QUY (Composite Pattern)
    const auto& children = group.getSVGElementArray();
    for (SVGElement* element : children) {
        // Gọi hàm render đa hình của TỪNG PHẦN TỬ CON
        // Lệnh này sẽ gọi đúng hàm render() của Line, Circle, hoặc Group con
        element->render(*this);
    }
    // 3. Khôi phục Context (nếu cần)
    //this->restoreContext(group);
}


