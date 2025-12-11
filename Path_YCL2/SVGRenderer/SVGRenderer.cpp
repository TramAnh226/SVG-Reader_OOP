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

    // Save transform
    Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    // Apply path transform
    if (Path && Path->transform) {
        g.MultiplyTransform(Path->transform, MatrixOrderPrepend);
    }

    GraphicsPath gp;

    // Apply fill-rule
    if (Path->getSVGStyle().fillRule == FillRule::EvenOdd)
        gp.SetFillMode(FillModeAlternate); // EvenOdd
    else
        gp.SetFillMode(FillModeWinding);   // NonZero

    PointF currentPoint(0, 0), startPoint(0, 0);

    for (const auto& cmd : commands) {
        switch (cmd.command) {

        case 'M': {
            currentPoint = PointF(cmd.parameters[0], cmd.parameters[1]);
            startPoint = currentPoint;
            break;
        }
        case 'L': {
            PointF p(cmd.parameters[0], cmd.parameters[1]);
            gp.AddLine(currentPoint, p);
            currentPoint = p;
            break;
        }
        case 'H': {
            float x = cmd.parameters[0];
            PointF p(x, currentPoint.Y);
            gp.AddLine(currentPoint, p);
            currentPoint = p;
            break;
        }
        case 'V': {
            float y = cmd.parameters[0];
            PointF p(currentPoint.X, y);
            gp.AddLine(currentPoint, p);
            currentPoint = p;
            break;
        }
        case 'C': {
            PointF p1(cmd.parameters[0], cmd.parameters[1]);
            PointF p2(cmd.parameters[2], cmd.parameters[3]);
            PointF p3(cmd.parameters[4], cmd.parameters[5]);

            gp.AddBezier(currentPoint, p1, p2, p3);
            currentPoint = p3;
            break;
        }
        case 'Z': {
            gp.AddLine(currentPoint, startPoint);
            currentPoint = startPoint;
            break;
        }

        }
    }

    // ----- FILL -----
    if (Path->getSVGStyle().fillColor.a > 0) {
        Gdiplus::Color gdiFillColor = Path->getSVGStyle().getGdiFillColor();
        SolidBrush brush(gdiFillColor);
        g.FillPath(&brush, &gp);
    }

    // ----- STROKE -----
    if (Path->getSVGStyle().stroke.width > 0) {

        Pen pen(Path->getSVGStyle().stroke.color.toGDI(), Path->getSVGStyle().stroke.width);

        // linejoin
        /*switch (Path->style.stroke.lineJoin) {
        case LineJoin::Miter:  pen.SetLineJoin(LineJoinMiter); break;
        case LineJoin::Round:  pen.SetLineJoin(LineJoinRound); break;
        case LineJoin::Bevel:  pen.SetLineJoin(LineJoinBevel); break;
        }*/

        // miterlimit
        pen.SetMiterLimit(Path->getSVGStyle().stroke.miterlimit);

        g.DrawPath(&pen, &gp);
    }

    // Restore transform
    g.SetTransform(&oldMatrix);
}
