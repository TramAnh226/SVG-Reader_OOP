#include "SVGRenderer.h"
#include <cmath>
#include <algorithm>
#include <cmath>

namespace Gdiplus {
    struct PointF;
    class Graphics;
    class SolidBrush;
    class Pen;
    class Color; 
}
using Gdiplus::PointF;
using Gdiplus::SolidBrush;
using Gdiplus::Pen;
using Gdiplus::Color;
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
void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect) {
    if (!rect) return;
    Gdiplus::Point p = rect->getTopLeftCorner();
    Gdiplus::PointF topLeft(static_cast<float>(p.X), static_cast<float>(p.Y));
    float w = rect->getWidth();
    float h = rect->getHeight();
    Gdiplus::SolidBrush brush(rect->getSVGStyle().getFillColor());
    Gdiplus::Pen pen(rect->getSVGStyle().getStroke().strokeColor, rect->getSVGStyle().getStroke().strokeWidth);
    g.FillRectangle(&brush, topLeft.X, topLeft.Y, w, h);    
    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, w, h);
}
void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle) {
    if (!circle) return;
    Gdiplus::PointF center = circle->getCenter();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
    float radius = circle->getRadius();
    Gdiplus::Color fill = circle->getSVGStyle().getFillColor();
    Gdiplus::Color stroke = circle->getSVGStyle().getStroke().strokeColor;
    float strokeW = circle->getSVGStyle().getStroke().strokeWidth;
    Gdiplus::SolidBrush brush(fill);
    Gdiplus::Pen pen(stroke, strokeW);
    g.FillEllipse(&brush, center.X - radius, center.Y - radius, radius * 2, radius * 2);
    g.DrawEllipse(&pen, center.X - radius, center.Y - radius, radius * 2, radius * 2);
}
void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse) {
    if (!ellipse) return;
    PointF center = ellipse->getCenter();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
    float rx = ellipse->getRadiusX();
    float ry = ellipse->getRadiusY();
    Gdiplus::SolidBrush brush(ellipse->getSVGStyle().getFillColor());
    Gdiplus::Pen pen(ellipse->getSVGStyle().getStroke().strokeColor, ellipse->getSVGStyle().getStroke().strokeWidth);
    g.FillEllipse(&brush, center.X - rx, center.Y - ry, rx * 2, ry * 2);
    g.DrawEllipse(&pen, center.X - rx, center.Y - ry, rx * 2, ry * 2);
}
void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* line) {
    if (!line) return;
    PointF start = line->getStartPoint();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
    PointF end = line->getEndPoint();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
    Gdiplus::Pen pen(line->getSVGStyle().getStroke().strokeColor, line->getSVGStyle().getStroke().strokeWidth);// trong SVGLine viết thêm getStrokeLine(), getStrokeColor
    g.DrawLine(&pen, start, end);
}
void SVGRenderer::renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon) {
    if (!polygon) return;
    const std::vector<CustomPoint>& customPoints = polygon->getPoints();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
    std::vector<PointF> gdiPoints;
    gdiPoints.reserve(customPoints.size());
    for (const auto& cp : customPoints) {
        gdiPoints.emplace_back(cp.x, cp.y);
    }
    if (gdiPoints.empty()) return;

    Gdiplus::SolidBrush brush(polygon->getSVGStyle().getFillColor()); //trong SVGLine viết thêm getFillColor(), 
    Gdiplus::Pen pen(polygon->getSVGStyle().getStroke().strokeColor, polygon->getSVGStyle().getStroke().strokeWidth);// getStrokeColor(), getStrokeWidth()
    g.FillPolygon(&brush, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
    g.DrawPolygon(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
}
void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline) {
    if (!polyline) return;

    std::vector<PointF> gdiPoints;
    const auto& customPoints = polyline->getPoints();// tương tự 
    for (const auto& cp : customPoints) {
        gdiPoints.emplace_back(cp.x, cp.y);
    }
    if (gdiPoints.size() < 2) return;
    Gdiplus::Pen pen(polyline->getSVGStyle().getStroke().strokeColor, polyline->getSVGStyle().getStroke().strokeWidth);// tương tự 
    g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
}
void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text) {
    if (!text) return;
    PointF pos = text->getStart();// tương tự 
    std::wstring content = text->getContent();
    Color color = text->getSVGStyle().getFillColor();// tương tự 
    float fontSize = text->getFontSize();
    Gdiplus::FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(color);
    g.DrawString(content.c_str(), -1, &font, pos, &brush);
}
void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* square) {
    if (!square) return;
    PointF topLeft = square->getTopLeftCorner();
    float side = square->getHeight();// tương tự 
    Gdiplus::SolidBrush brush(square->getSVGStyle().getFillColor());
    Gdiplus::Pen pen(square->getSVGStyle().getStroke().strokeColor, square->getSVGStyle().getStroke().strokeWidth);
    g.FillRectangle(&brush, topLeft.X, topLeft.Y, side, side);
    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, side, side);
}
void SVGRenderer::renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;
    rootGroup->render(*this, g);//hình như class SVGGroup chưa đc định nghĩa đầy đủ hay sao á, thử thêm virtual void render(SVGRenderer&) const = 0; vô thử 
}
// void renderGroup(Gdiplus::Graphics& g, const SVGGroup* group) {
//     //this->applyGroupContext(group);
//     // 2. Duyệt ĐỆ QUY (Composite Pattern)
//     const auto& children = group->getSVGElementArray();
//     for (SVGElement* element : children) {
//         // Gọi hàm render đa hình của TỪNG PHẦN TỬ CON
//         // Lệnh này sẽ gọi đúng hàm render() của Line, Circle, hoặc Group con
//         element->render(*this, g);
//     }
//     // 3. Khôi phục Context (nếu cần)
//     //this->restoreContext(group);
// }
void SVGRenderer::renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;

    const auto& children = rootGroup->getSVGElementArray();
    for (SVGElement* element : children) {
        // GỌI HÀM RENDER ĐA HÌNH
        // Vì SVGElement::render() trong SVGElement.h đã được sửa thành chỉ nhận SVGRenderer&,
        // bạn cần điều chỉnh cách gọi này. 
        
        // Ví dụ: element->render(*this); (Nếu bạn đổi lại chữ ký)

        // HOẶC nếu bạn dùng chữ ký cũ (render(SVGRenderer&, Gdiplus::Graphics&)):
        element->render(*this, g); 
    }
}

//render path
void SVGRenderer::renderPath(Gdiplus::Graphics& g, SVGPath* Path, vector<PathCommand> commands) const {
    using namespace Gdiplus;

    Pen pen(Color(255, 0, 0, 0), 2.0f); // Black pen with width 2.0f)
    PointF currentPoint(0.0f, 0.0f), startPoint(0.0f, 0.0f);

    for (const auto& cmd : commands) {
        switch (cmd.command) {
        case 'M': {
            currentPoint = PointF(cmd.parameters[0], cmd.parameters[1]);
            startPoint = currentPoint;
            break;
        }
        case 'L': {
            PointF p(cmd.parameters[0], cmd.parameters[1]);
            g.DrawLine(&pen, currentPoint, p);
            currentPoint = p;
            break;
        }
        case 'H': {
            float x = cmd.parameters[0];
            PointF p(x, currentPoint.Y);
            g.DrawLine(&pen, currentPoint, p);

            currentPoint = p;
            break;
        }
        case 'V': {
            float y = cmd.parameters[0];
            PointF p(currentPoint.X, y);
            g.DrawLine(&pen, currentPoint, p);
            currentPoint = p;
            break;
        }
        case 'Z': {
            g.DrawLine(&pen, currentPoint, startPoint);
            currentPoint = startPoint;
            break;
        }
        case 'C': {
            PointF p1(cmd.parameters[0], cmd.parameters[1]);
            PointF p2(cmd.parameters[2], cmd.parameters[3]);
            PointF p3(cmd.parameters[4], cmd.parameters[5]);

            g.DrawBezier(&pen, currentPoint, p1, p2, p3);
            currentPoint = p3;
            break;
        }
        }
    }
}
void SVGRenderer::drawCubicBezier(Gdiplus::Graphics& g,
    const CustomPoint& p0,
    const CustomPoint& p1,
    const CustomPoint& p2,
    const CustomPoint& p3) const
{
    using namespace Gdiplus;

    Pen pen(Color(255, 0, 0, 0), 2);

    PointF points[4] = {
        PointF(p0.x, p0.y),
        PointF(p1.x, p1.y),
        PointF(p2.x, p2.y),
        PointF(p3.x, p3.y)
    };

    g.DrawBeziers(&pen, points, 4);
}





/*
#include "SVGRenderer.h"
#include <sstream>
#include <algorithm>
#include <cmath>

// bởi vì không dùng using namespace Gdiplus; trong header nữa
namespace Gdiplus {
    struct PointF; // Forward declaration nếu cần
    class Graphics;
    class SolidBrush;
    class Pen;
    class Color; 
}

// Sửa lỗi: Đảm bảo Gdiplus::PointF được sử dụng đúng
using Gdiplus::PointF;
using Gdiplus::SolidBrush;
using Gdiplus::Pen;
using Gdiplus::Color;
// SVGRenderer::SVGRenderer() : zoom(1.0f), rotate(0.0f) {
// }
void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text) {
    if (!text) return;
    PointF pos = text->getStart();// tương tự 
    std::string content = text->getContent();
    Color color = text->getSVGStyle().getFillColor();// tương tự 
    float fontSize = text->getFontSize();
    Gdiplus::FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(color);
    g.DrawString(content.c_str(), -1, &font, pos, &brush);
}
void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* square) {
    if (!square) return;

    PointF topLeft = square->getTopLeftCorner();
    float side = square->getHeight();// tương tự 

    Gdiplus::SolidBrush brush(square->getSVGStyle().getFillColor());
    Gdiplus::Pen pen(square->getSVGStyle().getStroke().strokeColor, square->getSVGStyle().getStroke().strokeWidth);

    g.FillRectangle(&brush, topLeft.X, topLeft.Y, side, side);
    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, side, side);
}

void SVGRenderer::renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;
    rootGroup->render(*this, g);//hình như class SVGGroup chưa đc định nghĩa đầy đủ hay sao á, thử thêm virtual void render(SVGRenderer&) const = 0; vô thử 
}

void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect) {
    if (!rect) return;

    // LƯU Ý: Phải có hàm chuyển đổi CustomPoint sang Gdiplus::Point/PointF
    // Giả định getTopLeftCorner() trả về CustomPoint có thể ép kiểu/convert
    CustomPoint cp = rect->getTopLeftCorner();
    Gdiplus::PointF topLeft(cp.x, cp.y); 

    float w = rect->getWidth();
    float h = rect->getHeight();

    // Sử dụng Gdiplus::SolidBrush và Gdiplus::Pen
    Gdiplus::SolidBrush brush(rect->getSVGStyle().getFillColor()); 
    Gdiplus::Pen pen(rect->getSVGStyle().getStroke().strokeColor, rect->getSVGStyle().getStroke().strokeWidth);

    g.FillRectangle(&brush, topLeft.X, topLeft.Y, w, h); 
    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, w, h);
}

void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle) {
    if (!circle) return;

    CustomPoint centerCp = circle->getCenter();
    PointF center(centerCp.x, centerCp.y); 
    float radius = circle->getRadius();

    SolidBrush brush(circle->getSVGStyle().getFillColor());
    Pen pen(circle->getSVGStyle().getStroke().strokeColor, circle->getSVGStyle().getStroke().strokeWidth);

    float diameter = radius * 2;
    g.FillEllipse(&brush, center.X - radius, center.Y - radius, diameter, diameter);
    g.DrawEllipse(&pen, center.X - radius, center.Y - radius, diameter, diameter);
}

void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse) {
    if (!ellipse) return;

    CustomPoint centerCp = ellipse->getCenter();
    PointF center(centerCp.x, centerCp.y); 
    float rx = ellipse->getRadiusX();
    float ry = ellipse->getRadiusY();

    SolidBrush brush(ellipse->getSVGStyle().getFillColor());
    Pen pen(ellipse->getSVGStyle().getStroke().strokeColor, ellipse->getSVGStyle().getStroke().strokeWidth);

    g.FillEllipse(&brush, center.X - rx, center.Y - ry, rx * 2, ry * 2);
    g.DrawEllipse(&pen, center.X - rx, center.Y - ry, rx * 2, ry * 2);
}

void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* line) {
    if (!line) return;

    PointF start(line->getStartPoint().x, line->getStartPoint().y);
    PointF end(line->getEndPoint().x, line->getEndPoint().y);

    Pen pen(line->getSVGStyle().getStroke().strokeColor, line->getSVGStyle().getStroke().strokeWidth);
    g.DrawLine(&pen, start, end);
}

void SVGRenderer::renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon) {
    if (!polygon) return;

    // LƯU Ý: Nếu getPoints() trả về vector<CustomPoint>, 
    // bạn cần chuyển đổi sang std::vector<Gdiplus::PointF>
    
    // Ví dụ về chuyển đổi:
    std::vector<PointF> gdiPoints;
    const auto& customPoints = polygon->getPoints();
    for (const auto& cp : customPoints) {
        gdiPoints.emplace_back(cp.x, cp.y);
    }
    if (gdiPoints.empty()) return;

    SolidBrush brush(polygon->getSVGStyle().getFillColor());
    Pen pen(polygon->getSVGStyle().getStroke().strokeColor, polygon->getSVGStyle().getStroke().strokeWidth);

    g.FillPolygon(&brush, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
    g.DrawPolygon(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
}

// Hàm renderGroup sẽ gọi các hàm render đa hình của từng Element
void SVGRenderer::renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;

    const auto& children = rootGroup->getSVGElementArray();
    for (SVGElement* element : children) {
        // GỌI HÀM RENDER ĐA HÌNH
        // Vì SVGElement::render() trong SVGElement.h đã được sửa thành chỉ nhận SVGRenderer&,
        // bạn cần điều chỉnh cách gọi này. 
        
        // Ví dụ: element->render(*this); (Nếu bạn đổi lại chữ ký)

        // HOẶC nếu bạn dùng chữ ký cũ (render(SVGRenderer&, Gdiplus::Graphics&)):
        element->render(*this, g); 
    }
}
*/