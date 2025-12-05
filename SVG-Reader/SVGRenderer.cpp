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
//void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect) {
//    if (!rect) return;
//    Gdiplus::Point p = rect->getTopLeftCorner();
//    Gdiplus::PointF topLeft(static_cast<float>(p.X), static_cast<float>(p.Y));
//    float w = rect->getWidth();
//    float h = rect->getHeight();
//    Gdiplus::SolidBrush brush(rect->getSVGStyle().getFillColor());
//    Gdiplus::Pen pen(rect->getSVGStyle().getStroke()->strokeColor, rect->getSVGStyle().getStroke()->strokeWidth);
//    g.FillRectangle(&brush, topLeft.X, topLeft.Y, w, h);
//    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, w, h);
//}
void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect) {
    if (!rect) return;

    // <<< BỔ SUNG CÁC KHAI BÁO BIẾN BỊ THIẾU TẠI ĐÂY >>>

    // Lấy tọa độ và kích thước từ đối tượng rect
    Gdiplus::Point p = rect->getTopLeftCorner();
    Gdiplus::PointF topLeft(static_cast<float>(p.X), static_cast<float>(p.Y));
    float w = rect->getWidth();
    float h = rect->getHeight();

    // Khởi tạo Brush cho Fill
    Gdiplus::SolidBrush brush(rect->getSVGStyle().getGdiFillColor());
    //Gdiplus::SolidBrush brush(rect->getSVGStyle().getFillColor());
    g.FillRectangle(&brush, topLeft.X, topLeft.Y, w, h);

    // KIỂM TRA NULL TRƯỚC KHI TRUY CẬP STROKE
    Stroke* strokeObj = rect->getSVGStyle().getStroke();
    //Stroke* strokeObj = rect->style.stroke;
    if (strokeObj != nullptr) {
        // Tạo Pen bằng cách truy cập các thuộc tính thông qua con trỏ hợp lệ
        Gdiplus::Pen pen(strokeObj->strokeColor, strokeObj->strokeWidth);

        // Chỉ vẽ DrawRectangle nếu có stroke
        g.DrawRectangle(&pen, topLeft.X, topLeft.Y, w, h);
    }
}
//void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle) {
//    if (!circle) return;
//    Gdiplus::PointF center = circle->getCenter();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
//    float radius = circle->getRadius();
//    Gdiplus::Color fill = circle->getSVGStyle().getFillColor();
//    Gdiplus::Color stroke = circle->getSVGStyle().getStroke()->strokeColor;
//    float strokeW = circle->getSVGStyle().getStroke()->strokeWidth;
//    Gdiplus::SolidBrush brush(fill);
//    Gdiplus::Pen pen(stroke, strokeW);
//    g.FillEllipse(&brush, center.X - radius, center.Y - radius, radius * 2, radius * 2);
//    g.DrawEllipse(&pen, center.X - radius, center.Y - radius, radius * 2, radius * 2);
//}
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

    if (strokeObj != nullptr) {
        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
        Gdiplus::Color strokeColor = strokeObj->strokeColor;
        float strokeW = strokeObj->strokeWidth;
        if (strokeW <= 0.0f) {
            strokeW = 1.0f; // Đảm bảo độ rộng tối thiểu để viền hiển thị
        }
        // Tạo Pen
        Gdiplus::Pen pen(strokeColor, strokeW);

        // Vẽ stroke
        g.DrawEllipse(&pen, center.X - radius, center.Y - radius, diameter, diameter);
    }
}
//void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse) {
//    if (!ellipse) return;
//    PointF center = ellipse->getCenter();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
//    float rx = ellipse->getRadiusX();
//    float ry = ellipse->getRadiusY();
//    Gdiplus::SolidBrush brush(ellipse->getSVGStyle().getFillColor());
//    Gdiplus::Pen pen(ellipse->getSVGStyle().getStroke()->strokeColor, ellipse->getSVGStyle().getStroke()->strokeWidth);
//    g.FillEllipse(&brush, center.X - rx, center.Y - ry, rx * 2, ry * 2);
//    g.DrawEllipse(&pen, center.X - rx, center.Y - ry, rx * 2, ry * 2);
//}
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
    //Stroke* strokeObj = new Stroke();
    if (strokeObj != nullptr) {
        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
        Gdiplus::Color strokeColor = strokeObj->strokeColor;
        float strokeW = strokeObj->strokeWidth;

        // Tạo Pen và vẽ
        Gdiplus::Pen pen(strokeColor, strokeW);
        g.DrawEllipse(&pen, center.X - rx, center.Y - ry, rx * 2, ry * 2);
    }
}
void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* line) {
    if (!line) return;

    // Lấy thông tin điểm (giả định CustomPoint đã có toán tử chuyển đổi hoặc bạn truy cập x, y)
    PointF start = line->getStartPoint();
    PointF end = line->getEndPoint();

    // --------------------------------------------------------
    // 1. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
    // --------------------------------------------------------
    Stroke* strokeObj = line->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
        Gdiplus::Color strokeColor = strokeObj->strokeColor;
        float strokeW = strokeObj->strokeWidth;

        // Tạo Pen và vẽ đường thẳng
        Gdiplus::Pen pen(strokeColor, strokeW);
        g.DrawLine(&pen, start, end);
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
        // Tạo Pen và vẽ
        Gdiplus::Pen pen(strokeObj->strokeColor, strokeObj->strokeWidth);
        g.DrawPolygon(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
    }
}
//void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline) {
//    if (!polyline) return;
//
//    std::vector<PointF> gdiPoints;
//    const auto& customPoints = polyline->getPoints();// tương tự 
//    for (const auto& cp : customPoints) {
//        gdiPoints.emplace_back(cp.x, cp.y);
//    }
//    if (gdiPoints.size() < 2) return;
//    Gdiplus::Pen pen(polyline->getSVGStyle().getStroke()->strokeColor, polyline->getSVGStyle().getStroke()->strokeWidth);// tương tự 
//    g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//}
//void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline) {
//    if (!polyline) return;
//
//    // 1. Chuyển đổi điểm
//    std::vector<PointF> gdiPoints;
//    const auto& customPoints = polyline->getPoints();
//
//    // Giả sử CustomPoint có thành viên công khai x, y
//    for (const auto& cp : customPoints) {
//        gdiPoints.emplace_back(cp.x, cp.y);
//    }
//
//    if (gdiPoints.size() < 2) return;
//
//    // --------------------------------------------------------
//    // 2. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
//    // --------------------------------------------------------
//    Stroke* strokeObj = polyline->getSVGStyle().getStroke();
//
//    if (strokeObj != nullptr) {
//        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
//        Gdiplus::Color strokeColor = strokeObj->strokeColor;
//        float strokeW = strokeObj->strokeWidth;
//
//        // Tạo Pen và vẽ đường gấp khúc
//        Gdiplus::Pen pen(strokeColor, strokeW);
//        g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//    }
//}
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

    // Lấy màu Fill đã có Opacity
    Gdiplus::Color fillColor = polyline->getSVGStyle().getGdiFillColor();

    // --------------------------------------------------------
    // 2. TÔ MÀU FILL (Nếu Alpha > 0)
    // --------------------------------------------------------
    // Nếu fill được định nghĩa và opacity > 0, tô màu nó như một Polygon đóng
    if (fillColor.GetA() > 0) {
        Gdiplus::SolidBrush brush(fillColor);
        // Sử dụng FillPolygon để tô màu (xem như hình đóng)
        g.FillPolygon(&brush, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
    }
    // --------------------------------------------------------

    // 3. Kiểm tra và Vẽ Stroke
    Stroke* strokeObj = polyline->getSVGStyle().getStroke();

    if (strokeObj != nullptr) {
        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
        float strokeW = strokeObj->strokeWidth;

        // Ép buộc độ rộng tối thiểu
        if (strokeW <= 0.0f) {
            strokeW = 1.0f;
        }

        // Tạo Pen và vẽ đường gấp khúc (DrawLines)
        Gdiplus::Pen pen(finalStrokeColor, strokeW);
        g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
    }
}
void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text) {
    if (!text) return;
    PointF pos = text->getStart();// tương tự 
    std::wstring content = text->getContent();
    Color color = text->getSVGStyle().getGdiFillColor();// tương tự 
    float fontSize = text->getFontSize();
    Gdiplus::FontFamily fontFamily(L"Times New Roman");
    Gdiplus::Font font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(color);
    PointF new_pos(pos.X, pos.Y - text->getFontSize() * 1.0f);
    g.DrawString(content.c_str(), -1, &font, new_pos, &brush);
}
void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* square) {
    if (!square) return;
    PointF topLeft = square->getTopLeftCorner();
    float side = square->getHeight();// tương tự 
    Gdiplus::SolidBrush brush(square->getSVGStyle().getGdiFillColor());
    Gdiplus::Pen pen(square->getSVGStyle().getStroke()->strokeColor, square->getSVGStyle().getStroke()->strokeWidth);
    g.FillRectangle(&brush, topLeft.X, topLeft.Y, side, side);
    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, side, side);
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
