#include "SVGRenderer.h"
#include "Stroke.h"        
#include "SVGRectangle.h"  
#include "SVGCircle.h"    
#include "SVGElement.h"   
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

    // Lấy tọa độ và kích thước từ đối tượng rect
    Gdiplus::Point p = rect->getTopLeftCorner();
    Gdiplus::PointF topLeft(static_cast<float>(p.X), static_cast<float>(p.Y));
    float w = rect->getWidth();
    float h = rect->getHeight();

    // 1. Fill (Tô màu)
    //Gdiplus::SolidBrush brush(rect->getSVGStyle().getGdiFillColor());
    //g.FillRectangle(&brush, topLeft.X, topLeft.Y, w, h);
    Gdiplus::Color fillColor = rect->getSVGStyle().getGdiFillColor();

    // CHỈ TÔ MÀU NẾU FILL COLOR KHÔNG HOÀN TOÀN TRONG SUỐT (Alpha > 0)
    // Nếu fill-opacity = 0.01, Alpha sẽ là 3. Nếu fill-opacity = 0, Alpha là 0.
    if (fillColor.GetA() > 0) {
        Gdiplus::SolidBrush brush(fillColor);
        g.FillRectangle(&brush, topLeft.X, topLeft.Y, w, h);
    }

    // 2. Stroke (Viền)
    Stroke* strokeObj = rect->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        float strokeWidth = strokeObj->strokeWidth;
        // Bổ sung: Chỉ vẽ nếu độ rộng viền lớn hơn 0
        if (strokeWidth > 0.0f) {
            // Tạo Pen: Giả định strokeObj->getGdiColor() đã tính đến strokeOpacity
            Gdiplus::Pen pen(strokeObj->getGdiColor(), strokeWidth);

            g.DrawRectangle(&pen, topLeft.X, topLeft.Y, w, h);
        }
    }
}
void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle) {
    if (!circle) return;

    // get infor
    Gdiplus::PointF center = circle->getCenter();
    float radius = circle->getRadius();

    // get fill color
    Gdiplus::SolidBrush brush(circle->getSVGStyle().getGdiFillColor());
    //Gdiplus::Color fill = circle->getSVGStyle().getFillColor();
    //Gdiplus::SolidBrush brush(fill);

    // 1. fill the circle
    float diameter = radius * 2;
    g.FillEllipse(&brush, center.X - radius, center.Y - radius, diameter, diameter);

    // --------------------------------------------------------
    // 2. check and render stroke
    // --------------------------------------------------------
    Stroke* strokeObj = circle->getSVGStyle().getStroke();
    //if (strokeObj != nullptr) {
        //float strokeW = strokeObj->strokeWidth;

        //// ĐIỀU KIỆN QUAN TRỌNG: Chỉ vẽ nếu có độ rộng viền > 0
        //if (strokeW > 0.0f) {
        //    Gdiplus::Color strokeColor = strokeObj->getGdiColor();
        //    Gdiplus::Pen pen(strokeColor, strokeW);
        //    float diameter = circle->getRadius() * 2;
        //    g.DrawEllipse(&pen, center.X - radius, center.Y - radius, diameter, diameter);
        //}
    //}
    if (strokeObj != nullptr) { // Vẫn giữ kiểm tra null phòng ngừa (hoặc bỏ nếu đã fix Bước 1)
        Gdiplus::Color strokeColor = strokeObj->getGdiColor();
        float strokeW = strokeObj->strokeWidth;
        float halfStroke = strokeW / 2.0f; // Bù trừ

        // Tạo Pen (đã có)
        Gdiplus::Pen pen(strokeColor, strokeW);

        // SỬA: Điều chỉnh tọa độ và kích thước để Stroke được vẽ bên trong
        g.DrawEllipse(&pen,
            center.X - radius + halfStroke, // X mới
            center.Y - radius + halfStroke, // Y mới
            diameter - strokeW,             // Width mới (diameter - 2*halfStroke)
            diameter - strokeW);            // Height mới
    }
}
void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse) {
    if (!ellipse) return;

    // Lấy thông tin cơ bản
    PointF center = ellipse->getCenter();
    float rx = ellipse->getRadiusX();
    float ry = ellipse->getRadiusY();

    // Khởi tạo Brush cho Fill
    //Gdiplus::SolidBrush brush(ellipse->getSVGStyle().getFillColor());
    Gdiplus::SolidBrush brush(ellipse->getSVGStyle().getGdiFillColor());
    // 1. Fill hình elip
    g.FillEllipse(&brush, center.X - rx, center.Y - ry, rx * 2, ry * 2);

    // --------------------------------------------------------
    // 2. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
    // --------------------------------------------------------
    Stroke* strokeObj = ellipse->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        Gdiplus::Color strokeColor = strokeObj->getGdiColor();
        float strokeW = strokeObj->strokeWidth;

        // BỔ SUNG: Chỉ vẽ khi có độ rộng viền > 0
        if (strokeW > 0.0f) {
            Gdiplus::Pen pen(strokeColor, strokeW);
            g.DrawEllipse(&pen, center.X - rx, center.Y - ry, rx * 2, ry * 2);
        }
    }
}
void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* line) {
    if (!line) return;

    // Lấy thông tin điểm (giả định CustomPoint đã có toán tử chuyển đổi hoặc bạn truy cập x, y)
    Gdiplus::PointF start = line->getStartPoint();
    Gdiplus::PointF end = line->getEndPoint();

    // --------------------------------------------------------
    // 1. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
    // --------------------------------------------------------
    Stroke* strokeObj = line->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        Gdiplus::Color strokeColor = strokeObj->getGdiColor();;
        float strokeW = strokeObj->strokeWidth;

        // BỔ SUNG: Chỉ vẽ khi có độ rộng viền > 0
        if (strokeW > 0.0f) {
            Gdiplus::Pen pen(strokeColor, strokeW);
            g.DrawLine(&pen, start, end);
        }
    }
}
void SVGRenderer::renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon) {
    if (!polygon) return;
    const std::vector<CustomPoint>& customPoints = polygon->getPoints();
    std::vector<PointF> gdiPoints;
    gdiPoints.reserve(customPoints.size());
    for (const auto& cp : customPoints) {
        gdiPoints.emplace_back(cp.x, cp.y);
    }
    if (gdiPoints.empty()) return;

    // 1. Fill Polygon
    //Gdiplus::SolidBrush brush(polygon->getSVGStyle().getFillColor());
    Gdiplus::SolidBrush brush(polygon->getSVGStyle().getGdiFillColor());
    g.FillPolygon(&brush, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));

    // 2. KIỂM TRA NULL TRƯỚC KHI TRUY CẬP STROKE
    Stroke* strokeObj = polygon->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        float strokeW = strokeObj->strokeWidth;

        // BỔ SUNG: Chỉ vẽ khi có độ rộng viền > 0
        if (strokeW > 0.0f) {
            Gdiplus::Pen pen(strokeObj->getGdiColor(), strokeW);
            g.DrawPolygon(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
        }
    }
}
void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline) {
    if (!polyline) return;

    // 1. Chuyển đổi điểm
    std::vector<PointF> gdiPoints;
    const auto& customPoints = polyline->getPoints();

    // Giả sử CustomPoint có thành viên công khai x, y
    for (const auto& cp : customPoints) {
        gdiPoints.emplace_back(cp.x, cp.y);
    }

    if (gdiPoints.size() < 2) return;

    // --------------------------------------------------------
    // 2. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
    // --------------------------------------------------------
    Stroke* strokeObj = polyline->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        Gdiplus::Color strokeColor = strokeObj->getGdiColor();
        float strokeW = strokeObj->strokeWidth;

        // BỔ SUNG: Chỉ vẽ khi có độ rộng viền > 0
        if (strokeW > 0.0f) {
            Gdiplus::Pen pen(strokeColor, strokeW);
            g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
        }
    }
}
void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text) {
    if (!text) return;
    PointF pos = text->getStart();// tương tự 
    std::wstring content = text->getContent();
    Color color = text->getSVGStyle().getGdiFillColor();// tương tự 
    float fontSize = text->getFontSize();
    Gdiplus::FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(color);
    g.DrawString(content.c_str(), -1, &font, pos, &brush);
}
void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* square) {
    if (!square) return;
    PointF topLeft = square->getTopLeftCorner();
    float side = square->getHeight();

    // 1. Fill (Tô màu)
    Gdiplus::SolidBrush brush(square->getSVGStyle().getGdiFillColor());
    g.FillRectangle(&brush, topLeft.X, topLeft.Y, side, side);

    // 2. Stroke (Viền)
    Stroke* strokeObj = square->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        float strokeWidth = strokeObj->strokeWidth;
        // Chỉ vẽ nếu độ rộng viền lớn hơn 0
        if (strokeWidth > 0.0f) {
            Gdiplus::Pen pen(strokeObj->getGdiColor(), strokeWidth);
            g.DrawRectangle(&pen, topLeft.X, topLeft.Y, side, side);
        }
    }
}
void SVGRenderer::renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;
    rootGroup->render(*this, g);//hình như class SVGGroup chưa đc định nghĩa đầy đủ hay sao á, thử thêm virtual void render(SVGRenderer&) const = 0; vô thử 
}
void SVGRenderer::renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup) {
    if (!rootGroup) return;

    const auto& children = rootGroup->getSVGElementArray();
    for (SVGElement* element : children) {
        element->render(*this, g);
    }
}
