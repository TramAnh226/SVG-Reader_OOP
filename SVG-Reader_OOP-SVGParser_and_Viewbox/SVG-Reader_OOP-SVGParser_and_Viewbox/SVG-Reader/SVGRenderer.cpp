#include "SVGRenderer.h"
#include "Stroke.h"        
#include "SVGRectangle.h"  
#include "SVGCircle.h"    
#include "SVGElement.h"  
#include "CustomColor.h" // Cần cho getARGB
#include "CustomPoint.h" // Cần cho CustomPoint.x/y
#include "SVGDocumentContext.h" // Cần cho Context
#include "SVGGradient.h"
#include "SVGLinearGradient.h" // Cần cho dynamic_cast
#include "SVGRadialGradient.h"// Cần cho dynamic_cast
#include "SVGStop.h"
#include "SVGGroup.h"
#include <gdiplus.h>
#include <gdipluscolor.h>
#include <gdiplusbrush.h>
#include <gdiplusimaging.h>
#include <gdiplusgraphics.h>
#include <gdipluscolor.h>
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
using Gdiplus::LinearGradientBrush;
using Gdiplus::PathGradientBrush;
using Gdiplus::GraphicsPath;
using Gdiplus::Matrix; // Cần cho việc áp dụng Transform
using Gdiplus::RectF;
using Gdiplus::PointF;
using Gdiplus::Color;
using Gdiplus::WrapModeTileFlipXY;
using Gdiplus::WrapModeTile;
using Gdiplus::WrapModeClamp;
using Gdiplus::Status;
using Gdiplus::InvalidParameter;
using Gdiplus::Ok;
using Gdiplus::NotImplemented;

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
//void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect, const SVGDocumentContext& context) {
//    if (!rect) return;
//
//    // <<< BỔ SUNG CÁC KHAI BÁO BIẾN BỊ THIẾU TẠI ĐÂY >>>
//
//    // Lấy tọa độ và kích thước từ đối tượng rect
//    Gdiplus::Point p = rect->getTopLeftCorner();
//    Gdiplus::PointF topLeft(static_cast<float>(p.X), static_cast<float>(p.Y));
//    float w = rect->getWidth();
//    float h = rect->getHeight();
//
//    // Khởi tạo Brush cho Fill
//    Gdiplus::SolidBrush brush(rect->getSVGStyle().getGdiFillColor());
//    //Gdiplus::SolidBrush brush(rect->getSVGStyle().getFillColor());
//    g.FillRectangle(&brush, topLeft.X, topLeft.Y, w, h);
//
//    // KIỂM TRA NULL TRƯỚC KHI TRUY CẬP STROKE
//    Stroke* strokeObj = rect->getSVGStyle().getStroke();
//    //Stroke* strokeObj = rect->style.stroke;
//    if (strokeObj != nullptr) {
//        // Tạo Pen bằng cách truy cập các thuộc tính thông qua con trỏ hợp lệ
//        Gdiplus::Pen pen(strokeObj->strokeColor, strokeObj->strokeWidth);
//
//        // Chỉ vẽ DrawRectangle nếu có stroke
//        g.DrawRectangle(&pen, topLeft.X, topLeft.Y, w, h);
//    }
//}
void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect, const SVGDocumentContext& context) {
    if (!rect) return;

    // 1. TÍNH TOÁN BOUNDS (Giữ nguyên)
    CustomPoint customP = rect->getTopLeftCorner();
    Gdiplus::PointF topLeft(static_cast<float>(customP.x), static_cast<float>(customP.y));
    float w = rect->getWidth();
    float h = rect->getHeight();
    Gdiplus::RectF bounds(topLeft.X, topLeft.Y, w, h);

    // 2. LOGIC FILL (Gradient HOẶC Màu đơn - Giữ nguyên logic Fill)
    if (rect->getSVGStyle().hasGradientFill()) {
        const SVGGradient* grad = rect->getSVGStyle().getGrad();
        if (grad) {
            renderGradientFill(g, grad, bounds, context);
        }
    }
    else {
        // TRƯỜNG HỢP MÀU ĐƠN
        Color fillColor = rect->getSVGStyle().getGdiFillColor();
        if (fillColor.GetA() > 0) {
            SolidBrush brush(fillColor);
            g.FillRectangle(&brush, bounds);
        }
    }

    // 3. LOGIC STROKE (CHỈ MÀU ĐƠN)
    Stroke* strokeObj = rect->getSVGStyle().getStroke();
    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        // finalStrokeColor đã tính toán Opacity (từ getGdiColor())
        Color finalStrokeColor = strokeObj->getGdiColor();

        if (finalStrokeColor.GetA() > 0) {
            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
            g.DrawRectangle(&pen, bounds);
        }
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
//void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle, const SVGDocumentContext& context) {
//    if (!circle) return;
//
//    // get infor
//    Gdiplus::PointF center = circle->getCenter();
//    float radius = circle->getRadius();
//
//    // get fill color
//    Gdiplus::SolidBrush brush(circle->getSVGStyle().getGdiFillColor());
//    //Gdiplus::Color fill = circle->getSVGStyle().getFillColor();
//    //Gdiplus::SolidBrush brush(fill);
//
//    // 1. fill the circle
//    float diameter = radius * 2;
//    g.FillEllipse(&brush, center.X - radius, center.Y - radius, diameter, diameter);
//
//    // --------------------------------------------------------
//    // 2. check and render stroke
//    // --------------------------------------------------------
//    Stroke* strokeObj = circle->getSVGStyle().getStroke();
//
//    if (strokeObj != nullptr) {
//        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
//        Gdiplus::Color strokeColor = strokeObj->strokeColor;
//        float strokeW = strokeObj->strokeWidth;
//        if (strokeW <= 0.0f) {
//            strokeW = 1.0f; // Đảm bảo độ rộng tối thiểu để viền hiển thị
//        }
//        // Tạo Pen
//        Gdiplus::Pen pen(strokeColor, strokeW);
//
//        // Vẽ stroke
//        g.DrawEllipse(&pen, center.X - radius, center.Y - radius, diameter, diameter);
//    }
//}
void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle, const SVGDocumentContext& context) {
    if (!circle) return;

    PointF center = circle->getCenter();
    float radius = circle->getRadius();
    float diameter = radius * 2;
    Gdiplus::RectF bounds(center.X - radius, center.Y - radius, diameter, diameter);

    // 1. LOGIC FILL
    if (circle->getSVGStyle().hasGradientFill()) {
        const SVGGradient* grad = circle->getSVGStyle().getGrad();
        if (grad) {
            renderGradientFill(g, grad, bounds, context);
        }
    }
    else {
        // TRƯỜNG HỢP MÀU ĐƠN
        Color fillColor = circle->getSVGStyle().getGdiFillColor();
        if (fillColor.GetA() > 0) {
            SolidBrush brush(fillColor);
            g.FillEllipse(&brush, bounds);
        }
    }

    // 2. LOGIC STROKE
    Stroke* strokeObj = circle->getSVGStyle().getStroke();
    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        Color finalStrokeColor = strokeObj->getGdiColor();

        if (finalStrokeColor.GetA() > 0) {
            // Cần bounds để vẽ
            PointF center = circle->getCenter();
            float radius = circle->getRadius();
            float diameter = radius * 2;
            Gdiplus::RectF bounds(center.X - radius, center.Y - radius, diameter, diameter);

            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
            g.DrawEllipse(&pen, bounds);
        }
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
//void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse, const SVGDocumentContext& context) {
//    if (!ellipse) return;
//
//    // Lấy thông tin cơ bản
//    PointF center = ellipse->getCenter();
//    float rx = ellipse->getRadiusX();
//    float ry = ellipse->getRadiusY();
//
//    // Khởi tạo Brush cho Fill
//    //Gdiplus::SolidBrush brush(ellipse->getSVGStyle().getFillColor());
//    Gdiplus::SolidBrush brush(ellipse->getSVGStyle().getGdiFillColor());
//    // 1. Fill hình elip
//    g.FillEllipse(&brush, center.X - rx, center.Y - ry, rx * 2, ry * 2);
//
//    // --------------------------------------------------------
//    // 2. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
//    // --------------------------------------------------------
//    
//    Stroke* strokeObj = ellipse->getSVGStyle().getStroke();
//    //Stroke* strokeObj = new Stroke();
//    if (strokeObj != nullptr) {
//        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
//        Gdiplus::Color strokeColor = strokeObj->strokeColor;
//        float strokeW = strokeObj->strokeWidth;
//
//        // Tạo Pen và vẽ
//        Gdiplus::Pen pen(strokeColor, strokeW);
//        g.DrawEllipse(&pen, center.X - rx, center.Y - ry, rx * 2, ry * 2);
//    }
//}
void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse, const SVGDocumentContext& context) {
    if (!ellipse) return;

    PointF center = ellipse->getCenter();
    float rx = ellipse->getRadiusX();
    float ry = ellipse->getRadiusY();
    Gdiplus::RectF bounds(center.X - rx, center.Y - ry, rx * 2, ry * 2);

    // 1. LOGIC FILL
    if (ellipse->getSVGStyle().hasGradientFill()) {
        const SVGGradient* grad = ellipse->getSVGStyle().getGrad();
        if (grad) {
            renderGradientFill(g, grad, bounds, context);
        }
    }
    else {
        Color fillColor = ellipse->getSVGStyle().getGdiFillColor();
        if (fillColor.GetA() > 0) {
            SolidBrush brush(fillColor);
            g.FillEllipse(&brush, bounds);
        }
    }

    // 2. LOGIC STROKE
    Stroke* strokeObj = ellipse->getSVGStyle().getStroke();
    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        Color finalStrokeColor = strokeObj->getGdiColor();

        if (finalStrokeColor.GetA() > 0) {
            // Cần bounds để vẽ
            PointF center = ellipse->getCenter();
            float rx = ellipse->getRadiusX();
            float ry = ellipse->getRadiusY();
            Gdiplus::RectF bounds(center.X - rx, center.Y - ry, rx * 2, ry * 2);

            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
            g.DrawEllipse(&pen, bounds);
        }
    }
}
//void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* line, const SVGDocumentContext& context) {
//    if (!line) return;
//
//    // Lấy thông tin điểm (giả định CustomPoint đã có toán tử chuyển đổi hoặc bạn truy cập x, y)
//    PointF start = line->getStartPoint();
//    PointF end = line->getEndPoint();
//
//    // --------------------------------------------------------
//    // 1. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
//    // --------------------------------------------------------
//    Stroke* strokeObj = line->getSVGStyle().getStroke();
//
//    if (strokeObj != nullptr) {
//        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
//        Gdiplus::Color strokeColor = strokeObj->strokeColor;
//        float strokeW = strokeObj->strokeWidth;
//
//        // Tạo Pen và vẽ đường thẳng
//        Gdiplus::Pen pen(strokeColor, strokeW);
//        g.DrawLine(&pen, start, end);
//    }
//}
void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* line, const SVGDocumentContext& context) {
    if (!line) return;

    PointF start = line->getStartPoint();
    PointF end = line->getEndPoint();

    // 1. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
    Stroke* strokeObj = line->getSVGStyle().getStroke();

    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        Color finalStrokeColor = strokeObj->getGdiColor();
        float strokeW = strokeObj->strokeWidth;

        if (finalStrokeColor.GetA() > 0) {
            // FIX: Gradient Stroke Logic (Nếu cần)
            // Hiện tại Line không thể fill, nên ta chỉ cần xử lý Stroke

            Gdiplus::Pen pen(finalStrokeColor, strokeW);
            g.DrawLine(&pen, start, end);
        }
    }
}

//void SVGRenderer::renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon, const SVGDocumentContext& context) {
//    if (!polygon) return;
//    const std::vector<CustomPoint>& customPoints = polygon->getPoints();
//    std::vector<PointF> gdiPoints;
//    gdiPoints.reserve(customPoints.size());
//    for (const auto& cp : customPoints) {
//        gdiPoints.emplace_back(cp.x, cp.y);
//    }
//    if (gdiPoints.empty()) return;
//
//    // 1. Fill Polygon
//    //Gdiplus::SolidBrush brush(polygon->getSVGStyle().getFillColor());
//    Gdiplus::SolidBrush brush(polygon->getSVGStyle().getGdiFillColor());
//    g.FillPolygon(&brush, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//
//    // 2. KIỂM TRA NULL TRƯỚC KHI TRUY CẬP STROKE
//    Stroke* strokeObj = polygon->getSVGStyle().getStroke();
//
//    if (strokeObj != nullptr) {
//        // Tạo Pen và vẽ
//        Gdiplus::Pen pen(strokeObj->strokeColor, strokeObj->strokeWidth);
//        g.DrawPolygon(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//    }
//}
void SVGRenderer::renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon, const SVGDocumentContext& context) {
    if (!polygon) return;
    const std::vector<CustomPoint>& customPoints = polygon->getPoints();
    std::vector<PointF> gdiPoints;
    gdiPoints.reserve(customPoints.size());
    for (const auto& cp : customPoints) {
        gdiPoints.emplace_back(cp.x, cp.y);
    }
    if (gdiPoints.empty()) return;

    Gdiplus::GraphicsPath path;
    path.AddLines(gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
    path.CloseFigure();
    Gdiplus::RectF bounds;
    path.GetBounds(&bounds);

    // 2. LOGIC FILL (Gọi renderGradientPathFill)
    if (polygon->getSVGStyle().hasGradientFill()) {
        const SVGGradient* grad = polygon->getSVGStyle().getGrad();
        if (grad) {
            // Dùng hàm hỗ trợ tô Path bằng Gradient
            renderGradientPathFill(g, grad, path, bounds, context);
        }
    }
    else {
        // TRƯỜNG HỢP MÀU ĐƠN
        Color fillColor = polygon->getSVGStyle().getGdiFillColor();
        if (fillColor.GetA() > 0) {
            SolidBrush brush(fillColor);
            g.FillPath(&brush, &path);
        }
    }

    // 3. LOGIC STROKE (CHỈ MÀU ĐƠN)
    Stroke* strokeObj = polygon->getSVGStyle().getStroke();
    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        Color finalStrokeColor = strokeObj->getGdiColor();
        if (finalStrokeColor.GetA() > 0) {
            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
            g.DrawPath(&pen, &path); // Vẽ Path
        }
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
//void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline, const SVGDocumentContext& context) {
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
//    // Lấy màu Fill đã có Opacity
//    Gdiplus::Color fillColor = polyline->getSVGStyle().getGdiFillColor();
//
//    // --------------------------------------------------------
//    // 2. TÔ MÀU FILL (Nếu Alpha > 0)
//    // --------------------------------------------------------
//    // Nếu fill được định nghĩa và opacity > 0, tô màu nó như một Polygon đóng
//    if (fillColor.GetA() > 0) {
//        Gdiplus::SolidBrush brush(fillColor);
//        // Sử dụng FillPolygon để tô màu (xem như hình đóng)
//        g.FillPolygon(&brush, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//    }
//    // --------------------------------------------------------
//
//    // 3. Kiểm tra và Vẽ Stroke
//    Stroke* strokeObj = polyline->getSVGStyle().getStroke();
//
//    if (strokeObj != nullptr) {
//        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
//        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth;
//
//        // Ép buộc độ rộng tối thiểu
//        if (strokeW <= 0.0f) {
//            strokeW = 1.0f;
//        }
//
//        // Tạo Pen và vẽ đường gấp khúc (DrawLines)
//        Gdiplus::Pen pen(finalStrokeColor, strokeW);
//        g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//    }
//}
void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline, const SVGDocumentContext& context) {
    using namespace Gdiplus;

    if (!polyline) return;

    // 1. Chuyển đổi điểm
    std::vector<PointF> gdiPoints;
    const auto& customPoints = polyline->getPoints();
    for (const auto& cp : customPoints) {
        gdiPoints.emplace_back(cp.x, cp.y);
    }
    if (gdiPoints.size() < 2) return;

    // 1.1. TẠO GRAPHICS PATH VÀ TÍNH BOUNDS (Bắt buộc cho Fill và Gradient)
    GraphicsPath path;
    path.AddLines(gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
    path.CloseFigure(); // Xử lý như Polygon đóng cho Fill

    RectF bounds;
    path.GetBounds(&bounds); // Lấy Bounding Box chính xác

    // 2. LOGIC FILL (Gradient HOẶC Màu đơn)
    if (polyline->getSVGStyle().hasGradientFill()) {
        const SVGGradient* grad = polyline->getSVGStyle().getGrad();
        if (grad) {
            // GỌI HÀM HỖ TRỢ FILL PATH BẰNG GRADIENT
            renderGradientPathFill(g, grad, path, bounds, context);
        }
    }
    else {
        // TRƯỜNG HỢP MÀU ĐƠN (Solid Fill)
        Color fillColor = polyline->getSVGStyle().getGdiFillColor();

        if (fillColor.GetA() > 0 && polyline->getSVGStyle().getFillOpacity() > 0) {
            SolidBrush brush(fillColor);
            // Dùng FillPath sau khi Path đã được đóng
            g.FillPath(&brush, &path);
        }
    }

    // 3. Kiểm tra và Vẽ Stroke (Chỉ cần vẽ đường, không cần đóng Path)
    Stroke* strokeObj = polyline->getSVGStyle().getStroke();
    if (strokeObj != nullptr) {
        Color finalStrokeColor = strokeObj->getGdiColor();
        float strokeW = strokeObj->strokeWidth;

        if (finalStrokeColor.GetA() > 0) {
            // Tạo Pen và vẽ đường gấp khúc (DrawLines)
            Gdiplus::Pen pen(finalStrokeColor, strokeW);
            // Dùng DrawLines thay vì DrawPath để vẽ Polyline MỞ (chuẩn SVG)
            g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
        }
    }
}
//void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text, const SVGDocumentContext& context) {
//    if (!text) return;
//    PointF pos = text->getStart();// tương tự 
//    std::wstring content = text->getContent();
//    Color color = text->getSVGStyle().getGdiFillColor();// tương tự 
//    float fontSize = text->getFontSize();
//    Gdiplus::FontFamily fontFamily(L"Times New Roman");
//    Gdiplus::Font font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
//    Gdiplus::SolidBrush brush(color);
//    PointF new_pos(pos.X, pos.Y - text->getFontSize() * 1.0f);
//    g.DrawString(content.c_str(), -1, &font, new_pos, &brush);
//}
void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text, const SVGDocumentContext& context) {
    using namespace Gdiplus;

    if (!text) return;

    // Lấy thông tin cơ bản
    PointF pos = text->getStart();
    std::wstring content = text->getContent();
    float fontSize = text->getFontSize();

    // Gdiplus Font setup
    FontFamily fontFamily(L"Times New Roman"); // Hoặc lấy từ Style nếu có
    Font font(&fontFamily, fontSize, FontStyleRegular, UnitPixel);

    // Vị trí baseline (Điều chỉnh Y)
    PointF new_pos(pos.X, pos.Y - text->getFontSize() * 0.8f);

    // --- LOGIC FILL: GRADIENT HAY SOLID ---

    if (text->getSVGStyle().hasGradientFill()) {

        // 1. TẠO GRAPHICS PATH TỪ TEXT (BẮT BUỘC cho Gradient)
        GraphicsPath path;

        // Cờ định dạng để đảm bảo căn chỉnh đúng (FormatFlagsNoWrap là mặc định tốt)
        StringFormat format(StringFormat::GenericTypographic());

        // Thêm chuỗi văn bản vào Path
        Status status = path.AddString(
            content.c_str(),
            static_cast<INT>(content.length()),
            &fontFamily,
            FontStyleRegular,
            fontSize,
            new_pos, // Điểm bắt đầu
            &format
        );

        if (status == Ok) {
            RectF bounds;
            path.GetBounds(&bounds); // Lấy Bounding Box của Text Path

            const SVGGradient* grad = text->getSVGStyle().getGrad();
            if (grad) {
                // Sử dụng hàm đã có để tô Path bằng Gradient
                renderGradientPathFill(g, grad, path, bounds, context);
            }
        }
    }
    else {
        // TRƯỜNG HỢP MÀU ĐƠN (Solid Fill)
        Color color = text->getSVGStyle().getGdiFillColor();

        if (color.GetA() > 0) {
            SolidBrush brush(color);

            // Sử dụng DrawString truyền thống cho Solid Fill
            g.DrawString(content.c_str(), -1, &font, new_pos, &brush);
        }
    }

    // --- LOGIC STROKE CHO TEXT (Nếu cần) ---
    // GDI+ không hỗ trợ Stroke Text bằng Pen dễ dàng như SVG.
    // Nếu bạn muốn Stroke Text, bạn phải dùng Path::Widen và tô màu Path đó.
    // Tạm thời bỏ qua logic Stroke phức tạp này để giữ Text hoạt động.
}

//void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* square, const SVGDocumentContext& context) {
//    if (!square) return;
//    PointF topLeft = square->getTopLeftCorner();
//    float side = square->getHeight();// tương tự 
//    Gdiplus::SolidBrush brush(square->getSVGStyle().getGdiFillColor());
//    Gdiplus::Pen pen(square->getSVGStyle().getStroke()->strokeColor, square->getSVGStyle().getStroke()->strokeWidth);
//    g.FillRectangle(&brush, topLeft.X, topLeft.Y, side, side);
//    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, side, side);
//}
void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* square, const SVGDocumentContext& context) {
    if (!square) return;

    // Sử dụng renderRectangle vì Square là Rect đặc biệt
    SVGRectangle rect; // Tạo rect tạm thời
    rect.setTopLeftCorner(square->getTopLeftCorner());
    rect.setWidth(square->getHeight());
    rect.setHeight(square->getHeight());
    rect.setSVGStyle(square->getSVGStyle()); // Kế thừa Style

    renderRectangle(g, &rect, context);
}
void SVGRenderer::renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup, const SVGDocumentContext& context) {
    if (!rootGroup) return;
    // FIX: Truyền Context xuống hàm Group
    renderGroup(g, rootGroup, context);
}
void SVGRenderer::renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup, const SVGDocumentContext& context) {
    if (!rootGroup) return;

    const auto& children = rootGroup->getSVGElementArray();
    for (SVGElement* element : children) {
        // FIX: Double Dispatch: Truyền Context xuống các element con
        element->render(*this, g, context);
    }
}

void SVGRenderer::renderGradientPathFill(Gdiplus::Graphics& g, const SVGGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds, const SVGDocumentContext& context) const {
    using namespace Gdiplus;
    if (!grad) return;

    const SVGGradient* resolvedGrad;
    if (!resolvedGrad->resolveReference(context)) return;

    if (auto* linear = dynamic_cast<const SVGLinearGradient*>(grad)) {
        const std::vector<SVGStop>& stops = linear->getStopArray();
        if (stops.empty()) return;

        PointF p1(bounds.X + linear->getX1() * bounds.Width, bounds.Y + linear->getY1() * bounds.Height);
        PointF p2(bounds.X + linear->getX2() * bounds.Width, bounds.Y + linear->getY2() * bounds.Height);

        Color c1 = (Color)stops.front().getStopColor();
        Color c2 = (stops.size() > 1) ? (Color)stops.back().getStopColor() : c1;

        LinearGradientBrush brush(p1, p2, c1, c2);
        brush.SetWrapMode(getGdiWrapMode(linear->getSpreadMethod()));
        g.FillPath(&brush, &path); // *** Fill Path ***
    }
    else if (auto* radial = dynamic_cast<const SVGRadialGradient*>(grad)) {
        // Tối giản Radial Fill trên Path: Dùng Solid Fill bằng màu Center
        const std::vector<SVGStop>& stops = radial->getStopArray();
        if (stops.empty()) return;

        Color centerColor = (Color)stops.front().getStopColor();
        if (centerColor.GetA() > 0) {
            SolidBrush brush(centerColor);
            g.FillPath(&brush, &path); // *** Fill Path ***
        }
    }
}

//render path
//void SVGRenderer::renderPath(Gdiplus::Graphics& g, SVGPath* Path, vector<PathCommand> commands, const SVGDocumentContext& context) const {
//    using namespace Gdiplus;
//
//    // Lưu transform hiện tại
//    Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    // Áp dụng transform của path (nếu có)
//    if (Path && Path->transform) {
//        // MultiplyTransform: kết hợp transform hiện tại với transform của path
//        // Chọn MatrixOrderPrepend hoặc Append tuỳ mục tiêu (prepend = local -> then world)
//        g.MultiplyTransform(Path->transform, MatrixOrderPrepend);
//    }
//
//    Pen pen(Color(255, 0, 0, 0), 2.0f); // Black pen with width 2.0f)
//    PointF currentPoint(0.0f, 0.0f), startPoint(0.0f, 0.0f);
//
//    for (const auto& cmd : commands) {
//        switch (cmd.command) {
//        case 'M': {
//            currentPoint = PointF(cmd.parameters[0], cmd.parameters[1]);
//            startPoint = currentPoint;
//            break;
//        }
//        case 'L': {
//            PointF p(cmd.parameters[0], cmd.parameters[1]);
//            g.DrawLine(&pen, currentPoint, p);
//            currentPoint = p;
//            break;
//        }
//        case 'H': {
//            float x = cmd.parameters[0];
//            PointF p(x, currentPoint.Y);
//            g.DrawLine(&pen, currentPoint, p);
//
//            currentPoint = p;
//            break;
//        }
//        case 'V': {
//            float y = cmd.parameters[0];
//            PointF p(currentPoint.X, y);
//            g.DrawLine(&pen, currentPoint, p);
//            currentPoint = p;
//            break;
//        }
//        case 'Z': {
//            g.DrawLine(&pen, currentPoint, startPoint);
//            currentPoint = startPoint;
//            break;
//        }
//        case 'C': {
//            PointF p1(cmd.parameters[0], cmd.parameters[1]);
//            PointF p2(cmd.parameters[2], cmd.parameters[3]);
//            PointF p3(cmd.parameters[4], cmd.parameters[5]);
//
//            g.DrawBezier(&pen, currentPoint, p1, p2, p3);
//            currentPoint = p3;
//            break;
//        }
//        }
//    }
//    g.SetTransform(&oldMatrix); // Khôi phục transform ban đầu
//}

void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* pathElement, const std::vector<PathCommand>& commands, const SVGDocumentContext& context) const {
    using namespace Gdiplus;

    if (!pathElement) return;

    // Lưu transform hiện tại
    Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    // Áp dụng transform của path (nếu có)
    if (pathElement->transform) {
        g.MultiplyTransform(pathElement->transform, MatrixOrderPrepend);
    }

    // 1. XÂY DỰNG GRAPHICS PATH TỪ CÁC COMMANDS
    GraphicsPath path;
    PointF currentPoint(0.0f, 0.0f), startPoint(0.0f, 0.0f);
    bool subpathStarted = false;

    for (const auto& cmd : commands) {
        // 
        switch (cmd.command) {
        case 'M':
        case 'm': {
            // M: MoveTo
            currentPoint = PointF(cmd.parameters[0], cmd.parameters[1]);
            startPoint = currentPoint;
            if (subpathStarted) {
                path.StartFigure(); // Bắt đầu hình dạng mới
            }
            subpathStarted = true;
            break;
        }
        case 'L':
        case 'l': {
            // L: LineTo
            PointF p(cmd.parameters[0], cmd.parameters[1]);
            path.AddLine(currentPoint, p);
            currentPoint = p;
            break;
        }
                // ... (Thêm các lệnh khác H, V, Q, S, A nếu cần) ...
        case 'H': { // Horizontal LineTo
            float x = cmd.parameters[0];
            PointF p(x, currentPoint.Y);
            path.AddLine(currentPoint, p);
            currentPoint = p;
            break;
        }
        case 'V': { // Vertical LineTo
            float y = cmd.parameters[0];
            PointF p(currentPoint.X, y);
            path.AddLine(currentPoint, p);
            currentPoint = p;
            break;
        }
        case 'C': {
            // C: Cubic Bezier
            PointF p1(cmd.parameters[0], cmd.parameters[1]);
            PointF p2(cmd.parameters[2], cmd.parameters[3]);
            PointF p3(cmd.parameters[4], cmd.parameters[5]);
            path.AddBezier(currentPoint, p1, p2, p3);
            currentPoint = p3;
            break;
        }
        case 'Z':
        case 'z': {
            // Z: ClosePath
            path.CloseFigure();
            currentPoint = startPoint;
            subpathStarted = false;
            break;
        }
        }
    }

    // 2. TÍNH TOÁN BOUNDS
    RectF bounds;
    path.GetBounds(&bounds);

    // 3. LOGIC FILL (Gradient HOẶC Màu đơn)
    if (pathElement->getSVGStyle().hasGradientFill()) {
        const SVGGradient* grad = pathElement->getSVGStyle().getGrad();
        if (grad) {
            // Sử dụng hàm hỗ trợ Fill Path (Đã sửa ở câu trước)
            renderGradientPathFill(g, grad, path, bounds, context);
        }
    }
    else {
        // TRƯỜNG HỢP MÀU ĐƠN
        Color fillColor = pathElement->getSVGStyle().getGdiFillColor();
        if (fillColor.GetA() > 0) {
            SolidBrush brush(fillColor);
            g.FillPath(&brush, &path);
        }
    }

    // 4. LOGIC STROKE (CHỈ MÀU ĐƠN)
    Stroke* strokeObj = pathElement->getSVGStyle().getStroke();
    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        Color finalStrokeColor = strokeObj->getGdiColor();
        if (finalStrokeColor.GetA() > 0) {
            // Pen chỉ dùng màu đơn (theo quyết định trước đó)
            Pen pen(finalStrokeColor, strokeObj->strokeWidth);
            g.DrawPath(&pen, &path);
        }
    }

    // Khôi phục transform ban đầu
    g.SetTransform(&oldMatrix);
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

Gdiplus::WrapMode SVGRenderer::getGdiWrapMode(const std::string& spreadMethod) const {
    if (spreadMethod == "reflect") {
        return Gdiplus::WrapModeTileFlipXY;
    }
    if (spreadMethod == "repeat") {
        return Gdiplus::WrapModeTile;
    }
    // Mặc định hoặc "pad"
    return Gdiplus::WrapModeClamp;
}

void SVGRenderer::renderLinearGradientFill(Gdiplus::Graphics& g, const SVGLinearGradient* grad, const Gdiplus::RectF& bounds) {

    // Sử dụng Getter để truy cập stopArray
    const std::vector<SVGStop>& stops = grad->getStopArray();

    if (!grad || stops.empty()) return;

    // 1. Tính toán điểm bắt đầu và kết thúc dựa trên bounds
    // Sử dụng Getters cho X1, Y1, X2, Y2
    PointF p1(bounds.X + grad->getX1() * bounds.Width,
        bounds.Y + grad->getY1() * bounds.Height);

    PointF p2(bounds.X + grad->getX2() * bounds.Width,
        bounds.Y + grad->getY2() * bounds.Height);

    // 2. Lấy màu đầu và màu cuối (KHÔNG DÙNG ColorBlend)

    // Lấy màu đầu
    Color c1 = stops.front().getStopColor();

    // Lấy màu cuối
    Color c2 = (stops.size() > 1)
        ? stops.back().getStopColor()
        : c1;

    // 3. Tạo LinearGradientBrush
    LinearGradientBrush brush(p1, p2, c1, c2);

    // 4. Áp dụng Spread Method (Wrap Mode)
    // Sử dụng Getter cho spreadMethod
    brush.SetWrapMode(getGdiWrapMode(grad->getSpreadMethod()));

    // 5. Fill hình dạng trong bounds
    g.FillRectangle(&brush, bounds);
}
// Hàm hỗ trợ tạo Radial Gradient Brush
void SVGRenderer::renderRadialGradientFill(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::RectF& bounds) {
    using namespace Gdiplus;

    if (!grad) return;

    // Sử dụng Getter để truy cập stopArray
    const std::vector<SVGStop>& stops = grad->getStopArray();

    if (stops.empty()) return;

    // 1. CHUẨN BỊ: Màu sắc và Vị trí
    Color centerColor = stops.front().getStopColor(); // Center Color (Stop 1)

    // Màu bao quanh (Surround Color) - Tối giản: Chỉ lấy Stop cuối cùng
    Color surroundColor = (stops.size() > 1)
        ? stops.back().getStopColor()
        : centerColor;

    if (centerColor.GetA() == 0 && surroundColor.GetA() == 0) return;

    // 2. TẠO GRAPHICS PATH (Hình dạng cho Radial Gradient)
    // Radial Gradient thường được áp dụng trên hình tròn/elip. Ta tạo Elip trong bounds.
    GraphicsPath path;

    // Gdiplus::RectF nhận top/left/width/height. Dùng nó để tạo Elip.
    path.AddEllipse(bounds);

    // 3. TẠO PATH GRADIENT BRUSH
    PathGradientBrush brush(&path);

    // 4. CẤU HÌNH BRUSH

    // Center Point: Tính toán dựa trên cx, cy (giả định là giá trị tương đối [0, 1])
    // Cần Getter cho cx và cy.
    PointF centerPoint(bounds.X + grad->getCX() * bounds.Width,
        bounds.Y + grad->getCY() * bounds.Height);

    brush.SetCenterPoint(centerPoint);
    brush.SetCenterColor(centerColor);

    // Surround Colors (Màu bao quanh)
    // Chỉ sử dụng 1 màu bao quanh (Stop cuối)
    int count = 1;
    // Gdiplus::PathGradientBrush yêu cầu con trỏ mảng màu và số lượng phần tử

    // NOTE: Vì ta không dùng ColorBlend, ta phải cung cấp mảng Surround Colors,
    // ngay cả khi chỉ có 1 màu.
    Color surroundColors[1] = { surroundColor };
    brush.SetSurroundColors(surroundColors, &count);

    // 5. Áp dụng Spread Method (Wrap Mode)
    brush.SetWrapMode(getGdiWrapMode(grad->getSpreadMethod()));

    // 6. FILL
    g.FillRectangle(&brush, bounds);
    // CHÚ Ý: FillRectangle được dùng vì brush được định nghĩa trên bounds. 
    // Nếu bạn muốn Fill chính xác hình dạng Element (VD: chỉ fill hình tròn),
    // bạn cần dùng g.FillPath(&brush, &path); và phải biết hình dạng gốc.
    // Ở đây ta dùng FillRectangle (hoặc FillEllipse) để tô đầy bounds.
}
// Hàm tổng quát renderGradientFill mới
void SVGRenderer::renderGradientFill(Gdiplus::Graphics& g, const SVGGradient* grad, const RectF& bounds, const SVGDocumentContext& context) {
    if (!grad) return;

    const SVGGradient* resolvedGrad;
    if (!resolvedGrad->resolveReference(context)) return;

    if (auto* linear = dynamic_cast<const SVGLinearGradient*>(grad)) {
        // Gọi Linear Gradient (Đã sửa)
        renderLinearGradientFill(g, linear, bounds);
    }
    // Loại bỏ logic Solid Fill tạm thời và gọi hàm đầy đủ
    else if (auto* radial = dynamic_cast<const SVGRadialGradient*>(grad)) {
        // GỌI HÀM RADIAL ĐÃ TRIỂN KHAI ĐẦY ĐỦ
        renderRadialGradientFill(g, radial, bounds);
    }
}


