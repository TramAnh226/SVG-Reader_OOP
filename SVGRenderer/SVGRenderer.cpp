#include "SVGRenderer.h"
//#include "SVGGroup.h"// có thể trong SVGGroup.h include mấy thư viện khác chưa được, chổ SVGElement, getSVGElementArray() xem thử
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

void SVGRenderer::renderRectangle(Graphics& g, const SVGRectangle* rect) {
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


void SVGRenderer::renderCircle(Graphics& g, const SVGCircle* circle) {
    if (!circle) return;

    Gdiplus::PointF center = circle->getCenter();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
    float radius = circle->getRadius();
    Gdiplus::Color fill = circle->getFillColor();
    Gdiplus::Color stroke = circle->getStrokeColor();
    float strokeW = circle->getStrokeWidth();
    SolidBrush brush(fill);
    Pen pen(stroke, strokeW);
    g.FillEllipse(&brush, center.X - radius, center.Y - radius, radius * 2, radius * 2);
    g.DrawEllipse(&pen, center.X - radius, center.Y - radius, radius * 2, radius * 2);
}

void SVGRenderer::renderEllipse(Graphics& g, const SVGEllipse* ellipse) {
    if (!ellipse) return;

    PointF center = ellipse->getCenter();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
    float rx = ellipse->getRadiusX();
    float ry = ellipse->getRadiusY();

    SolidBrush brush(ellipse->getFillColor());
    Pen pen(ellipse->getStrokeColor(), ellipse->getStrokeWidth());

    g.FillEllipse(&brush, center.X - rx, center.Y - ry, rx * 2, ry * 2);
    g.DrawEllipse(&pen, center.X - rx, center.Y - ry, rx * 2, ry * 2);
}

void SVGRenderer::renderLine(Graphics& g, const SVGLine* line) {
    if (!line) return;

    PointF start = line->getStartPoint();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
    PointF end = line->getEndPoint();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF

    Pen pen(line->getStrokeColor(), line->getStrokeWidth());// trong SVGLine viết thêm getStrokeLine(), getStrokeColor
    g.DrawLine(&pen, start, end);
}


void SVGRenderer::renderPolygon(Graphics& g, const SVGPolygon* polygon) {
    if (!polygon) return;

    const std::vector<PointF>& points = polygon->getPoints();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
    if (points.empty()) return;

    SolidBrush brush(polygon->getFillColor()); //trong SVGLine viết thêm getFillColor(), 
    Pen pen(polygon->getStrokeColor(), polygon->getStrokeWidth());// getStrokeColor(), getStrokeWidth()

    g.FillPolygon(&brush, points.data(), static_cast<INT>(points.size()));
    g.DrawPolygon(&pen, points.data(), static_cast<INT>(points.size()));
}

void SVGRenderer::renderPolyline(Graphics& g, const SVGPolyline* polyline) {
    if (!polyline) return;

    const std::vector<PointF>& points = polyline->getPoints();// tương tự 
    if (points.size() < 2) return;

    Pen pen(polyline->getStrokeColor(), polyline->getStrokeWidth());// tương tự 
    g.DrawLines(&pen, points.data(), static_cast<INT>(points.size()));
}

void SVGRenderer::renderText(Graphics& g, const SVGText* text) {
    if (!text) return;

    PointF pos = text->getPosition();// tương tự 
    std::wstring content = text->getContent();
    Color color = text->getFillColor();// tương tự 
    float fontSize = text->getFontSize();

    FontFamily fontFamily(L"Arial");
    Font font(&fontFamily, fontSize, FontStyleRegular, UnitPixel);
    SolidBrush brush(color);

    g.DrawString(content.c_str(), -1, &font, pos, &brush);
}

void SVGRenderer::renderSquare(Graphics& g, const SVGSquare* square) {
    if (!square) return;

    PointF topLeft = square->getTopLeftCorner();
    float side = square->getSideLength();// tương tự 

    SolidBrush brush(square->getFillColor());
    Pen pen(square->getStrokeColor(), square->getStrokeWidth());

    g.FillRectangle(&brush, topLeft.X, topLeft.Y, side, side);
    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, side, side);
}

void SVGRenderer::renderFigure(Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;
    rootGroup->render(*this);//hình như class SVGGroup chưa đc định nghĩa đầy đủ hay sao á, thử thêm virtual void render(SVGRenderer&) const = 0; vô thử 
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
