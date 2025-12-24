//#include "CustomPoint.h"
//#include "SVGRectangle.h"
//#include "Stroke.h"        
//#include "SVGCircle.h"   
//#include "SVGLine.h"
//#include "SVGPolygon.h"
//#include "SVGPolyline.h"
//#include "SVGText.h"
//#include "SVGSquare.h"
//#include "SVGStop.h"
////#include "SVGElement.h"  
////#include "CustomColor.h" // Cần cho getARGB
////#include "CustomPoint.h" // Cần cho CustomPoint.x/y
////#include "SVGDocumentContext.h" // Cần cho Context
////#include "SVGGradient.h"
//#include "SVGLinearGradient.h" // Cần cho dynamic_cast
//#include "SVGRadialGradient.h"// Cần cho dynamic_cast
////#include "SVGStop.h"
//#include "SVGGroup.h"
////#include <gdiplus.h>
////#include <gdipluscolor.h>
////#include <gdiplusbrush.h>
////#include <gdiplusimaging.h>
////#include <gdiplusgraphics.h>
////#include <gdipluscolor.h>
//
//#include "SVGRenderer.h"
//
//// &localMatrix => dia chi luon ton tai tham chi no khong co vung nho
//
////namespace Gdiplus {
////    struct PointF;
////    class Graphics;
////    class SolidBrush;
////    class Pen;
////    class Color;
////}
////using Gdiplus::PointF;
////using Gdiplus::SolidBrush;
////using Gdiplus::Pen;
////using Gdiplus::Color;
////using Gdiplus::LinearGradientBrush;
////using Gdiplus::PathGradientBrush;
////using Gdiplus::GraphicsPath;
////using Gdiplus::Matrix; // Cần cho việc áp dụng Transform
////using Gdiplus::RectF;
////using Gdiplus::PointF;
////using Gdiplus::Color;
////using Gdiplus::WrapModeTileFlipXY;
////using Gdiplus::WrapModeTile;
////using Gdiplus::WrapModeClamp;
////using Gdiplus::Status;
////using Gdiplus::InvalidParameter;
////using Gdiplus::Ok;
////using Gdiplus::NotImplemented;
//
//SVGRenderer::SVGRenderer() : zoom(1.0f), rotate(0.0f) {}
//
//
//
////void SVGRenderer::setZoom(double z) {
////    zoom = static_cast<float>(z);
////}
////
////void SVGRenderer::setRotation(double deg) {
////    rotate = static_cast<float>(deg);
////}
////pair<double, double> SVGRenderer::transformPoint(double x, double y) const {
////    double rad = rotate * M_PI / 180.0;
////    double newX = (x * cos(rad) - y * sin(rad)) * zoom;
////    double newY = (x * sin(rad) + y * cos(rad)) * zoom;
////    return { newX, newY };
////}
////void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect) {
////    if (!rect) return;
////    Gdiplus::Point p = rect->getTopLeftCorner();
////    Gdiplus::PointF topLeft(static_cast<float>(p.X), static_cast<float>(p.Y));
////    float w = rect->getWidth();
////    float h = rect->getHeight();
////    Gdiplus::SolidBrush brush(rect->getSVGStyle().getFillColor());
////    Gdiplus::Pen pen(rect->getSVGStyle().getStroke()->strokeColor, rect->getSVGStyle().getStroke()->strokeWidth);
////    g.FillRectangle(&brush, topLeft.X, topLeft.Y, w, h);
////    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, w, h);
////}
////void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect, const SVGDocumentContext& context) {
////    if (!rect) return;
////
////    // <<< BỔ SUNG CÁC KHAI BÁO BIẾN BỊ THIẾU TẠI ĐÂY >>>
////
////    // Lấy tọa độ và kích thước từ đối tượng rect
////    Gdiplus::Point p = rect->getTopLeftCorner();
////    Gdiplus::PointF topLeft(static_cast<float>(p.X), static_cast<float>(p.Y));
////    float w = rect->getWidth();
////    float h = rect->getHeight();
////
////    // Khởi tạo Brush cho Fill
////    Gdiplus::SolidBrush brush(rect->getSVGStyle().getGdiFillColor());
////    //Gdiplus::SolidBrush brush(rect->getSVGStyle().getFillColor());
////    g.FillRectangle(&brush, topLeft.X, topLeft.Y, w, h);
////
////    // KIỂM TRA NULL TRƯỚC KHI TRUY CẬP STROKE
////    Stroke* strokeObj = rect->getSVGStyle().getStroke();
////    //Stroke* strokeObj = rect->style.stroke;
////    if (strokeObj != nullptr) {
////        // Tạo Pen bằng cách truy cập các thuộc tính thông qua con trỏ hợp lệ
////        Gdiplus::Pen pen(strokeObj->strokeColor, strokeObj->strokeWidth);
////
////        // Chỉ vẽ DrawRectangle nếu có stroke
////        g.DrawRectangle(&pen, topLeft.X, topLeft.Y, w, h);
////    }
////}
//
//void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect, const SVGDocumentContext& context) {
//    if (!rect) return;
//    // 0. ÁP DỤNG MATRIX LÊN CONTEXT
//    // lưu matrix hiện tại của Graphics Context
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    // áp dụng ma trận biến đổi của đối tượng
//    Gdiplus::Matrix localMatrix;
//    rect->getTransformMatrix().applyToMatrix(localMatrix);
//    //const SVGTransform* matrix = rect->getTransformMatrix();
//    //if (&localMatrix) {
//    //    // nhân ma trận của rect và của context
//    //    //g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    //    g.MultiplyTransform(&localMatrix);
//    //}
//    g.MultiplyTransform(&localMatrix);
//    // 1. TÍNH TOÁN BOUNDS (Giữ nguyên) : dùng toạ độ gốc chưa biến đổi để vẽ
//    CustomPoint customP = rect->getTopLeftCorner();
//    Gdiplus::PointF topLeft(static_cast<float>(customP.x), static_cast<float>(customP.y));
//    float w = rect->getWidth();
//    float h = rect->getHeight();
//    Gdiplus::RectF bounds(topLeft.X, topLeft.Y, w, h);
//
//    // 2. LOGIC FILL (Gradient HOẶC Màu đơn - Giữ nguyên logic Fill)
//    if (rect->getSVGStyle().hasGradientFill()) {
//        const SVGGradient* grad = rect->getSVGStyle().getGrad();
//        if (grad) {
//            renderGradientFill(g, grad, bounds, context);
//        }
//    }
//    else {
//        // TRƯỜNG HỢP MÀU ĐƠN
//        Gdiplus::Color fillColor = rect->getSVGStyle().getGdiFillColor();
//        if (fillColor.GetA() > 0) {
//            Gdiplus::SolidBrush brush(fillColor);
//            g.FillRectangle(&brush, bounds);
//        }
//    }
//
//    // 3. LOGIC STROKE (CHỈ MÀU ĐƠN)
//    Stroke* strokeObj = rect->getSVGStyle().getStroke();
//    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
//        // finalStrokeColor đã tính toán Opacity (từ getGdiColor())
//        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
//
//        if (finalStrokeColor.GetA() > 0) {
//            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
//            g.DrawRectangle(&pen, bounds);
//        }
//    }
//
//    // 4. KHÔI PHỤC MA TRẬN GRAPHICS CONTEXT
//    g.SetTransform(&oldMatrix);
//}
////void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle) {
////    if (!circle) return;
////    Gdiplus::PointF center = circle->getCenter();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
////    float radius = circle->getRadius();
////    Gdiplus::Color fill = circle->getSVGStyle().getFillColor();
////    Gdiplus::Color stroke = circle->getSVGStyle().getStroke()->strokeColor;
////    float strokeW = circle->getSVGStyle().getStroke()->strokeWidth;
////    Gdiplus::SolidBrush brush(fill);
////    Gdiplus::Pen pen(stroke, strokeW);
////    g.FillEllipse(&brush, center.X - radius, center.Y - radius, radius * 2, radius * 2);
////    g.DrawEllipse(&pen, center.X - radius, center.Y - radius, radius * 2, radius * 2);
////}
////void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle, const SVGDocumentContext& context) {
////    if (!circle) return;
////
////    // get infor
////    Gdiplus::PointF center = circle->getCenter();
////    float radius = circle->getRadius();
////
////    // get fill color
////    Gdiplus::SolidBrush brush(circle->getSVGStyle().getGdiFillColor());
////    //Gdiplus::Color fill = circle->getSVGStyle().getFillColor();
////    //Gdiplus::SolidBrush brush(fill);
////
////    // 1. fill the circle
////    float diameter = radius * 2;
////    g.FillEllipse(&brush, center.X - radius, center.Y - radius, diameter, diameter);
////
////    // --------------------------------------------------------
////    // 2. check and render stroke
////    // --------------------------------------------------------
////    Stroke* strokeObj = circle->getSVGStyle().getStroke();
////
////    if (strokeObj != nullptr) {
////        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
////        Gdiplus::Color strokeColor = strokeObj->strokeColor;
////        float strokeW = strokeObj->strokeWidth;
////        if (strokeW <= 0.0f) {
////            strokeW = 1.0f; // Đảm bảo độ rộng tối thiểu để viền hiển thị
////        }
////        // Tạo Pen
////        Gdiplus::Pen pen(strokeColor, strokeW);
////
////        // Vẽ stroke
////        g.DrawEllipse(&pen, center.X - radius, center.Y - radius, diameter, diameter);
////    }
////}
//void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle, const SVGDocumentContext& context) {
//    if (!circle) return;
//
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    // lưu ma trận hiện tại
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    // áp dụng ma trận biến đội
//    Gdiplus::Matrix localMatrix;
//    circle->getTransformMatrix().applyToMatrix(localMatrix);
//    //if (&localMatrix) {
//    //    //g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    //    g.MultiplyTransform(&localMatrix);
//    //}
//    g.MultiplyTransform(&localMatrix);
//    // 2. DÙNG TOẠ ĐỘ GỐC ĐỂ VẼ
//    Gdiplus::PointF center = circle->getCenter();
//    float radius = circle->getRadius();
//    float diameter = radius * 2;
//    Gdiplus::RectF bounds(center.X - radius, center.Y - radius, diameter, diameter);
//
//    // 1. LOGIC FILL
//    if (circle->getSVGStyle().hasGradientFill()) {
//        const SVGGradient* grad = circle->getSVGStyle().getGrad();
//        if (grad) {
//            renderGradientFill(g, grad, bounds, context);
//        }
//    }
//    else {
//        // TRƯỜNG HỢP MÀU ĐƠN
//        Gdiplus::Color fillColor = circle->getSVGStyle().getGdiFillColor();
//        if (fillColor.GetA() > 0) {
//            Gdiplus::SolidBrush brush(fillColor);
//            g.FillEllipse(&brush, bounds);
//        }
//    }
//
//    // 2. LOGIC STROKE
//    Stroke* strokeObj = circle->getSVGStyle().getStroke();
//    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
//        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
//
//        if (finalStrokeColor.GetA() > 0) {
//            // Cần bounds để vẽ
//            Gdiplus::PointF center = circle->getCenter();
//            float radius = circle->getRadius();
//            float diameter = radius * 2;
//            Gdiplus::RectF bounds(center.X - radius, center.Y - radius, diameter, diameter);
//
//            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
//            g.DrawEllipse(&pen, bounds);
//        }
//    }
//    g.SetTransform(&oldMatrix);
//}
////void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse) {
////    if (!ellipse) return;
////    PointF center = ellipse->getCenter();// trong CustomPoint có thể viết thêm chuyển đổi CustomPoint thành PointF
////    float rx = ellipse->getRadiusX();
////    float ry = ellipse->getRadiusY();
////    Gdiplus::SolidBrush brush(ellipse->getSVGStyle().getFillColor());
////    Gdiplus::Pen pen(ellipse->getSVGStyle().getStroke()->strokeColor, ellipse->getSVGStyle().getStroke()->strokeWidth);
////    g.FillEllipse(&brush, center.X - rx, center.Y - ry, rx * 2, ry * 2);
////    g.DrawEllipse(&pen, center.X - rx, center.Y - ry, rx * 2, ry * 2);
////}
////void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse, const SVGDocumentContext& context) {
////    if (!ellipse) return;
////
////    // Lấy thông tin cơ bản
////    PointF center = ellipse->getCenter();
////    float rx = ellipse->getRadiusX();
////    float ry = ellipse->getRadiusY();
////
////    // Khởi tạo Brush cho Fill
////    //Gdiplus::SolidBrush brush(ellipse->getSVGStyle().getFillColor());
////    Gdiplus::SolidBrush brush(ellipse->getSVGStyle().getGdiFillColor());
////    // 1. Fill hình elip
////    g.FillEllipse(&brush, center.X - rx, center.Y - ry, rx * 2, ry * 2);
////
////    // --------------------------------------------------------
////    // 2. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
////    // --------------------------------------------------------
////    
////    Stroke* strokeObj = ellipse->getSVGStyle().getStroke();
////    //Stroke* strokeObj = new Stroke();
////    if (strokeObj != nullptr) {
////        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
////        Gdiplus::Color strokeColor = strokeObj->strokeColor;
////        float strokeW = strokeObj->strokeWidth;
////
////        // Tạo Pen và vẽ
////        Gdiplus::Pen pen(strokeColor, strokeW);
////        g.DrawEllipse(&pen, center.X - rx, center.Y - ry, rx * 2, ry * 2);
////    }
////}
//void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse, const SVGDocumentContext& context) {
//    if (!ellipse) return;
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    // lưu ma trận hiện tại
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    // áp dụng ma trận biến đội
//    Gdiplus::Matrix localMatrix;
//    ellipse->getTransformMatrix().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//    //if (&localMatrix) {
//    //    //g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    //    g.MultiplyTransform(&localMatrix);
//    //}
//    Gdiplus::PointF center = ellipse->getCenter();
//    float rx = ellipse->getRadiusX();
//    float ry = ellipse->getRadiusY();
//    Gdiplus::RectF bounds(center.X - rx, center.Y - ry, rx * 2, ry * 2);
//
//    // 1. LOGIC FILL
//    if (ellipse->getSVGStyle().hasGradientFill()) {
//        const SVGGradient* grad = ellipse->getSVGStyle().getGrad();
//        if (grad) {
//            renderGradientFill(g, grad, bounds, context);
//        }
//    }
//    else {
//        Gdiplus::Color fillColor = ellipse->getSVGStyle().getGdiFillColor();
//        if (fillColor.GetA() > 0) {
//            Gdiplus::SolidBrush brush(fillColor);
//            g.FillEllipse(&brush, bounds);
//        }
//    }
//
//    // 2. LOGIC STROKE
//    Stroke* strokeObj = ellipse->getSVGStyle().getStroke();
//    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
//        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
//
//        if (finalStrokeColor.GetA() > 0) {
//            // Cần bounds để vẽ
//            Gdiplus::PointF center = ellipse->getCenter();
//            float rx = ellipse->getRadiusX();
//            float ry = ellipse->getRadiusY();
//            Gdiplus::RectF bounds(center.X - rx, center.Y - ry, rx * 2, ry * 2);
//
//            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
//            g.DrawEllipse(&pen, bounds);
//        }
//    }
//    g.SetTransform(&oldMatrix);
//}
////void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* line, const SVGDocumentContext& context) {
////    if (!line) return;
////
////    // Lấy thông tin điểm (giả định CustomPoint đã có toán tử chuyển đổi hoặc bạn truy cập x, y)
////    PointF start = line->getStartPoint();
////    PointF end = line->getEndPoint();
////
////    // --------------------------------------------------------
////    // 1. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
////    // --------------------------------------------------------
////    Stroke* strokeObj = line->getSVGStyle().getStroke();
////
////    if (strokeObj != nullptr) {
////        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
////        Gdiplus::Color strokeColor = strokeObj->strokeColor;
////        float strokeW = strokeObj->strokeWidth;
////
////        // Tạo Pen và vẽ đường thẳng
////        Gdiplus::Pen pen(strokeColor, strokeW);
////        g.DrawLine(&pen, start, end);
////    }
////}
//void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* line, const SVGDocumentContext& context) {
//    if (!line) return;
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    // lưu ma trận hiện tại
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    // áp dụng ma trận biến đội
//    Gdiplus::Matrix localMatrix;
//    line->getTransformMatrix().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//    //if (&localMatrix) {
//    //    //g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    //    g.MultiplyTransform(&localMatrix);
//    //}
//    
//
//    Gdiplus::PointF start = line->getStartPoint();
//    Gdiplus::PointF end = line->getEndPoint();
//
//    // 1. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
//    Stroke* strokeObj = line->getSVGStyle().getStroke();
//
//    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
//        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth;
//
//        if (finalStrokeColor.GetA() > 0) {
//            // FIX: Gradient Stroke Logic (Nếu cần)
//            // Hiện tại Line không thể fill, nên ta chỉ cần xử lý Stroke
//
//            Gdiplus::Pen pen(finalStrokeColor, strokeW);
//            g.DrawLine(&pen, start, end);
//        }
//    }
//    g.SetTransform(&oldMatrix);
//}
//
////void SVGRenderer::renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon, const SVGDocumentContext& context) {
////    if (!polygon) return;
////    const std::vector<CustomPoint>& customPoints = polygon->getPoints();
////    std::vector<PointF> gdiPoints;
////    gdiPoints.reserve(customPoints.size());
////    for (const auto& cp : customPoints) {
////        gdiPoints.emplace_back(cp.x, cp.y);
////    }
////    if (gdiPoints.empty()) return;
////
////    // 1. Fill Polygon
////    //Gdiplus::SolidBrush brush(polygon->getSVGStyle().getFillColor());
////    Gdiplus::SolidBrush brush(polygon->getSVGStyle().getGdiFillColor());
////    g.FillPolygon(&brush, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
////
////    // 2. KIỂM TRA NULL TRƯỚC KHI TRUY CẬP STROKE
////    Stroke* strokeObj = polygon->getSVGStyle().getStroke();
////
////    if (strokeObj != nullptr) {
////        // Tạo Pen và vẽ
////        Gdiplus::Pen pen(strokeObj->strokeColor, strokeObj->strokeWidth);
////        g.DrawPolygon(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
////    }
////}
//void SVGRenderer::renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon, const SVGDocumentContext& context) {
//    if (!polygon) return;
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    // lưu ma trận hiện tại
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    // áp dụng ma trận biến đội
//    Gdiplus::Matrix localMatrix;
//    polygon->getTransformMatrix().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//    //if (&localMatrix) {
//    //    //g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    //    g.MultiplyTransform(&localMatrix);
//    //}
//
//    const std::vector<CustomPoint>& customPoints = polygon->getPoints();
//    std::vector<Gdiplus::PointF> gdiPoints;
//    gdiPoints.reserve(customPoints.size());
//    for (const auto& cp : customPoints) {
//        gdiPoints.emplace_back(cp.x, cp.y);
//    }
//    if (gdiPoints.empty()) return;
//
//    Gdiplus::GraphicsPath path;
//    path.AddLines(gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//    path.CloseFigure();
//    Gdiplus::RectF bounds;
//    path.GetBounds(&bounds);
//
//    // 2. LOGIC FILL (Gọi renderGradientPathFill)
//    if (polygon->getSVGStyle().hasGradientFill()) {
//        const SVGGradient* grad = polygon->getSVGStyle().getGrad();
//        if (grad) {
//            // Dùng hàm hỗ trợ tô Path bằng Gradient
//            renderGradientPathFill(g, grad, path, bounds, context);
//        }
//    }
//    else {
//        // TRƯỜNG HỢP MÀU ĐƠN
//        Gdiplus::Color fillColor = polygon->getSVGStyle().getGdiFillColor();
//        if (fillColor.GetA() > 0) {
//            Gdiplus::SolidBrush brush(fillColor);
//            g.FillPath(&brush, &path);
//        }
//    }
//
//    // 3. LOGIC STROKE (CHỈ MÀU ĐƠN)
//    Stroke* strokeObj = polygon->getSVGStyle().getStroke();
//    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
//        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
//        if (finalStrokeColor.GetA() > 0) {
//            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
//            g.DrawPath(&pen, &path); // Vẽ Path
//        }
//    }
//    g.SetTransform(&oldMatrix);
//}
//
//
////void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline) {
////    if (!polyline) return;
////
////    std::vector<PointF> gdiPoints;
////    const auto& customPoints = polyline->getPoints();// tương tự 
////    for (const auto& cp : customPoints) {
////        gdiPoints.emplace_back(cp.x, cp.y);
////    }
////    if (gdiPoints.size() < 2) return;
////    Gdiplus::Pen pen(polyline->getSVGStyle().getStroke()->strokeColor, polyline->getSVGStyle().getStroke()->strokeWidth);// tương tự 
////    g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
////}
////void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline) {
////    if (!polyline) return;
////
////    // 1. Chuyển đổi điểm
////    std::vector<PointF> gdiPoints;
////    const auto& customPoints = polyline->getPoints();
////
////    // Giả sử CustomPoint có thành viên công khai x, y
////    for (const auto& cp : customPoints) {
////        gdiPoints.emplace_back(cp.x, cp.y);
////    }
////
////    if (gdiPoints.size() < 2) return;
////
////    // --------------------------------------------------------
////    // 2. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
////    // --------------------------------------------------------
////    Stroke* strokeObj = polyline->getSVGStyle().getStroke();
////
////    if (strokeObj != nullptr) {
////        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
////        Gdiplus::Color strokeColor = strokeObj->strokeColor;
////        float strokeW = strokeObj->strokeWidth;
////
////        // Tạo Pen và vẽ đường gấp khúc
////        Gdiplus::Pen pen(strokeColor, strokeW);
////        g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
////    }
////}
////void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline, const SVGDocumentContext& context) {
////    if (!polyline) return;
////
////    // 1. Chuyển đổi điểm
////    std::vector<PointF> gdiPoints;
////    const auto& customPoints = polyline->getPoints();
////
////    // Giả sử CustomPoint có thành viên công khai x, y
////    for (const auto& cp : customPoints) {
////        gdiPoints.emplace_back(cp.x, cp.y);
////    }
////
////    if (gdiPoints.size() < 2) return;
////
////    // Lấy màu Fill đã có Opacity
////    Gdiplus::Color fillColor = polyline->getSVGStyle().getGdiFillColor();
////
////    // --------------------------------------------------------
////    // 2. TÔ MÀU FILL (Nếu Alpha > 0)
////    // --------------------------------------------------------
////    // Nếu fill được định nghĩa và opacity > 0, tô màu nó như một Polygon đóng
////    if (fillColor.GetA() > 0) {
////        Gdiplus::SolidBrush brush(fillColor);
////        // Sử dụng FillPolygon để tô màu (xem như hình đóng)
////        g.FillPolygon(&brush, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
////    }
////    // --------------------------------------------------------
////
////    // 3. Kiểm tra và Vẽ Stroke
////    Stroke* strokeObj = polyline->getSVGStyle().getStroke();
////
////    if (strokeObj != nullptr) {
////        // Lấy thuộc tính stroke từ đối tượng Stroke hợp lệ
////        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
////        float strokeW = strokeObj->strokeWidth;
////
////        // Ép buộc độ rộng tối thiểu
////        if (strokeW <= 0.0f) {
////            strokeW = 1.0f;
////        }
////
////        // Tạo Pen và vẽ đường gấp khúc (DrawLines)
////        Gdiplus::Pen pen(finalStrokeColor, strokeW);
////        g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
////    }
////}
//void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline, const SVGDocumentContext& context) {
//    if (!polyline) return;
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    // lưu ma trận hiện tại
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    // áp dụng ma trận biến đội
//    Gdiplus::Matrix localMatrix;
//    polyline->getTransformMatrix().applyToMatrix(localMatrix);
//    //g.MultiplyTransform(&localMatrix);
//    if (&localMatrix) {
//        //g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//        g.MultiplyTransform(&localMatrix);
//    }
//
//    // 1. Chuyển đổi điểm
//    std::vector<Gdiplus::PointF> gdiPoints;
//    const auto& customPoints = polyline->getPoints();
//    for (const auto& cp : customPoints) {
//        gdiPoints.emplace_back(cp.x, cp.y);
//    }
//    if (gdiPoints.size() < 2) return;
//
//    // 1.1. TẠO GRAPHICS PATH VÀ TÍNH BOUNDS (Bắt buộc cho Fill và Gradient)
//    Gdiplus::GraphicsPath path;
//    path.AddLines(gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//    path.CloseFigure(); // Xử lý như Polygon đóng cho Fill
//
//    Gdiplus::RectF bounds;
//    path.GetBounds(&bounds); // Lấy Bounding Box chính xác
//
//    // 2. LOGIC FILL (Gradient HOẶC Màu đơn)
//    if (polyline->getSVGStyle().hasGradientFill()) {
//        const SVGGradient* grad = polyline->getSVGStyle().getGrad();
//        if (grad) {
//            // GỌI HÀM HỖ TRỢ FILL PATH BẰNG GRADIENT
//            renderGradientPathFill(g, grad, path, bounds, context);
//        }
//    }
//    else {
//        // TRƯỜNG HỢP MÀU ĐƠN (Solid Fill)
//        Gdiplus::Color fillColor = polyline->getSVGStyle().getGdiFillColor();
//
//        if (fillColor.GetA() > 0 && polyline->getSVGStyle().getFillOpacity() > 0) {
//            Gdiplus::SolidBrush brush(fillColor);
//            // Dùng FillPath sau khi Path đã được đóng
//            g.FillPath(&brush, &path);
//        }
//    }
//
//    // 3. Kiểm tra và Vẽ Stroke (Chỉ cần vẽ đường, không cần đóng Path)
//    Stroke* strokeObj = polyline->getSVGStyle().getStroke();
//    if (strokeObj != nullptr) {
//        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth;
//
//        if (finalStrokeColor.GetA() > 0) {
//            // Tạo Pen và vẽ đường gấp khúc (DrawLines)
//            Gdiplus::Pen pen(finalStrokeColor, strokeW);
//            // Dùng DrawLines thay vì DrawPath để vẽ Polyline MỞ (chuẩn SVG)
//            g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//        }
//    }
//
//    g.SetTransform(&oldMatrix);
//}
////void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text, const SVGDocumentContext& context) {
////    if (!text) return;
////    PointF pos = text->getStart();// tương tự 
////    std::wstring content = text->getContent();
////    Color color = text->getSVGStyle().getGdiFillColor();// tương tự 
////    float fontSize = text->getFontSize();
////    Gdiplus::FontFamily fontFamily(L"Times New Roman");
////    Gdiplus::Font font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
////    Gdiplus::SolidBrush brush(color);
////    PointF new_pos(pos.X, pos.Y - text->getFontSize() * 1.0f);
////    g.DrawString(content.c_str(), -1, &font, new_pos, &brush);
////}
//void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text, const SVGDocumentContext& context) {
//    if (!text) return;
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    // lưu ma trận hiện tại
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    // áp dụng ma trận biến đội
//    Gdiplus::Matrix localMatrix;
//    text->getTransformMatrix().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//    //if (&localMatrix) {
//    //    //g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    //    g.MultiplyTransform(&localMatrix);
//    //}
//
//    // Lấy thông tin cơ bản
//    Gdiplus::PointF pos = text->getStart();
//    std::wstring content = text->getContent();
//    float fontSize = text->getFontSize();
//
//    // Gdiplus Font setup
//    Gdiplus::FontFamily fontFamily(L"Times New Roman"); // Hoặc lấy từ Style nếu có
//    Gdiplus::Font font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
//
//    // Vị trí baseline (Điều chỉnh Y)
//    Gdiplus::PointF new_pos(pos.X, pos.Y - text->getFontSize() * 0.8f);
//
//    // --- LOGIC FILL: GRADIENT HAY SOLID ---
//
//    if (text->getSVGStyle().hasGradientFill()) {
//
//        // 1. TẠO GRAPHICS PATH TỪ TEXT (BẮT BUỘC cho Gradient)
//        Gdiplus::GraphicsPath path;
//
//        // Cờ định dạng để đảm bảo căn chỉnh đúng (FormatFlagsNoWrap là mặc định tốt)
//        Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());
//
//        // Thêm chuỗi văn bản vào Path
//        Gdiplus::Status status = path.AddString(
//            content.c_str(),
//            static_cast<INT>(content.length()),
//            &fontFamily,
//            Gdiplus::FontStyleRegular,
//            fontSize,
//            new_pos, // Điểm bắt đầu
//            &format
//        );
//
//        if (status == Gdiplus::Ok) {
//            Gdiplus::RectF bounds;
//            path.GetBounds(&bounds); // Lấy Bounding Box của Text Path
//
//            const SVGGradient* grad = text->getSVGStyle().getGrad();
//            if (grad) {
//                // Sử dụng hàm đã có để tô Path bằng Gradient
//                renderGradientPathFill(g, grad, path, bounds, context);
//            }
//        }
//    }
//    else {
//        // TRƯỜNG HỢP MÀU ĐƠN (Solid Fill)
//        Gdiplus::Color color = text->getSVGStyle().getGdiFillColor();
//
//        if (color.GetA() > 0) {
//            Gdiplus::SolidBrush brush(color);
//
//            // Sử dụng DrawString truyền thống cho Solid Fill
//            g.DrawString(content.c_str(), -1, &font, new_pos, &brush);
//        }
//    }
//
//    // --- LOGIC STROKE CHO TEXT (Nếu cần) ---
//    // GDI+ không hỗ trợ Stroke Text bằng Pen dễ dàng như SVG.
//    // Nếu bạn muốn Stroke Text, bạn phải dùng Path::Widen và tô màu Path đó.
//    // Tạm thời bỏ qua logic Stroke phức tạp này để giữ Text hoạt động.
//
//    g.SetTransform(&oldMatrix);
//}
//
////void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* square, const SVGDocumentContext& context) {
////    if (!square) return;
////    PointF topLeft = square->getTopLeftCorner();
////    float side = square->getHeight();// tương tự 
////    Gdiplus::SolidBrush brush(square->getSVGStyle().getGdiFillColor());
////    Gdiplus::Pen pen(square->getSVGStyle().getStroke()->strokeColor, square->getSVGStyle().getStroke()->strokeWidth);
////    g.FillRectangle(&brush, topLeft.X, topLeft.Y, side, side);
////    g.DrawRectangle(&pen, topLeft.X, topLeft.Y, side, side);
////}
//void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* square, const SVGDocumentContext& context) {
//    if (!square) return;
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    // lưu ma trận hiện tại
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    // áp dụng ma trận biến đội
//    Gdiplus::Matrix localMatrix;
//    square->getTransformMatrix().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//    //if (&localMatrix) {
//    //    //g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    //    g.MultiplyTransform(&localMatrix);
//    //}
//
//    // Sử dụng renderRectangle vì Square là Rect đặc biệt
//    SVGRectangle rect; // Tạo rect tạm thời
//    rect.setTopLeftCorner(square->getTopLeftCorner());
//    rect.setWidth(square->getHeight());
//    rect.setHeight(square->getHeight());
//    rect.setSVGStyle(square->getSVGStyle()); // Kế thừa Style
//
//    renderRectangle(g, &rect, context);
//    
//    g.SetTransform(&oldMatrix);
//}
//void SVGRenderer::renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup, const SVGDocumentContext& context) {
//    if (!rootGroup) return;
//    // FIX: Truyền Context xuống hàm Group
//    rootGroup->render(*this, g, context);
//    //renderGroup(g, rootGroup, context);
//}
//void SVGRenderer::renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup, const SVGDocumentContext& context) {
//    if (!rootGroup) return;
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    // lưu ma trận hiện tại
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    // áp dụng ma trận biến đội
//    Gdiplus::Matrix groupMatrix;
//    rootGroup->getTransformMatrix().applyToMatrix(groupMatrix);
//    g.MultiplyTransform(&groupMatrix);
//    //if (&groupMatrix) {
//    //    // dùng tempMatrix để sửa đổi (nếu cần) mà không thay đổi SVGGroup
//    //    Gdiplus::Matrix* tempMatrix = groupMatrix.Clone();
//    //    //g.MultiplyTransform(tempMatrix, Gdiplus::MatrixOrderPrepend);
//    //    g.MultiplyTransform(tempMatrix);
//    //    delete tempMatrix;
//    //}
//    const auto& children = rootGroup->getSVGElementArray();
//    for (SVGElement* element : children) {
//        // FIX: Double Dispatch: Truyền Context xuống các element con
//        if (element) element->render(*this, g, context);
//    }
//    g.SetTransform(&oldMatrix);
//}
//
//void SVGRenderer::renderGradientPathFill(Gdiplus::Graphics& g, const SVGGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds, const SVGDocumentContext& context) const {
//    using namespace Gdiplus;
//    if (!grad) return;
//
//    const SVGGradient* resolvedGrad = grad;
//    if (!resolvedGrad->resolveReference(context)) return;
//
//    if (auto* linear = dynamic_cast<const SVGLinearGradient*>(grad)) {
//        const std::vector<SVGStop>& stops = linear->getStopArray();
//        if (stops.empty()) return;
//
//        PointF p1(bounds.X + linear->getX1() * bounds.Width, bounds.Y + linear->getY1() * bounds.Height);
//        PointF p2(bounds.X + linear->getX2() * bounds.Width, bounds.Y + linear->getY2() * bounds.Height);
//
//        Color c1 = (Color)stops.front().getStopColor();
//        Color c2 = (stops.size() > 1) ? (Color)stops.back().getStopColor() : c1;
//
//        LinearGradientBrush brush(p1, p2, c1, c2);
//        brush.SetWrapMode(getGdiWrapMode(linear->getSpreadMethod()));
//        g.FillPath(&brush, &path); // *** Fill Path ***
//    }
//    else if (auto* radial = dynamic_cast<const SVGRadialGradient*>(grad)) {
//        // Tối giản Radial Fill trên Path: Dùng Solid Fill bằng màu Center
//        const std::vector<SVGStop>& stops = radial->getStopArray();
//        if (stops.empty()) return;
//
//        Color centerColor = (Color)stops.front().getStopColor();
//        if (centerColor.GetA() > 0) {
//            SolidBrush brush(centerColor);
//            g.FillPath(&brush, &path); // *** Fill Path ***
//        }
//    }
//}
//
////render path
////void SVGRenderer::renderPath(Gdiplus::Graphics& g, SVGPath* Path, vector<PathCommand> commands, const SVGDocumentContext& context) const {
////    using namespace Gdiplus;
////
////    // Lưu transform hiện tại
////    Matrix oldMatrix;
////    g.GetTransform(&oldMatrix);
////
////    // Áp dụng transform của path (nếu có)
////    if (Path && Path->transform) {
////        // MultiplyTransform: kết hợp transform hiện tại với transform của path
////        // Chọn MatrixOrderPrepend hoặc Append tuỳ mục tiêu (prepend = local -> then world)
////        g.MultiplyTransform(Path->transform, MatrixOrderPrepend);
////    }
////
////    Pen pen(Color(255, 0, 0, 0), 2.0f); // Black pen with width 2.0f)
////    PointF currentPoint(0.0f, 0.0f), startPoint(0.0f, 0.0f);
////
////    for (const auto& cmd : commands) {
////        switch (cmd.command) {
////        case 'M': {
////            currentPoint = PointF(cmd.parameters[0], cmd.parameters[1]);
////            startPoint = currentPoint;
////            break;
////        }
////        case 'L': {
////            PointF p(cmd.parameters[0], cmd.parameters[1]);
////            g.DrawLine(&pen, currentPoint, p);
////            currentPoint = p;
////            break;
////        }
////        case 'H': {
////            float x = cmd.parameters[0];
////            PointF p(x, currentPoint.Y);
////            g.DrawLine(&pen, currentPoint, p);
////
////            currentPoint = p;
////            break;
////        }
////        case 'V': {
////            float y = cmd.parameters[0];
////            PointF p(currentPoint.X, y);
////            g.DrawLine(&pen, currentPoint, p);
////            currentPoint = p;
////            break;
////        }
////        case 'Z': {
////            g.DrawLine(&pen, currentPoint, startPoint);
////            currentPoint = startPoint;
////            break;
////        }
////        case 'C': {
////            PointF p1(cmd.parameters[0], cmd.parameters[1]);
////            PointF p2(cmd.parameters[2], cmd.parameters[3]);
////            PointF p3(cmd.parameters[4], cmd.parameters[5]);
////
////            g.DrawBezier(&pen, currentPoint, p1, p2, p3);
////            currentPoint = p3;
////            break;
////        }
////        }
////    }
////    g.SetTransform(&oldMatrix); // Khôi phục transform ban đầu
////}
//
////void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* pathElement, const std::vector<PathCommand>& commands, const SVGDocumentContext& context) const {
////    if (!pathElement) return;
////    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
////    // lưu ma trận hiện tại
////    Gdiplus::Matrix oldMatrix;
////    g.GetTransform(&oldMatrix);
////    // áp dụng ma trận biến đội
////    const Gdiplus::Matrix* matrix = pathElement->getTransformMatrix();
////    if (matrix) {
////        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
////    }
////
////    // 1. XÂY DỰNG GRAPHICS PATH TỪ CÁC COMMANDS
////    Gdiplus::GraphicsPath path;
////    Gdiplus::PointF currentPoint(0.0f, 0.0f), startPoint(0.0f, 0.0f);
////    bool subpathStarted = false;
////
////    for (const auto& cmd : commands) {
////        // 
////        switch (cmd.command) {
////        case 'M':
////        case 'm': {
////            // M: MoveTo
////            currentPoint = Gdiplus::PointF(cmd.parameters[0], cmd.parameters[1]);
////            startPoint = currentPoint;
////            if (subpathStarted) {
////                path.StartFigure(); // Bắt đầu hình dạng mới
////            }
////            subpathStarted = true;
////            break;
////        }
////        case 'L':
////        case 'l': {
////            // L: LineTo
////            Gdiplus::PointF p(cmd.parameters[0], cmd.parameters[1]);
////            path.AddLine(currentPoint, p);
////            currentPoint = p;
////            break;
////        }
////                // ... (Thêm các lệnh khác H, V, Q, S, A nếu cần) ...
////        case 'H': { // Horizontal LineTo
////            float x = cmd.parameters[0];
////            Gdiplus::PointF p(x, currentPoint.Y);
////            path.AddLine(currentPoint, p);
////            currentPoint = p;
////            break;
////        }
////        case 'V': { // Vertical LineTo
////            float y = cmd.parameters[0];
////            Gdiplus::PointF p(currentPoint.X, y);
////            path.AddLine(currentPoint, p);
////            currentPoint = p;
////            break;
////        }
////        case 'C': {
////            // C: Cubic Bezier
////            Gdiplus::PointF p1(cmd.parameters[0], cmd.parameters[1]);
////            Gdiplus::PointF p2(cmd.parameters[2], cmd.parameters[3]);
////            Gdiplus::PointF p3(cmd.parameters[4], cmd.parameters[5]);
////            path.AddBezier(currentPoint, p1, p2, p3);
////            currentPoint = p3;
////            break;
////        }
////        case 'Z':
////        case 'z': {
////            // Z: ClosePath
////            path.CloseFigure();
////            currentPoint = startPoint;
////            subpathStarted = false;
////            break;
////        }
////        }
////    }
////
////    // 2. TÍNH TOÁN BOUNDS
////    Gdiplus::RectF bounds;
////    path.GetBounds(&bounds);
////
////    // 3. LOGIC FILL (Gradient HOẶC Màu đơn)
////    if (pathElement->getSVGStyle().hasGradientFill()) {
////        const SVGGradient* grad = pathElement->getSVGStyle().getGrad();
////        if (grad) {
////            // Sử dụng hàm hỗ trợ Fill Path (Đã sửa ở câu trước)
////            renderGradientPathFill(g, grad, path, bounds, context);
////        }
////    }
////    else {
////        // TRƯỜNG HỢP MÀU ĐƠN
////        Gdiplus::Color fillColor = pathElement->getSVGStyle().getGdiFillColor();
////        if (fillColor.GetA() > 0) {
////            Gdiplus::SolidBrush brush(fillColor);
////            g.FillPath(&brush, &path);
////        }
////    }
////
////    // 4. LOGIC STROKE (CHỈ MÀU ĐƠN)
////    Stroke* strokeObj = pathElement->getSVGStyle().getStroke();
////    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
////        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
////        if (finalStrokeColor.GetA() > 0) {
////            // Pen chỉ dùng màu đơn (theo quyết định trước đó)
////            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
////            g.DrawPath(&pen, &path);
////        }
////    }
////
////    // Khôi phục transform ban đầu
////    g.SetTransform(&oldMatrix);
////}
//
////void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* pathElement, const std::vector<PathCommand>& commands, const SVGDocumentContext& context)
////void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* path) {
////    if (!path) return;
////    Matrix oldMatrix; g.GetTransform(&oldMatrix);
////    Matrix localMatrix; path->getTransform().applyToMatrix(localMatrix);
////    g.MultiplyTransform(&localMatrix);
////
////    Gdiplus::GraphicsPath gp;
////    PointF current(0.0f, 0.0f);
////
////    // Mặc định SVG dùng luật Winding, nhưng GDI+ mặc định là Alternate.
////    // Nếu hình vẫn bị thủng lỗ sai, hãy thử bật dòng dưới:
////    // gp.SetFillMode(Gdiplus::FillModeWinding);
////
////    for (const auto& cmd : path->getCommands()) {
////        switch (cmd.type) {
////        case PathCommandType::MoveTo: {
////            gp.StartFigure(); // QUAN TRỌNG: Ngắt nét để không nối với điểm cũ
////            current = PointF(cmd.params[0], cmd.params[1]);
////            break;
////        }
////        case PathCommandType::LineTo: {
////            PointF p(cmd.params[0], cmd.params[1]);
////            gp.AddLine(current, p);
////            current = p;
////            break;
////        }
////        case PathCommandType::HLineTo: {
////            PointF p(cmd.params[0], current.Y);
////            gp.AddLine(current, p);
////            current = p;
////            break;
////        }
////        case PathCommandType::VLineTo: {
////            PointF p(current.X, cmd.params[0]);
////            gp.AddLine(current, p);
////            current = p;
////            break;
////        }
////        case PathCommandType::ClosePath: {
////            gp.CloseFigure(); // QUAN TRỌNG: Đóng hình chuẩn
////            Gdiplus::PointF lastPoint;
////            gp.GetLastPoint(&lastPoint);
////            current = lastPoint;
////            break;
////        }
////        case PathCommandType::CubicBezier: {
////            PointF p1(cmd.params[0], cmd.params[1]);
////            PointF p2(cmd.params[2], cmd.params[3]);
////            PointF p3(cmd.params[4], cmd.params[5]);
////            gp.AddBezier(current, p1, p2, p3);
////            current = p3;
////            break;
////        }
////        case PathCommandType::SmoothCubicBezier: {
////            PointF p1(cmd.params[0], cmd.params[1]);
////            PointF p2(cmd.params[2], cmd.params[3]);
////            PointF p3(cmd.params[4], cmd.params[5]);
////            gp.AddBezier(current, p1, p2, p3);
////            current = p3;
////            break;
////        }
////        }
////    }
////
////    const SVGStyle& st = path->getSVGStyle();
////    Color fillColor = st.getGdiFillColor();
////    Stroke* strokeObj = st.getStroke();
////
////    if (fillColor.GetA() > 0) {
////        SolidBrush brush(fillColor);
////        g.FillPath(&brush, &gp);
////    }
////    if (strokeObj) {
////        Color strokeColor = strokeObj->getGdiColor();
////        float strokeW = strokeObj->strokeWidth > 0 ? strokeObj->strokeWidth : 1.0f;
////        Pen pen(strokeColor, strokeW);
////        g.DrawPath(&pen, &gp);
////    }
////    g.SetTransform(&oldMatrix);
////}
//
//
////void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* path) {
////    if (!path) return;
////    Matrix oldMatrix; g.GetTransform(&oldMatrix);
////    Matrix localMatrix; path->getTransformMatrix().applyToMatrix(localMatrix);
////    g.MultiplyTransform(&localMatrix);
////
////    Gdiplus::GraphicsPath gp;
////    PointF current(0.0f, 0.0f);
////
////    // Mặc định SVG dùng luật Winding, nhưng GDI+ mặc định là Alternate.
////    // Nếu hình vẫn bị thủng lỗ sai, hãy thử bật dòng dưới:
////    // gp.SetFillMode(Gdiplus::FillModeWinding);
////
////    for (const auto& cmd : path->getCommands()) {
////        switch (cmd.type) {
////        case PathCommandType::MoveTo: {
////            gp.StartFigure(); // QUAN TRỌNG: Ngắt nét để không nối với điểm cũ
////            current = PointF(cmd.params[0], cmd.params[1]);
////            break;
////        }
////        case PathCommandType::LineTo: {
////            PointF p(cmd.params[0], cmd.params[1]);
////            gp.AddLine(current, p);
////            current = p;
////            break;
////        }
////        case PathCommandType::HLineTo: {
////            PointF p(cmd.params[0], current.Y);
////            gp.AddLine(current, p);
////            current = p;
////            break;
////        }
////        case PathCommandType::VLineTo: {
////            PointF p(current.X, cmd.params[0]);
////            gp.AddLine(current, p);
////            current = p;
////            break;
////        }
////        case PathCommandType::ClosePath: {
////            gp.CloseFigure(); // QUAN TRỌNG: Đóng hình chuẩn
////            Gdiplus::PointF lastPoint;
////            gp.GetLastPoint(&lastPoint);
////            current = lastPoint;
////            break;
////        }
////        case PathCommandType::CubicBezier: {
////            PointF p1(cmd.params[0], cmd.params[1]);
////            PointF p2(cmd.params[2], cmd.params[3]);
////            PointF p3(cmd.params[4], cmd.params[5]);
////            gp.AddBezier(current, p1, p2, p3);
////            current = p3;
////            break;
////        }
////        case PathCommandType::SmoothCubicBezier: {
////            PointF p1(cmd.params[0], cmd.params[1]);
////            PointF p2(cmd.params[2], cmd.params[3]);
////            PointF p3(cmd.params[4], cmd.params[5]);
////            gp.AddBezier(current, p1, p2, p3);
////            current = p3;
////            break;
////        }
////        }
////    }
////
////    const SVGStyle& st = path->getSVGStyle();
////    Color fillColor = st.getGdiFillColor();
////    Stroke* strokeObj = st.getStroke();
////
////    if (fillColor.GetA() > 0) {
////        SolidBrush brush(fillColor);
////        g.FillPath(&brush, &gp);
////    }
////    if (strokeObj) {
////        Color strokeColor = strokeObj->getGdiColor();
////        float strokeW = strokeObj->strokeWidth > 0 ? strokeObj->strokeWidth : 1.0f;
////        Pen pen(strokeColor, strokeW);
////        g.DrawPath(&pen, &gp);
////    }
////    g.SetTransform(&oldMatrix);
////}
//
//
//
//void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* pathElement, const SVGDocumentContext& context) const {
//    if (!pathElement) return;
//
//    // Lấy commands từ đối tượng Path
//    const std::vector<PathCommand>& commands = pathElement->getCommands(); // Giả sử bạn có hàm getter này
//
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//
//    Gdiplus::Matrix localMatrix;
//    pathElement->getTransformMatrix().applyToMatrix(localMatrix); // Thừa kế từ SVGElement
//    g.MultiplyTransform(&localMatrix);
//    //if (&localMatrix) {
//    //    //g.MultiplyTransform(matrix, MatrixOrderPrepend);
//    //    g.MultiplyTransform(&localMatrix);
//    //}
//
//    // 2. XÂY DỰNG GRAPHICS PATH TỪ CÁC COMMANDS
//    GraphicsPath path;
//    PointF currentPoint(0.0f, 0.0f); // Vị trí điểm hiện tại
//    PointF startPoint(0.0f, 0.0f);  // Vị trí điểm đầu tiên của subpath
//
//    // NOTE: Cú pháp parse của bạn (M, L, C, S...) đã chuyển đổi tất cả sang tọa độ tuyệt đối
//    // (Relative coordinates đã được tính toán thành Absolute trong hàm parse).
//    // Mặc định SVG dùng luật Winding, nhưng GDI+ mặc định là Alternate.
//    // Nếu hình vẫn bị thủng lỗ sai, hãy thử bật dòng dưới:
//    // gp.SetFillMode(Gdiplus::FillModeWinding);
//
//    for (const auto& cmd : commands) {
//        // Kiểm tra số lượng tham số an toàn (Dù logic parse đã kiểm tra)
//        if (cmd.params.empty() && cmd.type != PathCommandType::ClosePath) continue;
//
//        switch (cmd.type) {
//        case PathCommandType::MoveTo: {
//            path.StartFigure(); // Bắt đầu một hình dạng mới
//            currentPoint = PointF(cmd.params[0], cmd.params[1]);
//            startPoint = currentPoint;
//            break;
//        }
//        case PathCommandType::LineTo: {
//            PointF p(cmd.params[0], cmd.params[1]);
//            path.AddLine(currentPoint, p);
//            currentPoint = p;
//            break;
//        }
//        case PathCommandType::HLineTo: {
//            PointF p(cmd.params[0], currentPoint.Y); // Y giữ nguyên
//            path.AddLine(currentPoint, p);
//            currentPoint = p;
//            break;
//        }
//        case PathCommandType::VLineTo: {
//            PointF p(currentPoint.X, cmd.params[0]); // X giữ nguyên
//            path.AddLine(currentPoint, p);
//            currentPoint = p;
//            break;
//        }
//        case PathCommandType::ClosePath: {
//            path.CloseFigure(); // QUAN TRỌNG: Đóng hình chuẩn
//            Gdiplus::PointF lastPoint;
//            path.GetLastPoint(&lastPoint);
//            currentPoint = lastPoint;
//            break;
//        }
//        case PathCommandType::CubicBezier: {
//            // Cần 3 cặp tọa độ (p1, p2, p3). p0 là currentPoint.
//            PointF p1(cmd.params[0], cmd.params[1]);
//            PointF p2(cmd.params[2], cmd.params[3]);
//            PointF p3(cmd.params[4], cmd.params[5]);
//            path.AddBezier(currentPoint, p1, p2, p3);
//            currentPoint = p3;
//            break;
//        }
//        case PathCommandType::SmoothCubicBezier: {
//            // Lệnh S đã tính toán p1, y1 (điểm điều khiển đối xứng)
//            // và lưu chúng vào pc.params[0], [1].
//            PointF p1(cmd.params[0], cmd.params[1]); // Điểm điều khiển đối xứng (đã tính trong parse)
//            PointF p2(cmd.params[2], cmd.params[3]);
//            PointF p3(cmd.params[4], cmd.params[5]);
//            path.AddBezier(currentPoint, p1, p2, p3);
//            currentPoint = p3;
//            break;
//        }
//        //case PathCommandType::ClosePath: {
//        //    path.CloseFigure();
//        //    currentPoint = startPoint;
//        //    break;
//        //}
//        default:
//            // Bỏ qua các lệnh chưa hỗ trợ (Q, A...)
//            break;
//        }
//    }
//
//    // 3. XỬ LÝ FILL VÀ STROKE
//
//    // Lấy Bounds để sử dụng cho Gradient/Brush
//    RectF bounds;
//    path.GetBounds(&bounds);
//
//    // 3A. FILL (Gradient HOẶC Màu đơn)
//    if (pathElement->getSVGStyle().hasGradientFill()) {
//        const SVGGradient* grad = pathElement->getSVGStyle().getGrad();
//        if (grad) {
//            // Sử dụng hàm hỗ trợ Fill Path
//            renderGradientPathFill(g, grad, path, bounds, context);
//        }
//    }
//    else {
//        // TRƯỜNG HỢP MÀU ĐƠN
//        Color fillColor = pathElement->getSVGStyle().getGdiFillColor();
//        if (fillColor.GetA() > 0) {
//            SolidBrush brush(fillColor);
//            g.FillPath(&brush, &path);
//        }
//    }
//    
//    // 3B. STROKE
//    const SVGStyle& st = pathElement->getSVGStyle();
//    Gdiplus::Color fillColor = st.getGdiFillColor();
//    Stroke* strokeObj = st.getStroke();
//    if (fillColor.GetA() > 0) {
//        Gdiplus::SolidBrush bush(fillColor);
//        g.FillPath(&bush, &path);
//    }
//    if (strokeObj) {
//        // Giả định getGdiColor() trả về màu đã tính opacity
//        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth > 0 ? strokeObj->strokeWidth : 1.0f;
//        //if (finalStrokeColor.GetA() > 0) {
//        //    Pen pen(finalStrokeColor, strokeObj->strokeWidth);
//        //    // Có thể thêm logic cho MiterLimit/LineCap/LineJoin nếu cần
//        //    g.DrawPath(&pen, &path);
//        //}
//        Gdiplus::Pen pen(finalStrokeColor, strokeW);
//        g.DrawPath(&pen, &path);
//    }
//
//    // 4. KHÔI PHỤC TRANSFORM
//    g.SetTransform(&oldMatrix);
//}
//
//
//void SVGRenderer::drawCubicBezier(Gdiplus::Graphics& g,
//    const CustomPoint& p0,
//    const CustomPoint& p1,
//    const CustomPoint& p2,
//    const CustomPoint& p3) const
//{
//    using namespace Gdiplus;
//
//    Pen pen(Color(255, 0, 0, 0), 2);
//
//    PointF points[4] = {
//        PointF(p0.x, p0.y),
//        PointF(p1.x, p1.y),
//        PointF(p2.x, p2.y),
//        PointF(p3.x, p3.y)
//    };
//
//    g.DrawBeziers(&pen, points, 4);
//}
//
//Gdiplus::WrapMode SVGRenderer::getGdiWrapMode(const std::string& spreadMethod) const {
//    if (spreadMethod == "reflect") {
//        return Gdiplus::WrapModeTileFlipXY;
//    }
//    if (spreadMethod == "repeat") {
//        return Gdiplus::WrapModeTile;
//    }
//    // Mặc định hoặc "pad"
//    return Gdiplus::WrapModeClamp;
//}
//
//void SVGRenderer::renderLinearGradientFill(Gdiplus::Graphics& g, const SVGLinearGradient* grad, const Gdiplus::RectF& bounds) {
//
//    // Sử dụng Getter để truy cập stopArray
//    const std::vector<SVGStop>& stops = grad->getStopArray();
//
//    if (!grad || stops.empty()) return;
//
//    // 1. Tính toán điểm bắt đầu và kết thúc dựa trên bounds
//    // Sử dụng Getters cho X1, Y1, X2, Y2
//    Gdiplus::PointF p1(bounds.X + grad->getX1() * bounds.Width,
//        bounds.Y + grad->getY1() * bounds.Height);
//
//    Gdiplus::PointF p2(bounds.X + grad->getX2() * bounds.Width,
//        bounds.Y + grad->getY2() * bounds.Height);
//
//    // 2. Lấy màu đầu và màu cuối (KHÔNG DÙNG ColorBlend)
//
//    // Lấy màu đầu
//    Gdiplus::Color c1 = stops.front().getStopColor();
//
//    // Lấy màu cuối
//    Gdiplus::Color c2 = (stops.size() > 1)
//        ? stops.back().getStopColor()
//        : c1;
//
//    // 3. Tạo LinearGradientBrush
//    Gdiplus::LinearGradientBrush brush(p1, p2, c1, c2);
//
//    // 4. Áp dụng Spread Method (Wrap Mode)
//    // Sử dụng Getter cho spreadMethod
//    brush.SetWrapMode(getGdiWrapMode(grad->getSpreadMethod()));
//
//    // 5. Fill hình dạng trong bounds
//    g.FillRectangle(&brush, bounds);
//}
//// Hàm hỗ trợ tạo Radial Gradient Brush
//void SVGRenderer::renderRadialGradientFill(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::RectF& bounds) {
//    using namespace Gdiplus;
//
//    if (!grad) return;
//
//    // Sử dụng Getter để truy cập stopArray
//    const std::vector<SVGStop>& stops = grad->getStopArray();
//
//    if (stops.empty()) return;
//
//    // 1. CHUẨN BỊ: Màu sắc và Vị trí
//    Color centerColor = stops.front().getStopColor(); // Center Color (Stop 1)
//
//    // Màu bao quanh (Surround Color) - Tối giản: Chỉ lấy Stop cuối cùng
//    Color surroundColor = (stops.size() > 1)
//        ? stops.back().getStopColor()
//        : centerColor;
//
//    if (centerColor.GetA() == 0 && surroundColor.GetA() == 0) return;
//
//    // 2. TẠO GRAPHICS PATH (Hình dạng cho Radial Gradient)
//    // Radial Gradient thường được áp dụng trên hình tròn/elip. Ta tạo Elip trong bounds.
//    GraphicsPath path;
//
//    // Gdiplus::RectF nhận top/left/width/height. Dùng nó để tạo Elip.
//    path.AddEllipse(bounds);
//
//    // 3. TẠO PATH GRADIENT BRUSH
//    PathGradientBrush brush(&path);
//
//    // 4. CẤU HÌNH BRUSH
//
//    // Center Point: Tính toán dựa trên cx, cy (giả định là giá trị tương đối [0, 1])
//    // Cần Getter cho cx và cy.
//    PointF centerPoint(bounds.X + grad->getCX() * bounds.Width,
//        bounds.Y + grad->getCY() * bounds.Height);
//
//    brush.SetCenterPoint(centerPoint);
//    brush.SetCenterColor(centerColor);
//
//    // Surround Colors (Màu bao quanh)
//    // Chỉ sử dụng 1 màu bao quanh (Stop cuối)
//    int count = 1;
//    // Gdiplus::PathGradientBrush yêu cầu con trỏ mảng màu và số lượng phần tử
//
//    // NOTE: Vì ta không dùng ColorBlend, ta phải cung cấp mảng Surround Colors,
//    // ngay cả khi chỉ có 1 màu.
//    Color surroundColors[1] = { surroundColor };
//    brush.SetSurroundColors(surroundColors, &count);
//
//    // 5. Áp dụng Spread Method (Wrap Mode)
//    brush.SetWrapMode(getGdiWrapMode(grad->getSpreadMethod()));
//
//    // 6. FILL
//    g.FillRectangle(&brush, bounds);
//    // CHÚ Ý: FillRectangle được dùng vì brush được định nghĩa trên bounds. 
//    // Nếu bạn muốn Fill chính xác hình dạng Element (VD: chỉ fill hình tròn),
//    // bạn cần dùng g.FillPath(&brush, &path); và phải biết hình dạng gốc.
//    // Ở đây ta dùng FillRectangle (hoặc FillEllipse) để tô đầy bounds.
//}
//// Hàm tổng quát renderGradientFill mới
//void SVGRenderer::renderGradientFill(Gdiplus::Graphics& g, const SVGGradient* grad, const Gdiplus::RectF& bounds, const SVGDocumentContext& context) {
//    if (!grad) return;
//
//    const SVGGradient* resolvedGrad = grad;
//    if (!resolvedGrad->resolveReference(context)) return;
//
//    if (auto* linear = dynamic_cast<const SVGLinearGradient*>(grad)) {
//        // Gọi Linear Gradient (Đã sửa)
//        renderLinearGradientFill(g, linear, bounds);
//    }
//    // Loại bỏ logic Solid Fill tạm thời và gọi hàm đầy đủ
//    else if (auto* radial = dynamic_cast<const SVGRadialGradient*>(grad)) {
//        // GỌI HÀM RADIAL ĐÃ TRIỂN KHAI ĐẦY ĐỦ
//        renderRadialGradientFill(g, radial, bounds);
//    }
//}
//
//


#include "CustomPoint.h"
#include "SVGRectangle.h"
#include "Stroke.h"        
#include "SVGCircle.h"   
#include "SVGLine.h"
#include "SVGPolygon.h"
#include "SVGPolyline.h"
#include "SVGText.h"
#include "SVGSquare.h"
#include "SVGStop.h"
//#include "SVGElement.h"  
//#include "CustomColor.h" // Cần cho getARGB
//#include "CustomPoint.h" // Cần cho CustomPoint.x/y
//#include "SVGDocumentContext.h" // Cần cho Context
//#include "SVGGradient.h"
#include "SVGLinearGradient.h" // Cần cho dynamic_cast
#include "SVGRadialGradient.h"// Cần cho dynamic_cast
//#include "SVGStop.h"
#include "SVGGroup.h"
//#include <gdiplus.h>
//#include <gdipluscolor.h>
//#include <gdiplusbrush.h>
//#include <gdiplusimaging.h>
//#include <gdiplusgraphics.h>
//#include <gdipluscolor.h>

#include "SVGRenderer.h"

//namespace Gdiplus {
//    struct PointF;
//    class Graphics;
//    class SolidBrush;
//    class Pen;
//    class Color;
//}
//using Gdiplus::PointF;
//using Gdiplus::SolidBrush;
//using Gdiplus::Pen;
//using Gdiplus::Color;
//using Gdiplus::LinearGradientBrush;
//using Gdiplus::PathGradientBrush;
//using Gdiplus::GraphicsPath;
//using Gdiplus::Matrix; // Cần cho việc áp dụng Transform
//using Gdiplus::RectF;
//using Gdiplus::PointF;
//using Gdiplus::Color;
//using Gdiplus::WrapModeTileFlipXY;
//using Gdiplus::WrapModeTile;
//using Gdiplus::WrapModeClamp;
//using Gdiplus::Status;
//using Gdiplus::InvalidParameter;
//using Gdiplus::Ok;
//using Gdiplus::NotImplemented;

SVGRenderer::SVGRenderer() : zoom(1.0f), rotate(0.0f) {}

// Chuyển đổi SpreadMethod sang WrapMode của GDI+
inline Gdiplus::WrapMode toGdiWrapMode(const std::string& spread) {
    if (spread == "reflect") return Gdiplus::WrapModeTileFlipXY; // Bí kíp làm mượt màu chéo
    if (spread == "repeat") return Gdiplus::WrapModeTile;
    return Gdiplus::WrapModeClamp; // Mặc định là "pad"
}

// Chuyển đổi tọa độ tỷ lệ (%) sang tọa độ tuyệt đối trên màn hình
inline Gdiplus::PointF mapPoint(float x, float y, const Gdiplus::RectF& b) {
    return Gdiplus::PointF(b.X + x * b.Width, b.Y + y * b.Height);
}



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
    // 0. ÁP DỤNG MATRIX LÊN CONTEXT
    // lưu matrix hiện tại của Graphics Context
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);
    // áp dụng ma trận biến đổi của đối tượng
    const Gdiplus::Matrix* matrix = rect->getTransformMatrix();
    if (matrix) {
        // nhân ma trận của rect và của context
        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    }
    // 1. TÍNH TOÁN BOUNDS (Giữ nguyên) : dùng toạ độ gốc chưa biến đổi để vẽ
    CustomPoint customP = rect->getTopLeftCorner();
    Gdiplus::PointF topLeft(static_cast<float>(customP.x), static_cast<float>(customP.y));
    float w = rect->getWidth();
    float h = rect->getHeight();
    Gdiplus::RectF bounds(topLeft.X, topLeft.Y, w, h);

    // 2. LOGIC FILL (Gradient HOẶC Màu đơn - Giữ nguyên logic Fill)
    if (rect->getSVGStyle().hasGradientFill()) {
        const SVGGradient* grad = rect->getSVGStyle().getGrad();
        if (grad) {
            Gdiplus::GraphicsPath tempPath;
            tempPath.AddRectangle(bounds);

            // Gọi hàm với tham số path mới
            renderGradientFill(g, grad, tempPath, bounds, context);
        }
    }
    else {
        // TRƯỜNG HỢP MÀU ĐƠN
        Gdiplus::Color fillColor = rect->getSVGStyle().getGdiFillColor();
        if (fillColor.GetA() > 0) {
            Gdiplus::SolidBrush brush(fillColor);
            g.FillRectangle(&brush, bounds);
        }
    }

    // 3. LOGIC STROKE (CHỈ MÀU ĐƠN)
    Stroke* strokeObj = rect->getSVGStyle().getStroke();
    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        // finalStrokeColor đã tính toán Opacity (từ getGdiColor())
        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();

        if (finalStrokeColor.GetA() > 0) {
            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
            pen.SetMiterLimit(strokeObj->getStrokeMiterLimit());
          
            g.DrawRectangle(&pen, bounds);
        }
    }

    // 4. KHÔI PHỤC MA TRẬN GRAPHICS CONTEXT
    g.SetTransform(&oldMatrix);
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
    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
    // lưu ma trận hiện tại
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);
    // áp dụng ma trận biến đội
    const Gdiplus::Matrix* matrix = circle->getTransformMatrix();
    if (matrix) {
        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    }
    // 2. DÙNG TOẠ ĐỘ GỐC ĐỂ VẼ
    Gdiplus::PointF center = circle->getCenter();
    float radius = circle->getRadius();
    float diameter = radius * 2;
    Gdiplus::RectF bounds(center.X - radius, center.Y - radius, diameter, diameter);

    // 1. LOGIC FILL
    if (circle->getSVGStyle().hasGradientFill()) {
        const SVGGradient* grad = circle->getSVGStyle().getGrad();
        if (grad) {
            Gdiplus::GraphicsPath tempPath;
            tempPath.AddEllipse(bounds);

            // Gọi hàm với tham số path mới
            renderGradientFill(g, grad, tempPath, bounds, context);
        }
    }
    else {
        // TRƯỜNG HỢP MÀU ĐƠN
        Gdiplus::Color fillColor = circle->getSVGStyle().getGdiFillColor();
        if (fillColor.GetA() > 0) {
            Gdiplus::SolidBrush brush(fillColor);
            g.FillEllipse(&brush, bounds);
        }
    }

    // 2. LOGIC STROKE
    Stroke* strokeObj = circle->getSVGStyle().getStroke();
    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();

        if (finalStrokeColor.GetA() > 0) {
            // Cần bounds để vẽ
            Gdiplus::PointF center = circle->getCenter();
            float radius = circle->getRadius();
            float diameter = radius * 2;
            Gdiplus::RectF bounds(center.X - radius, center.Y - radius, diameter, diameter);

            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
            pen.SetMiterLimit(strokeObj->getStrokeMiterLimit());

            g.DrawEllipse(&pen, bounds);
        }
    }
    g.SetTransform(&oldMatrix);
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
    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
    // lưu ma trận hiện tại
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);
    // áp dụng ma trận biến đội
    const Gdiplus::Matrix* matrix = ellipse->getTransformMatrix();
    if (matrix) {
        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    }
    Gdiplus::PointF center = ellipse->getCenter();
    float rx = ellipse->getRadiusX();
    float ry = ellipse->getRadiusY();
    Gdiplus::RectF bounds(center.X - rx, center.Y - ry, rx * 2, ry * 2);

    // 1. LOGIC FILL
    if (ellipse->getSVGStyle().hasGradientFill()) {
        const SVGGradient* grad = ellipse->getSVGStyle().getGrad();
        if (grad) {
            Gdiplus::GraphicsPath tempPath;
            tempPath.AddEllipse(bounds);

            // Gọi hàm với tham số path mới
            renderGradientFill(g, grad, tempPath, bounds, context);
        }
    }
    else {
        Gdiplus::Color fillColor = ellipse->getSVGStyle().getGdiFillColor();
        if (fillColor.GetA() > 0) {
            Gdiplus::SolidBrush brush(fillColor);
            g.FillEllipse(&brush, bounds);
        }
    }

    // 2. LOGIC STROKE
    Stroke* strokeObj = ellipse->getSVGStyle().getStroke();
    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();

        if (finalStrokeColor.GetA() > 0) {
            // Cần bounds để vẽ
            Gdiplus::PointF center = ellipse->getCenter();
            float rx = ellipse->getRadiusX();
            float ry = ellipse->getRadiusY();
            Gdiplus::RectF bounds(center.X - rx, center.Y - ry, rx * 2, ry * 2);

            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
            pen.SetMiterLimit(strokeObj->getStrokeMiterLimit());

            g.DrawEllipse(&pen, bounds);
        }
    }
    g.SetTransform(&oldMatrix);
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
    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
    // lưu ma trận hiện tại
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);
    // áp dụng ma trận biến đội
    const Gdiplus::Matrix* matrix = line->getTransformMatrix();
    if (matrix) {
        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    }


    Gdiplus::PointF start = line->getStartPoint();
    Gdiplus::PointF end = line->getEndPoint();

    // 1. Kiểm tra và Vẽ Stroke (Chỉ khi Stroke tồn tại)
    Stroke* strokeObj = line->getSVGStyle().getStroke();

    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
        float strokeW = strokeObj->strokeWidth;

        if (finalStrokeColor.GetA() > 0) {
            // FIX: Gradient Stroke Logic (Nếu cần)
            // Hiện tại Line không thể fill, nên ta chỉ cần xử lý Stroke

            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);

            pen.SetMiterLimit(strokeObj->getStrokeMiterLimit());

            g.DrawLine(&pen, start, end);
        }
    }
    g.SetTransform(&oldMatrix);
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
    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
    // lưu ma trận hiện tại
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);
    // áp dụng ma trận biến đội
    const Gdiplus::Matrix* matrix = polygon->getTransformMatrix();
    if (matrix) {
        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    }

    const std::vector<CustomPoint>& customPoints = polygon->getPoints();
    std::vector<Gdiplus::PointF> gdiPoints;
    gdiPoints.reserve(customPoints.size());
    for (const auto& cp : customPoints) {
        gdiPoints.emplace_back(cp.x, cp.y);
    }
    if (gdiPoints.empty()) return;

    Gdiplus::GraphicsPath path;
    path.AddLines(gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
    path.CloseFigure();
    FillRule svgFillRule = polygon->getSVGStyle().getFillRule();

    // Chuyển FillRule của SVG sang FillMode của GDI+
    if (svgFillRule == FillRule::EvenOdd) {
        path.SetFillMode(Gdiplus::FillModeAlternate);
    }
    else {
        // Mặc định là NonZero (Winding)
        path.SetFillMode(Gdiplus::FillModeWinding);
    }

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
        Gdiplus::Color fillColor = polygon->getSVGStyle().getGdiFillColor();
        if (fillColor.GetA() > 0) {
            Gdiplus::SolidBrush brush(fillColor);
            g.FillPath(&brush, &path);
        }
    }

    // 3. LOGIC STROKE (CHỈ MÀU ĐƠN)
    Stroke* strokeObj = polygon->getSVGStyle().getStroke();
    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
        if (finalStrokeColor.GetA() > 0) {
            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);

            pen.SetMiterLimit(strokeObj->getStrokeMiterLimit());

            g.DrawPath(&pen, &path); // Vẽ Path
        }
    }
    g.SetTransform(&oldMatrix);
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
//void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline, const SVGDocumentContext& context) {
//    if (!polyline) return;
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    // lưu ma trận hiện tại
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    // áp dụng ma trận biến đội
//    const Gdiplus::Matrix* matrix = polyline->getTransformMatrix();
//    if (matrix) {
//        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    }
//
//    // 1. Chuyển đổi điểm
//    std::vector<Gdiplus::PointF> gdiPoints;
//    const auto& customPoints = polyline->getPoints();
//    for (const auto& cp : customPoints) {
//        gdiPoints.emplace_back(cp.x, cp.y);
//    }
//    if (gdiPoints.size() < 2) return;
//
//    // 1.1. TẠO GRAPHICS PATH VÀ TÍNH BOUNDS (Bắt buộc cho Fill và Gradient)
//    Gdiplus::GraphicsPath path;
//    path.AddLines(gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//    path.CloseFigure(); // Xử lý như Polygon đóng cho Fill
//
//    Gdiplus::RectF bounds;
//    path.GetBounds(&bounds); // Lấy Bounding Box chính xác
//
//    // 2. LOGIC FILL (Gradient HOẶC Màu đơn)
//    if (polyline->getSVGStyle().hasGradientFill()) {
//        const SVGGradient* grad = polyline->getSVGStyle().getGrad();
//        if (grad) {
//            // GỌI HÀM HỖ TRỢ FILL PATH BẰNG GRADIENT
//            renderGradientPathFill(g, grad, path, bounds, context);
//        }
//    }
//    else {
//        // TRƯỜNG HỢP MÀU ĐƠN (Solid Fill)
//        Gdiplus::Color fillColor = polyline->getSVGStyle().getGdiFillColor();
//
//        if (fillColor.GetA() > 0 && polyline->getSVGStyle().getFillOpacity() > 0) {
//            Gdiplus::SolidBrush brush(fillColor);
//            // Dùng FillPath sau khi Path đã được đóng
//            g.FillPath(&brush, &path);
//        }
//    }
//
//    // 3. Kiểm tra và Vẽ Stroke (Chỉ cần vẽ đường, không cần đóng Path)
//    Stroke* strokeObj = polyline->getSVGStyle().getStroke();
//    if (strokeObj != nullptr) {
//        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth;
//
//        if (finalStrokeColor.GetA() > 0) {
//            // Tạo Pen và vẽ đường gấp khúc (DrawLines)
//            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
//            pen.SetMiterLimit(strokeObj->getStrokeMiterLimit());
//            // Dùng DrawLines thay vì DrawPath để vẽ Polyline MỞ (chuẩn SVG)
//            g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
//        }
//    }
//
//    g.SetTransform(&oldMatrix);
//}
void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline, const SVGDocumentContext& context) {
    if (!polyline) return;

    // 1. ÁP DỤNG MA TRẬN
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);
    const Gdiplus::Matrix* matrix = polyline->getTransformMatrix();
    if (matrix) {
        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    }

    // 1. Chuyển đổi điểm
    std::vector<Gdiplus::PointF> gdiPoints;
    const auto& customPoints = polyline->getPoints();
    for (const auto& cp : customPoints) {
        gdiPoints.emplace_back(cp.x, cp.y);
    }
    if (gdiPoints.size() < 2) {
        g.SetTransform(&oldMatrix);
        return;
    }

    // Kiểm tra xem có cần Fill (Gradient hoặc Solid) hay không
    bool shouldFill = (polyline->getSVGStyle().getGdiFillColor().GetA() > 0 || polyline->getSVGStyle().hasGradientFill());

    Gdiplus::GraphicsPath path;

    // 2. LOGIC FILL (Chỉ thực hiện nếu có thuộc tính Fill)
    if (shouldFill) {
        // TẠO PATH (ĐÓNG HÌNH cho mục đích Fill, theo chuẩn SVG)
        path.AddLines(gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
        path.CloseFigure(); // <<< ĐÓNG HÌNH CHỈ KHI CÓ FILL

        Gdiplus::RectF bounds;
        path.GetBounds(&bounds);

        // 2.1. Gradient Fill
        if (polyline->getSVGStyle().hasGradientFill()) {
            const SVGGradient* grad = polyline->getSVGStyle().getGrad();
            if (grad) {
                renderGradientPathFill(g, grad, path, bounds, context);
            }
        }
        // 2.2. Solid Fill
        else {
            Gdiplus::Color fillColor = polyline->getSVGStyle().getGdiFillColor();
            if (fillColor.GetA() > 0) {
                Gdiplus::SolidBrush brush(fillColor);
                g.FillPath(&brush, &path);
            }
        }
    }

    // 3. LOGIC STROKE (Luôn vẽ đường mở)
    Stroke* strokeObj = polyline->getSVGStyle().getStroke();
    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();

        if (finalStrokeColor.GetA() > 0) {
            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);

            // ÁP DỤNG MITER LIMIT
            pen.SetMiterLimit(strokeObj->getStrokeMiterLimit());

            // Sử dụng DRAWLINES cho đường mở (CHÍNH XÁC CHO POLYLINE)
            g.DrawLines(&pen, gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
        }
    }

    // 4. KHÔI PHỤC MA TRẬN
    g.SetTransform(&oldMatrix);
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

//void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text, const SVGDocumentContext& context) {
//    if (!text) return;
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    // lưu ma trận hiện tại
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    // áp dụng ma trận biến đội
//    const Gdiplus::Matrix* matrix = text->getTransformMatrix();
//    if (matrix) {
//        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    }
//
//    // Lấy thông tin cơ bản
//    Gdiplus::PointF pos = text->getStart();
//    std::wstring content = text->getContent();
//
//    // --- LOGIC STROKE CHO TEXT (Nếu cần) ---
//    // GDI+ không hỗ trợ Stroke Text bằng Pen dễ dàng như SVG.
//    // Nếu bạn muốn Stroke Text, bạn phải dùng Path::Widen và tô màu Path đó.
//    // Tạm thời bỏ qua logic Stroke phức tạp này để giữ Text hoạt động.
//
//    Gdiplus::Color fillColor = text->getSVGStyle().getGdiFillColor();
//    Stroke* strokeObj = text->getSVGStyle().getStroke();
//
//    float fontSize = text->getFontSize();
//    if (fontSize <= 0.0f) fontSize = 12.0f;
//    // fontStyle
//    //int fontStyle = text->getSVGStyle().getFontStyle();
//    // Gdiplus Font setup
//    Gdiplus::FontFamily fontFamily(L"Times New Roman"); // Hoặc lấy từ Style nếu có
//    Gdiplus::Font font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
//
//    // Vị trí baseline (Điều chỉnh Y)
//    Gdiplus::PointF new_pos(pos.X, pos.Y - text->getFontSize() * 0.8f);
//
//    // --- LOGIC FILL: GRADIENT HAY SOLID ---
//
//    if (text->getSVGStyle().hasGradientFill()) {
//
//        // 1. TẠO GRAPHICS PATH TỪ TEXT (BẮT BUỘC cho Gradient)
//        Gdiplus::GraphicsPath path;
//
//        // Cờ định dạng để đảm bảo căn chỉnh đúng (FormatFlagsNoWrap là mặc định tốt)
//        Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());
//
//        // Thêm chuỗi văn bản vào Path
//        Gdiplus::Status status = path.AddString(
//            content.c_str(),
//            static_cast<INT>(content.length()),
//            &fontFamily,
//            Gdiplus::FontStyleRegular,
//            fontSize,
//            new_pos, // Điểm bắt đầu
//            &format
//        );
//
//        if (status == Gdiplus::Ok) {
//            Gdiplus::RectF bounds;
//            path.GetBounds(&bounds); // Lấy Bounding Box của Text Path
//
//            const SVGGradient* grad = text->getSVGStyle().getGrad();
//            if (grad) {
//                // Sử dụng hàm đã có để tô Path bằng Gradient
//                renderGradientPathFill(g, grad, path, bounds, context);
//            }
//        }
//    }
//    else {
//        // TRƯỜNG HỢP MÀU ĐƠN (Solid Fill)
//        Gdiplus::Color color = text->getSVGStyle().getGdiFillColor();
//
//        if (color.GetA() > 0) {
//            Gdiplus::SolidBrush brush(color);
//
//            // Sử dụng DrawString truyền thống cho Solid Fill
//            g.DrawString(content.c_str(), -1, &font, new_pos, &brush);
//        }
//    }
//
//    g.SetTransform(&oldMatrix);
//}
//void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text, const SVGDocumentContext& context) {
//    if (!text) return;
//
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT (Giữ nguyên)
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    const Gdiplus::Matrix* matrix = text->getTransformMatrix();
//    if (matrix) {
//        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    }
//
//    // Lấy Style và Font
//    Gdiplus::PointF pos = text->getStart();
//    std::wstring content = text->getContent();
//    Stroke* strokeObj = text->getSVGStyle().getStroke(); // Dữ liệu Stroke
//
//    //float fontSize = text->getFontSize();
//    //if (fontSize <= 0.0f) fontSize = 12.0f;
//    //Gdiplus::FontFamily fontFamily(L"Times New Roman");
//    //Gdiplus::Font font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
//    //Gdiplus::PointF new_pos(pos.X, pos.Y - text->getFontSize() * 0.8f);
//    // Lấy thuộc tính font
//    std::string fontFamilyName = text->getFontFamily();
//    float fontSize = text->getFontSize();
//    if (fontSize <= 0.0f) fontSize = 12.0f;
//
//    // 1. CHUYỂN ĐỔI STYLE VÀ WEIGHT SANG Gdiplus::FontStyle
//    int gdiFontStyle = Gdiplus::FontStyleRegular;
//    //Gdiplus::PointF pos = text->getStart();
//    Gdiplus::PointF new_pos(pos.X, pos.Y - text->getFontSize() * 0.8f); // Vị trí Baseline
//    // Xử lý Font Style
//    if (text->getFontStyle() == FontStyles::Italic) {
//        gdiFontStyle |= Gdiplus::FontStyleItalic;
//    }
//
//    // Xử lý Font Weight
//    if (text->getFontWeight() == FontWeight::Bold) {
//        gdiFontStyle |= Gdiplus::FontStyleBold;
//    }
//
//    // 2. TẠO FONT
//    // Cần chuyển std::string sang std::wstring cho Gdiplus::FontFamily
//    std::wstring wFontFamilyName(fontFamilyName.begin(), fontFamilyName.end());
//
//    Gdiplus::FontFamily fontFamily(wFontFamilyName.c_str());
//    Gdiplus::Font font(&fontFamily, fontSize, gdiFontStyle, Gdiplus::UnitPixel);
//    // --- TẠO GRAPHICS PATH TỪ TEXT (Chung cho Stroke và Gradient Fill) ---
//    Gdiplus::GraphicsPath path;
//    Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());
//
//    Gdiplus::Status status = path.AddString(
//        content.c_str(),
//        static_cast<INT>(content.length()),
//        &fontFamily,
//        Gdiplus::FontStyleRegular,
//        fontSize,
//        new_pos, // Điểm bắt đầu
//        &format
//    );
//
//    if (status != Gdiplus::Ok) {
//        g.SetTransform(&oldMatrix);
//        return;
//    }
//
//    Gdiplus::RectF bounds;
//    path.GetBounds(&bounds); // Lấy Bounding Box của Text Path
//    // --- ÁP DỤNG TEXT-ANCHOR ---
//
//// Lấy Bounding Box của Text (Cần thiết để tính toán căn chỉnh)
//    Gdiplus::RectF layoutRect; // Kích thước của văn bản
//    //Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());
//    // Để đo kích thước văn bản, ta cần dùng đối tượng Graphics để lấy context
//    g.MeasureString(text->getContent().c_str(),
//        static_cast<INT>(text->getContent().length()),
//        &font,
//        new_pos, // Điểm bắt đầu giả định
//        &format,
//        &layoutRect);
//
//    // Điều chỉnh new_pos.X dựa trên TextAnchor
//    if (text->getTextAnchor() == TextAnchor::Middle) {
//        // Neo ở giữa: Dịch chuyển X ngược lại nửa chiều rộng
//        new_pos.X -= layoutRect.Width / 2.0f;
//    }
//    else if (text->getTextAnchor() == TextAnchor::End) {
//        // Neo ở cuối: Dịch chuyển X ngược lại toàn bộ chiều rộng
//        new_pos.X -= layoutRect.Width;
//    }
//    // --- 2. VẼ STROKE (VIỀN CHỮ) ---
//    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
//        //Gdiplus::Color strokeColor = strokeObj->getGdiStrokeColor();
//        Gdiplus::Color strokeColor = strokeObj->getGdiColor();
//
//        // Kiểm tra độ mờ (Opacity)
//        if (strokeColor.GetA() > 0) {
//            Gdiplus::Pen pen(strokeColor, strokeObj->strokeWidth);
//
//            // Thiết lập các thuộc tính Stroke (Miter Limit, LineJoin, v.v. nếu cần)
//            // pen.SetLineJoin(Gdiplus::LineJoinRound); 
//
//            // VẼ VIỀN BẰNG Gdiplus::DrawPath
//            g.DrawPath(&pen, &path);
//        }
//    }
//
//    // --- 3. VẼ FILL (NỀN CHỮ) ---
//    if (text->getSVGStyle().hasGradientFill()) {
//        // TRƯỜNG HỢP GRADIENT FILL
//        const SVGGradient* grad = text->getSVGStyle().getGrad();
//        if (grad) {
//            // Sử dụng hàm đã có để tô Path bằng Gradient (FillPath sẽ được gọi bên trong)
//            renderGradientPathFill(g, grad, path, bounds, context);
//        }
//    }
//    else {
//        // TRƯỜNG HỢP MÀU ĐƠN (Solid Fill)
//        Gdiplus::Color fillColor = text->getSVGStyle().getGdiFillColor();
//
//        if (fillColor.GetA() > 0) {
//            Gdiplus::SolidBrush brush(fillColor);
//
//            // SỬ DỤNG Gdiplus::FillPath (thay vì DrawString) để tô nền Path
//            g.FillPath(&brush, &path);
//        }
//    }
//
//    // 4. KHÔI PHỤC MA TRẬN (Giữ nguyên)
//    g.SetTransform(&oldMatrix);
//}
//void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text, const SVGDocumentContext& context) {
//    if (!text) return;
//
//    // 1. ÁP DỤNG MA TRẬN
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    const Gdiplus::Matrix* matrix = text->getTransformMatrix();
//    if (matrix) {
//        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    }
//
//    // Lấy thông tin cơ bản
//    Gdiplus::PointF pos = text->getStart();
//    std::wstring content = text->getContent();
//
//    float offsetX = text->getDx(); 
//    float offsetY = text->getDy();
//
//    Gdiplus::Color fillColor = text->getSVGStyle().getGdiFillColor();
//    Stroke* strokeObj = text->getSVGStyle().getStroke();
//
//    // --- BƯỚC 1: XỬ LÝ FONT STYLE & AN TOÀN FONT FAMILY ---
//    std::string fontFamilyName = text->getFontFamily();
//    float fontSize = text->getFontSize();
//    if (fontSize <= 0.0f) fontSize = 12.0f;
//
//    // Chuyển đổi Style/Weight sang Gdiplus::FontStyle
//    int gdiFontStyle = Gdiplus::FontStyleRegular;
//    if (text->getFontStyle() == FontStyles::Italic) {
//        gdiFontStyle |= Gdiplus::FontStyleItalic;
//    }
//    if (text->getFontWeight() == FontWeight::Bold) {
//        gdiFontStyle |= Gdiplus::FontStyleBold;
//    }
//
//    // Cơ chế Fallback an toàn cho Font Family (Dùng Times New Roman)
//    std::wstring wFontFamilyName;
//    std::wstring fallbackFontName = L"Arial";
//    if (fontFamilyName.empty() || fontFamilyName == "serif") {
//        // Fallback về Times New Roman nếu không có tên font hoặc là serif generic
//        wFontFamilyName = fallbackFontName;
//    }
//    else if (fontFamilyName == "sans-serif") {
//        // Xử lý các font generic khác nếu cần
//        wFontFamilyName = L"Arial";
//    }
//    else {
//        // Chuyển đổi tên font từ SVG sang wstring
//        wFontFamilyName.assign(fontFamilyName.begin(), fontFamilyName.end());
//    }
//
//    // TẠO FONT
//    Gdiplus::FontFamily fontFamily(wFontFamilyName.c_str());
//    Gdiplus::Font* fontPtr; // Sử dụng con trỏ để quản lý bộ nhớ linh hoạt hơn
//
//    if (fontFamily.GetLastStatus() != Gdiplus::Ok) {
//        // FALLBACK LẦN 2: Nếu font yêu cầu không hợp lệ, dùng Times New Roman
//        Gdiplus::FontFamily fallbackFamily(fallbackFontName.c_str());
//        fontPtr = new Gdiplus::Font(&fallbackFamily, fontSize, gdiFontStyle, Gdiplus::UnitPixel);
//        // Gán lại fontFamily cho mục đích AddString (cần dùng font family hợp lệ)
//        fontFamily = fallbackFamily;
//    }
//    else {
//        // Tên font hợp lệ và tồn tại trên hệ thống
//        fontPtr = new Gdiplus::Font(&fontFamily, fontSize, gdiFontStyle, Gdiplus::UnitPixel);
//    }
//
//    // KHAI BÁO FONT ĐỂ SỬ DỤNG
//    Gdiplus::Font& font = *fontPtr;
//
//    // Vị trí baseline (Điều chỉnh Y)
//    Gdiplus::PointF currentPos(pos.X + offsetX, 
//                               pos.Y - text->getFontSize() * 0.8f + offsetY);
//    Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());
//
//
//    // --- BƯỚC 2: CĂN CHỈNH TEXT-ANCHOR ---
//    Gdiplus::RectF layoutRect;
//
//    // Đo đạc kích thước văn bản
//    g.MeasureString(content.c_str(),
//        static_cast<INT>(content.length()),
//        &font,
//        currentPos,
//        &format,
//        &layoutRect);
//
//    // Điều chỉnh tọa độ X (currentPos.X) 
//    if (text->getTextAnchor() == TextAnchor::Middle) {
//        currentPos.X -= layoutRect.Width / 2.0f;
//    }
//    else if (text->getTextAnchor() == TextAnchor::End) {
//        currentPos.X -= layoutRect.Width;
//    }
//
//
//    // --- BƯỚC 3: LOGIC FILL VÀ STROKE (Tách biệt Path và DrawString) ---
//
//    // Nếu có Gradient Fill HOẶC Stroke, chúng ta cần dùng GraphicsPath
//    if (text->getSVGStyle().hasGradientFill() || (strokeObj && strokeObj->strokeWidth > 0.0f)) {
//
//        // Tạo GraphicsPath
//        Gdiplus::GraphicsPath path;
//        Gdiplus::Status status = path.AddString(
//            content.c_str(),
//            static_cast<INT>(content.length()),
//            &fontFamily,
//            gdiFontStyle,
//            fontSize,
//            currentPos, // VỊ TRÍ ĐÃ CĂN CHỈNH
//            &format
//        );
//
//        if (status != Gdiplus::Ok) {
//            // Xử lý lỗi
//            g.SetTransform(&oldMatrix);
//            return;
//        }
//
//        // 3.1: GRADIENT FILL
//        if (text->getSVGStyle().hasGradientFill()) {
//            Gdiplus::RectF bounds;
//            path.GetBounds(&bounds);
//            const SVGGradient* grad = text->getSVGStyle().getGrad();
//            if (grad) {
//                renderGradientPathFill(g, grad, path, bounds, context);
//            }
//        }
//        // 3.2: SOLID FILL (Khi có Stroke, nhưng không có Gradient)
//        else {
//            if (fillColor.GetA() > 0) {
//                Gdiplus::SolidBrush brush(fillColor);
//                g.FillPath(&brush, &path);
//            }
//        }
//
//        // 3.3: STROKE (Viền chữ)
//        if (strokeObj && strokeObj->strokeWidth > 0.0f) {
//            Gdiplus::Color strokeColor = strokeObj->getGdiColor();
//            if (strokeColor.GetA() > 0) {
//                Gdiplus::Pen pen(strokeColor, strokeObj->strokeWidth);
//                g.DrawPath(&pen, &path);
//            }
//        }
//    }
//    else {
//        // TRƯỜNG HỢP DUY NHẤT: CHỈ CÓ SOLID FILL (Không Stroke, không Gradient)
//        Gdiplus::Color color = text->getSVGStyle().getGdiFillColor();
//
//        if (color.GetA() > 0) {
//            Gdiplus::SolidBrush brush(color);
//            // SỬ DỤNG DrawString với vị trí đã căn chỉnh
//            g.DrawString(content.c_str(), -1, &font, currentPos, &brush);
//        }
//    }
//
//    // 4. KHÔI PHỤC MA TRẬN
//    g.SetTransform(&oldMatrix);
//    delete fontPtr;
//}
void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text, const SVGDocumentContext& context) {
    if (!text) return;

    // 1. ÁP DỤNG MA TRẬN
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);
    const Gdiplus::Matrix* matrix = text->getTransformMatrix();
    if (matrix) {
        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    }

    // Lấy thông tin cơ bản và Offset
    Gdiplus::PointF pos = text->getStart();
    std::wstring content = text->getContent();
    float offsetX = text->getDx();
    float offsetY = text->getDy();

    Gdiplus::Color fillColor = text->getSVGStyle().getGdiFillColor();
    Stroke* strokeObj = text->getSVGStyle().getStroke();

    // --- BƯỚC 1: XỬ LÝ FONT STYLE & AN TOÀN FONT FAMILY ---
    std::string fontFamilyName = text->getFontFamily();
    float fontSize = text->getFontSize();
    if (fontSize <= 0.0f) fontSize = 12.0f;

    // Chuyển đổi Style/Weight sang Gdiplus::FontStyle
    int gdiFontStyle = Gdiplus::FontStyleRegular;
    if (text->getFontStyle() == FontStyles::Italic) {
        gdiFontStyle |= Gdiplus::FontStyleItalic;
    }
    if (text->getFontWeight() == FontWeight::Bold) {
        gdiFontStyle |= Gdiplus::FontStyleBold;
    }

    // Cơ chế Fallback an toàn cho Font Family
    std::wstring wFontFamilyName;
    std::wstring fallbackFontName = L"Times New Roman";

    if (fontFamilyName.empty() || fontFamilyName == "serif") {
        wFontFamilyName = fallbackFontName;
    }
    else if (fontFamilyName == "sans-serif") {
        wFontFamilyName = L"Times New Roman";
    }
    else {
        wFontFamilyName.assign(fontFamilyName.begin(), fontFamilyName.end());
    }

    // --- XỬ LÝ FONT FAMILY KHÔNG TRUY CẬP ĐƯỢC TOÁN TỬ GÁN (operator=) ---
    Gdiplus::FontFamily actualRequestedFamily(wFontFamilyName.c_str());
    Gdiplus::FontFamily fallbackFamily(fallbackFontName.c_str());

    // Con trỏ tới Font Family sẽ được sử dụng để vẽ và tạo Path
    const Gdiplus::FontFamily* drawingFamily;

    if (actualRequestedFamily.GetLastStatus() != Gdiplus::Ok) {
        // Fallback: Sử dụng đối tượng fallbackFamily đã tạo trên stack
        drawingFamily = &fallbackFamily;
    }
    else {
        // Thành công: Sử dụng đối tượng actualRequestedFamily
        drawingFamily = &actualRequestedFamily;
    }

    // TẠO FONT (Sử dụng con trỏ Family HỢP LỆ)
    Gdiplus::Font font(drawingFamily, fontSize, gdiFontStyle, Gdiplus::UnitPixel);

    // Vị trí baseline (Điều chỉnh Y và áp dụng Offset X/Y)
    Gdiplus::PointF currentPos(
        pos.X + offsetX,
        pos.Y - text->getFontSize() * 0.8f + offsetY
    );
    Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());


    // --- BƯỚC 2: CĂN CHỈNH TEXT-ANCHOR --- 
    Gdiplus::RectF layoutRect;

    // Đo đạc kích thước văn bản (Cần thiết cho Text Anchor)
    g.MeasureString(content.c_str(),
        static_cast<INT>(content.length()),
        &font,
        currentPos,
        &format,
        &layoutRect);

    // Điều chỉnh tọa độ X (currentPos.X)
    if (text->getTextAnchor() == TextAnchor::Middle) {
        currentPos.X -= layoutRect.Width / 2.0f;
    }
    else if (text->getTextAnchor() == TextAnchor::End) {
        currentPos.X -= layoutRect.Width;
    }


    // --- BƯỚC 3: LOGIC FILL VÀ STROKE (Tách biệt Path và DrawString) ---

    // Nếu có Gradient Fill HOẶC Stroke, chúng ta cần dùng GraphicsPath
    if (text->getSVGStyle().hasGradientFill() || (strokeObj && strokeObj->strokeWidth > 0.0f)) {

        // Tạo GraphicsPath
        Gdiplus::GraphicsPath path;
        Gdiplus::Status status = path.AddString(
            content.c_str(),
            static_cast<INT>(content.length()),
            drawingFamily, // SỬ DỤNG CON TRỎ FAMILY HỢP LỆ
            gdiFontStyle,
            fontSize,
            currentPos, // VỊ TRÍ ĐÃ CĂN CHỈNH
            &format
        );

        if (status != Gdiplus::Ok) {
            g.SetTransform(&oldMatrix);
            return;
        }

        // 3.1: GRADIENT FILL
        if (text->getSVGStyle().hasGradientFill()) {
            Gdiplus::RectF bounds;
            path.GetBounds(&bounds);
            const SVGGradient* grad = text->getSVGStyle().getGrad();
            if (grad) {
                // Giả định hàm renderGradientPathFill tồn tại
                // renderGradientPathFill(g, grad, path, bounds, context); 
            }
        }
        // 3.2: SOLID FILL (Khi có Stroke, nhưng không có Gradient)
        else {
            if (fillColor.GetA() > 0) {
                Gdiplus::SolidBrush brush(fillColor);
                g.FillPath(&brush, &path);
            }
        }

        // 3.3: STROKE (Viền chữ)
        if (strokeObj && strokeObj->strokeWidth > 0.0f) {
            Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
            if (finalStrokeColor.GetA() > 0) {
                Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
                pen.SetMiterLimit(strokeObj->getStrokeMiterLimit());

                g.DrawPath(&pen, &path);
            }
        }
    }
    else {
        // TRƯỜNG HỢP DUY NHẤT: CHỈ CÓ SOLID FILL (Không Stroke, không Gradient)
        Gdiplus::Color color = text->getSVGStyle().getGdiFillColor();

        if (color.GetA() > 0) {
            Gdiplus::SolidBrush brush(color);
            // SỬ DỤNG DrawString với vị trí đã căn chỉnh
            g.DrawString(content.c_str(), -1, &font, currentPos, &brush);
        }
    }

    // 4. KHÔI PHỤC MA TRẬN
    g.SetTransform(&oldMatrix);
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
    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
    // lưu ma trận hiện tại
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);
    // áp dụng ma trận biến đội
    const Gdiplus::Matrix* matrix = square->getTransformMatrix();
    if (matrix) {
        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    }

    // Sử dụng renderRectangle vì Square là Rect đặc biệt
    SVGRectangle rect; // Tạo rect tạm thời
    rect.setTopLeftCorner(square->getTopLeftCorner());
    rect.setWidth(square->getHeight());
    rect.setHeight(square->getHeight());
    rect.setSVGStyle(square->getSVGStyle()); // Kế thừa Style

    renderRectangle(g, &rect, context);

    g.SetTransform(&oldMatrix);
}
void SVGRenderer::renderFigure(Gdiplus::Graphics& g, const SVGGroup* rootGroup, const SVGDocumentContext& context) {
    if (!rootGroup) return;
    // FIX: Truyền Context xuống hàm Group
    renderGroup(g, rootGroup, context);
}
//void SVGRenderer::renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup, const SVGDocumentContext& context) {
//    if (!rootGroup) return;
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    // lưu ma trận hiện tại
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    // áp dụng ma trận biến đội
//    const Gdiplus::Matrix* groupMatrix = rootGroup->getTransformMatrix();
//    if (groupMatrix) {
//        // dùng tempMatrix để sửa đổi (nếu cần) mà không thay đổi SVGGroup
//        Gdiplus::Matrix* tempMatrix = groupMatrix->Clone();
//        g.MultiplyTransform(tempMatrix, Gdiplus::MatrixOrderPrepend);
//        delete tempMatrix;
//    }
//    const auto& children = rootGroup->getSVGElementArray();
//    for (SVGElement* element : children) {
//        // FIX: Double Dispatch: Truyền Context xuống các element con
//        element->render(*this, g, context);
//    }
//    g.SetTransform(&oldMatrix);
//}
void SVGRenderer::renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup, const SVGDocumentContext& context) {
    if (!rootGroup) return;

    // 1. LƯU MA TRẬN HIỆN TẠI (Tích lũy từ các Group cha)
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);

    // 2. ÁP DỤNG MA TRẬN RIÊNG CỦA NHÓM NÀY
    const Gdiplus::Matrix* groupMatrix = rootGroup->getTransformMatrix();

    if (groupMatrix) {
        // Gdiplus::Graphics::MultiplyTransform() chỉ đọc ma trận được truyền vào.
        // Không cần gọi Clone() hay delete.
        g.MultiplyTransform(groupMatrix, Gdiplus::MatrixOrderPrepend);
    }

    // 3. RENDER CÁC PHẦN TỬ CON
    const auto& children = rootGroup->getSVGElementArray();
    for (SVGElement* element : children) {
        // Double Dispatch: Gọi hàm render cụ thể của phần tử con (rect, circle, text,...)
        element->render(*this, g, context);
    }

    // 4. KHÔI PHỤC MA TRẬN
    // Đảm bảo Graphics Context trở về trạng thái ban đầu cho các phần tử sau này (Group anh em).
    g.SetTransform(&oldMatrix);
}
//void SVGRenderer::renderGradientPathFill(Gdiplus::Graphics& g, const SVGGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds, const SVGDocumentContext& context) const {
//    using namespace Gdiplus;
//    if (!grad) return;
//
//    const SVGGradient* resolvedGrad = grad;
//    if (!resolvedGrad->resolveReference(context)) return;
//
//    if (auto* linear = dynamic_cast<const SVGLinearGradient*>(grad)) {
//        const std::vector<SVGStop>& stops = linear->getStopArray();
//        if (stops.empty()) return;
//
//        PointF p1(bounds.X + linear->getX1() * bounds.Width, bounds.Y + linear->getY1() * bounds.Height);
//        PointF p2(bounds.X + linear->getX2() * bounds.Width, bounds.Y + linear->getY2() * bounds.Height);
//
//        Color c1 = (Color)stops.front().getStopColor();
//        Color c2 = (stops.size() > 1) ? (Color)stops.back().getStopColor() : c1;
//
//        LinearGradientBrush brush(p1, p2, c1, c2);
//        brush.SetWrapMode(getGdiWrapMode(linear->getSpreadMethod()));
//        g.FillPath(&brush, &path); // *** Fill Path ***
//    }
//    else if (auto* radial = dynamic_cast<const SVGRadialGradient*>(grad)) {
//        // Tối giản Radial Fill trên Path: Dùng Solid Fill bằng màu Center
//        const std::vector<SVGStop>& stops = radial->getStopArray();
//        if (stops.empty()) return;
//
//        Color centerColor = (Color)stops.front().getStopColor();
//        if (centerColor.GetA() > 0) {
//            SolidBrush brush(centerColor);
//            g.FillPath(&brush, &path); // *** Fill Path ***
//        }
//    }
//}
//void SVGRenderer::renderGradientPathFill(Gdiplus::Graphics& g, const SVGGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds, const SVGDocumentContext& context) const {
//    using namespace Gdiplus;
//    if (!grad) return;
//
//    // 1. Giải quyết tham chiếu (href)
//    const SVGGradient* resolvedGrad = grad->resolveReference(context);
//    if (!resolvedGrad) return; // Bảo vệ chống crash nếu không tìm thấy ID gradient
//
//    // --- XỬ LÝ LINEAR GRADIENT ---
//    if (auto* linear = dynamic_cast<const SVGLinearGradient*>(resolvedGrad)) {
//        std::vector<SVGStop> stops = linear->getStopArray();
//        if (stops.empty()) return;
//
//        // Chuẩn hóa Stops
//        if (stops.front().getOffset() > 0.0f)
//            stops.insert(stops.begin(), SVGStop(stops.front().getStopColor(), 0.0f, stops.front().getStopOpacity()));
//        if (stops.back().getOffset() < 1.0f)
//            stops.push_back(SVGStop(stops.back().getStopColor(), 1.0f, stops.back().getStopOpacity()));
//
//        // Tính toán tọa độ p1, p2
//        PointF p1, p2;
//        if (linear->getGradientUnits() == "userSpaceOnUse") {
//            p1 = PointF(linear->getX1(), linear->getY1());
//            p2 = PointF(linear->getX2(), linear->getY2());
//        }
//        else {
//            p1 = PointF(bounds.X + linear->getX1() * bounds.Width, bounds.Y + linear->getY1() * bounds.Height);
//            p2 = PointF(bounds.X + linear->getX2() * bounds.Width, bounds.Y + linear->getY2() * bounds.Height);
//        }
//
//        int n = (int)stops.size();
//        std::vector<REAL> pos(n);
//        std::vector<Color> col(n);
//        for (int i = 0; i < n; ++i) {
//            pos[i] = stops[i].getOffset();
//            col[i] = Color(BYTE(stops[i].getStopOpacity() * 255), stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b);
//        }
//
//        // Đảm bảo mảng pos tăng dần để không crash GDI+
//        for (int i = 1; i < n; ++i) if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;
//
//        LinearGradientBrush brush(p1, p2, col.front(), col.back());
//        brush.SetInterpolationColors(col.data(), pos.data(), n);
//        brush.SetWrapMode(getGdiWrapMode(linear->getSpreadMethod()));
//
//        if (linear->getGradientTransform()) brush.SetTransform(linear->getGradientTransform());
//
//        g.FillPath(&brush, &path);
//    }
//
//    // --- XỬ LÝ RADIAL GRADIENT ---
//    else if (auto* radial = dynamic_cast<const SVGRadialGradient*>(resolvedGrad)) {
//        std::vector<SVGStop> stops = radial->getStopArray();
//        if (stops.empty()) return;
//
//        // Tính toán cx, cy, r
//        float cx, cy, r;
//        if (radial->getGradientUnits() == "userSpaceOnUse") {
//            cx = radial->getCX(); cy = radial->getCY(); r = radial->getR();
//        }
//        else {
//            cx = bounds.X + radial->getCX() * bounds.Width;
//            cy = bounds.Y + radial->getCY() * bounds.Height;
//            r = radial->getR() * (std::max)(bounds.Width, bounds.Height);
//        }
//        if (r <= 0.5f) r = 0.5f; // Tránh bán kính bằng 0 gây crash
//
//        GraphicsPath ellipsePath;
//        ellipsePath.AddEllipse(cx - r, cy - r, r * 2, r * 2);
//        PathGradientBrush brush(&ellipsePath);
//
//        // QUAN TRỌNG: Đặt tâm để không crash nếu cx, cy nằm ngoài ellipse
//        brush.SetCenterPoint(PointF(cx, cy));
//
//        int n = (int)stops.size();
//        std::vector<REAL> pos(n);
//        std::vector<Color> col(n);
//
//        // Đảo ngược mảng (SVG: tâm ra ngoài -> GDI+: ngoài vào tâm)
//        for (int i = 0; i < n; ++i) {
//            int idx = n - 1 - i;
//            pos[i] = 1.0f - stops[idx].getOffset();
//            col[i] = Color(BYTE(stops[idx].getStopOpacity() * 255), stops[idx].getStopColor().r, stops[idx].getStopColor().g, stops[idx].getStopColor().b);
//        }
//
//        // Đảm bảo mảng pos tăng dần
//        for (int i = 1; i < n; ++i) if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;
//
//        brush.SetInterpolationColors(col.data(), pos.data(), n);
//        brush.SetWrapMode(getGdiWrapMode(radial->getSpreadMethod()));
//
//        if (radial->getGradientTransform()) brush.SetTransform(radial->getGradientTransform());
//
//        g.FillPath(&brush, &path);
//    }
//}
void SVGRenderer::renderGradientPathFill(Gdiplus::Graphics& g, const SVGGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds, const SVGDocumentContext& context) const {
    if (!grad) return;

    // Tự nạp stops từ xlink:href
    grad->resolveReference(context);

    if (auto* linear = dynamic_cast<const SVGLinearGradient*>(grad)) {
        this->renderLinearGradientFill(g, linear, path, bounds);
    }
    else if (auto* radial = dynamic_cast<const SVGRadialGradient*>(grad)) {
        this->renderRadialGradientFill(g, radial, path, bounds);
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

//void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* pathElement, const std::vector<PathCommand>& commands, const SVGDocumentContext& context) const {
//    if (!pathElement) return;
//    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
//    // lưu ma trận hiện tại
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    // áp dụng ma trận biến đội
//    const Gdiplus::Matrix* matrix = pathElement->getTransformMatrix();
//    if (matrix) {
//        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    }
//
//    // 1. XÂY DỰNG GRAPHICS PATH TỪ CÁC COMMANDS
//    Gdiplus::GraphicsPath path;
//    Gdiplus::PointF currentPoint(0.0f, 0.0f), startPoint(0.0f, 0.0f);
//    bool subpathStarted = false;
//
//    for (const auto& cmd : commands) {
//        // 
//        switch (cmd.command) {
//        case 'M':
//        case 'm': {
//            // M: MoveTo
//            currentPoint = Gdiplus::PointF(cmd.parameters[0], cmd.parameters[1]);
//            startPoint = currentPoint;
//            if (subpathStarted) {
//                path.StartFigure(); // Bắt đầu hình dạng mới
//            }
//            subpathStarted = true;
//            break;
//        }
//        case 'L':
//        case 'l': {
//            // L: LineTo
//            Gdiplus::PointF p(cmd.parameters[0], cmd.parameters[1]);
//            path.AddLine(currentPoint, p);
//            currentPoint = p;
//            break;
//        }
//                // ... (Thêm các lệnh khác H, V, Q, S, A nếu cần) ...
//        case 'H': { // Horizontal LineTo
//            float x = cmd.parameters[0];
//            Gdiplus::PointF p(x, currentPoint.Y);
//            path.AddLine(currentPoint, p);
//            currentPoint = p;
//            break;
//        }
//        case 'V': { // Vertical LineTo
//            float y = cmd.parameters[0];
//            Gdiplus::PointF p(currentPoint.X, y);
//            path.AddLine(currentPoint, p);
//            currentPoint = p;
//            break;
//        }
//        case 'C': {
//            // C: Cubic Bezier
//            Gdiplus::PointF p1(cmd.parameters[0], cmd.parameters[1]);
//            Gdiplus::PointF p2(cmd.parameters[2], cmd.parameters[3]);
//            Gdiplus::PointF p3(cmd.parameters[4], cmd.parameters[5]);
//            path.AddBezier(currentPoint, p1, p2, p3);
//            currentPoint = p3;
//            break;
//        }
//        case 'Z':
//        case 'z': {
//            // Z: ClosePath
//            path.CloseFigure();
//            currentPoint = startPoint;
//            subpathStarted = false;
//            break;
//        }
//        }
//    }
//
//    // 2. TÍNH TOÁN BOUNDS
//    Gdiplus::RectF bounds;
//    path.GetBounds(&bounds);
//
//    // 3. LOGIC FILL (Gradient HOẶC Màu đơn)
//    if (pathElement->getSVGStyle().hasGradientFill()) {
//        const SVGGradient* grad = pathElement->getSVGStyle().getGrad();
//        if (grad) {
//            // Sử dụng hàm hỗ trợ Fill Path (Đã sửa ở câu trước)
//            renderGradientPathFill(g, grad, path, bounds, context);
//        }
//    }
//    else {
//        // TRƯỜNG HỢP MÀU ĐƠN
//        Gdiplus::Color fillColor = pathElement->getSVGStyle().getGdiFillColor();
//        if (fillColor.GetA() > 0) {
//            Gdiplus::SolidBrush brush(fillColor);
//            g.FillPath(&brush, &path);
//        }
//    }
//
//    // 4. LOGIC STROKE (CHỈ MÀU ĐƠN)
//    Stroke* strokeObj = pathElement->getSVGStyle().getStroke();
//    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
//        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
//        if (finalStrokeColor.GetA() > 0) {
//            // Pen chỉ dùng màu đơn (theo quyết định trước đó)
//            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);
//            g.DrawPath(&pen, &path);
//        }
//    }
//
//    // Khôi phục transform ban đầu
//    g.SetTransform(&oldMatrix);
//}

//void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* pathElement, const std::vector<PathCommand>& commands, const SVGDocumentContext& context)
//void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* path) {
//    if (!path) return;
//    Matrix oldMatrix; g.GetTransform(&oldMatrix);
//    Matrix localMatrix; path->getTransform().applyToMatrix(localMatrix);
//    g.MultiplyTransform(&localMatrix);
//
//    Gdiplus::GraphicsPath gp;
//    PointF current(0.0f, 0.0f);
//
//    // Mặc định SVG dùng luật Winding, nhưng GDI+ mặc định là Alternate.
//    // Nếu hình vẫn bị thủng lỗ sai, hãy thử bật dòng dưới:
//    // gp.SetFillMode(Gdiplus::FillModeWinding);
//
//    for (const auto& cmd : path->getCommands()) {
//        switch (cmd.type) {
//        case PathCommandType::MoveTo: {
//            gp.StartFigure(); // QUAN TRỌNG: Ngắt nét để không nối với điểm cũ
//            current = PointF(cmd.params[0], cmd.params[1]);
//            break;
//        }
//        case PathCommandType::LineTo: {
//            PointF p(cmd.params[0], cmd.params[1]);
//            gp.AddLine(current, p);
//            current = p;
//            break;
//        }
//        case PathCommandType::HLineTo: {
//            PointF p(cmd.params[0], current.Y);
//            gp.AddLine(current, p);
//            current = p;
//            break;
//        }
//        case PathCommandType::VLineTo: {
//            PointF p(current.X, cmd.params[0]);
//            gp.AddLine(current, p);
//            current = p;
//            break;
//        }
//        case PathCommandType::ClosePath: {
//            gp.CloseFigure(); // QUAN TRỌNG: Đóng hình chuẩn
//            Gdiplus::PointF lastPoint;
//            gp.GetLastPoint(&lastPoint);
//            current = lastPoint;
//            break;
//        }
//        case PathCommandType::CubicBezier: {
//            PointF p1(cmd.params[0], cmd.params[1]);
//            PointF p2(cmd.params[2], cmd.params[3]);
//            PointF p3(cmd.params[4], cmd.params[5]);
//            gp.AddBezier(current, p1, p2, p3);
//            current = p3;
//            break;
//        }
//        case PathCommandType::SmoothCubicBezier: {
//            PointF p1(cmd.params[0], cmd.params[1]);
//            PointF p2(cmd.params[2], cmd.params[3]);
//            PointF p3(cmd.params[4], cmd.params[5]);
//            gp.AddBezier(current, p1, p2, p3);
//            current = p3;
//            break;
//        }
//        }
//    }
//
//    const SVGStyle& st = path->getSVGStyle();
//    Color fillColor = st.getGdiFillColor();
//    Stroke* strokeObj = st.getStroke();
//
//    if (fillColor.GetA() > 0) {
//        SolidBrush brush(fillColor);
//        g.FillPath(&brush, &gp);
//    }
//    if (strokeObj) {
//        Color strokeColor = strokeObj->getGdiColor();
//        float strokeW = strokeObj->strokeWidth > 0 ? strokeObj->strokeWidth : 1.0f;
//        Pen pen(strokeColor, strokeW);
//        g.DrawPath(&pen, &gp);
//    }
//    g.SetTransform(&oldMatrix);
//}

void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* pathElement, const SVGDocumentContext& context) const {
    if (!pathElement) return;

    // 1. Chống răng cưa mức độ cao
    g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    // 2. Xử lý ma trận biến đổi (Sửa Prepend thành Append)
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);
    const Gdiplus::Matrix* pathMatrix = pathElement->getTransformMatrix();
    if (pathMatrix) {
        // Sử dụng Append để đảm bảo ma trận của Path được áp dụng SAU ma trận Group
        g.MultiplyTransform(pathMatrix, Gdiplus::MatrixOrderAppend);
    }

    // 3. Xây dựng GraphicsPath
    Gdiplus::GraphicsPath gdiPath;

    // Xử lý lỗ thủng: Logo Firefox bắt buộc dùng Alternate (Even-Odd)
    FillRule svgFillRule = pathElement->getSVGStyle().getFillRule();
    gdiPath.SetFillMode(svgFillRule == FillRule::EvenOdd ?
        Gdiplus::FillModeAlternate : Gdiplus::FillModeWinding);

    Gdiplus::PointF currentPoint(0, 0);
    Gdiplus::PointF subPathStart(0, 0);
    bool figureStarted = false;

    for (const auto& cmd : pathElement->getCommands()) {
        switch (cmd.type) {
        case PathCommandType::MoveTo:
            // Cập nhật điểm bắt đầu của một sub-path mới
            currentPoint = { cmd.params[0], cmd.params[1] };
            subPathStart = currentPoint;
            // Bắt đầu Figure mới để ngắt các đường nối rác
            gdiPath.StartFigure();
            figureStarted = true;
            break;

        case PathCommandType::LineTo:
            gdiPath.AddLine(currentPoint, { cmd.params[0], cmd.params[1] });
            currentPoint = { cmd.params[0], cmd.params[1] };
            break;

        case PathCommandType::HLineTo:
            gdiPath.AddLine(currentPoint, { cmd.params[0], currentPoint.Y });
            currentPoint.X = cmd.params[0];
            break;

        case PathCommandType::VLineTo:
            gdiPath.AddLine(currentPoint, { currentPoint.X, cmd.params[0] });
            currentPoint.Y = cmd.params[0];
            break;

        case PathCommandType::CubicBezier:
        case PathCommandType::SmoothCubicBezier:
            gdiPath.AddBezier(currentPoint,
                { cmd.params[0], cmd.params[1] },
                { cmd.params[2], cmd.params[3] },
                { cmd.params[4], cmd.params[5] });
            currentPoint = { cmd.params[4], cmd.params[5] };
            break;

        case PathCommandType::Arc:
            // Hàm addSvgArcToPath cần được đảm bảo đã chuẩn hóa tọa độ
            addSvgArcToPath(gdiPath, currentPoint, { cmd.params[5], cmd.params[6] },
                cmd.params[0], cmd.params[1], cmd.params[2],
                cmd.params[3] != 0, cmd.params[4] != 0);
            currentPoint = { cmd.params[5], cmd.params[6] };
            break;

        case PathCommandType::ClosePath:
            gdiPath.CloseFigure();
            currentPoint = subPathStart; // Quay về điểm bắt đầu của đoạn Figure hiện tại
            break;
        }
    }

    // 4. Đổ màu và Vẽ viền
    Gdiplus::RectF bounds;
    gdiPath.GetBounds(&bounds);

    // Xử lý Fill (Gradient hoặc Solid)
    if (pathElement->getSVGStyle().hasGradientFill()) {
        const SVGGradient* grad = pathElement->getSVGStyle().getGrad();
        if (grad) {
            renderGradientPathFill(g, grad, gdiPath, bounds, context);
        }
    }
    else {
        Gdiplus::Color fillColor = pathElement->getSVGStyle().getGdiFillColor();
        if (fillColor.GetA() > 0) {
            Gdiplus::SolidBrush brush(fillColor);
            g.FillPath(&brush, &gdiPath);
        }
    }

    // Xử lý Stroke
    Stroke* strokeObj = pathElement->getSVGStyle().getStroke();
    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        Gdiplus::Color strokeColor = strokeObj->getGdiColor();
        if (strokeColor.GetA() > 0) {
            Gdiplus::Pen pen(strokeColor, strokeObj->strokeWidth);
            pen.SetMiterLimit(strokeObj->getStrokeMiterLimit());
            pen.SetLineJoin(Gdiplus::LineJoinRound); // Bo góc để các mảnh khít nhau hơn
            g.DrawPath(&pen, &gdiPath);
        }
    }

    // 5. Khôi phục ma trận gốc
    g.SetTransform(&oldMatrix);
}
//void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* pathElement, const SVGDocumentContext& context) const {
//    if (!pathElement) return;
//
//    // 1. Cấu hình chất lượng vẽ (Khử răng cưa để các mảnh ghép mịn hơn)
//    g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
//
//    // 2. Xử lý Ma trận biến đổi (Transform)
//    Gdiplus::Matrix oldMatrix;
//    g.GetTransform(&oldMatrix);
//    const Gdiplus::Matrix* matrix = pathElement->getTransformMatrix();
//    if (matrix) {
//        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
//    }
//
//    // 3. Xây dựng đối tượng GraphicsPath
//    Gdiplus::GraphicsPath path;
//
//    // Xử lý FillRule (Winding cho Firefox/Instagram, EvenOdd nếu được chỉ định)
//    FillRule svgFillRule = pathElement->getSVGStyle().getFillRule();
//    if (svgFillRule == FillRule::EvenOdd) {
//        // Tương ứng với "evenodd" trong SVG
//        path.SetFillMode(Gdiplus::FillModeAlternate);
//    }
//    else {
//        // Tương ứng với "nonzero" trong SVG (Mặc định)
//        path.SetFillMode(Gdiplus::FillModeWinding);
//    }
//
//    Gdiplus::PointF currentPoint(0.0f, 0.0f);
//    Gdiplus::PointF subPathStartPoint(0.0f, 0.0f);
//
//    const std::vector<PathCommand>& commands = pathElement->getCommands();
//
//    for (const auto& cmd : commands) {
//        switch (cmd.type) {
//        case PathCommandType::MoveTo:
//            // BẮT BUỘC: StartFigure để ngắt các Sub-path (tránh vệt đen xuyên tâm)
//            path.StartFigure();
//            currentPoint = Gdiplus::PointF(cmd.params[0], cmd.params[1]);
//            subPathStartPoint = currentPoint;
//            break;
//
//        case PathCommandType::LineTo:
//            path.AddLine(currentPoint, Gdiplus::PointF(cmd.params[0], cmd.params[1]));
//            currentPoint = Gdiplus::PointF(cmd.params[0], cmd.params[1]);
//            break;
//
//        case PathCommandType::HLineTo:
//            path.AddLine(currentPoint, Gdiplus::PointF(cmd.params[0], currentPoint.Y));
//            currentPoint.X = cmd.params[0];
//            break;
//
//        case PathCommandType::VLineTo:
//            path.AddLine(currentPoint, Gdiplus::PointF(currentPoint.X, cmd.params[0]));
//            currentPoint.Y = cmd.params[0];
//            break;
//
//        case PathCommandType::CubicBezier:
//        case PathCommandType::SmoothCubicBezier:
//            // Cả C và S đều dùng chung AddBezier vì Parser đã tính toán điểm đối xứng cho S
//            path.AddBezier(currentPoint,
//                Gdiplus::PointF(cmd.params[0], cmd.params[1]), // Control point 1
//                Gdiplus::PointF(cmd.params[2], cmd.params[3]), // Control point 2
//                Gdiplus::PointF(cmd.params[4], cmd.params[5])); // End point
//            currentPoint = Gdiplus::PointF(cmd.params[4], cmd.params[5]);
//            break;
//
//        case PathCommandType::Arc:
//            // Sử dụng hàm helper chuyển đổi SVG Arc sang phân đoạn Bezier
//            addSvgArcToPath(path, currentPoint, Gdiplus::PointF(cmd.params[5], cmd.params[6]),
//                cmd.params[0], cmd.params[1], cmd.params[2],
//                cmd.params[3] != 0, cmd.params[4] != 0);
//            currentPoint = Gdiplus::PointF(cmd.params[5], cmd.params[6]);
//            break;
//
//        case PathCommandType::ClosePath:
//            path.CloseFigure(); // Đóng kín Sub-path
//            currentPoint = subPathStartPoint; // Đồng bộ quay về điểm đầu của Sub-path
//            break;
//        }
//    }
//
//    // 4. LOGIC FILL (Tô màu nội dung)
//    Gdiplus::RectF bounds;
//    path.GetBounds(&bounds);
//
//    if (pathElement->getSVGStyle().hasGradientFill()) {
//        const SVGGradient* grad = pathElement->getSVGStyle().getGrad();
//        if (grad) {
//            renderGradientPathFill(g, grad, path, bounds, context);
//        }
//    }
//    else {
//        Gdiplus::Color fillColor = pathElement->getSVGStyle().getGdiFillColor();
//        if (fillColor.GetA() > 0) {
//            Gdiplus::SolidBrush brush(fillColor);
//            g.FillPath(&brush, &path);
//        }
//    }
//
//    // 5. LOGIC STROKE (Vẽ đường viền)
//    Stroke* strokeObj = pathElement->getSVGStyle().getStroke();
//    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
//        Gdiplus::Color strokeColor = strokeObj->getGdiColor();
//        if (strokeColor.GetA() > 0) {
//            Gdiplus::Pen pen(strokeColor, strokeObj->strokeWidth);
//            pen.SetMiterLimit(strokeObj->getStrokeMiterLimit());
//            pen.SetLineJoin(Gdiplus::LineJoinRound); // Giúp các góc nối mượt mà hơn
//            g.DrawPath(&pen, &path);
//        }
//    }
//
//    // 6. Khôi phục trạng thái Graphics Context
//    g.SetTransform(&oldMatrix);
//}
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

void SVGRenderer::addSvgArcToPath(Gdiplus::GraphicsPath& path,
    Gdiplus::PointF start, Gdiplus::PointF end,
    float rx, float ry, float rot,
    bool largeArc, bool sweep) const
{
    double x1 = start.X; double y1 = start.Y;
    double x2 = end.X;   double y2 = end.Y;

    // 1. Kiểm tra điều kiện biên
    if (x1 == x2 && y1 == y2) return;
    if (rx == 0 || ry == 0) {
        path.AddLine(start, end);
        return;
    }

    rx = std::abs(rx); ry = std::abs(ry);
    double phi = rot * M_PI / 180.0;
    double cosPhi = std::cos(phi);
    double sinPhi = std::sin(phi);

    // 2. Chuyển đổi sang hệ tọa độ trung gian (x1', y1')
    double dx2 = (x1 - x2) / 2.0;
    double dy2 = (y1 - y2) / 2.0;
    double x1p = cosPhi * dx2 + sinPhi * dy2;
    double y1p = -sinPhi * dx2 + cosPhi * dy2;

    double rx2 = rx * rx; double ry2 = ry * ry;
    double x1p2 = x1p * x1p; double y1p2 = y1p * y1p;

    // Hiệu chỉnh bán kính nếu điểm đầu/cuối nằm ngoài phạm vi elip
    double check = x1p2 / rx2 + y1p2 / ry2;
    if (check > 1) {
        rx *= std::sqrt(check);
        ry *= std::sqrt(check);
        rx2 = rx * rx; ry2 = ry * ry;
    }

    // 3. Tính toán tâm elip trong hệ tọa độ trung gian (cx', cy')
    double sign = (largeArc == sweep) ? -1.0 : 1.0;
    double num = rx2 * ry2 - rx2 * y1p2 - ry2 * x1p2;
    double den = rx2 * y1p2 + ry2 * x1p2;
    double factor = sign * std::sqrt((std::max)(0.0, num / den));

    double cxp = factor * (rx * y1p / ry);
    double cyp = factor * (-ry * x1p / rx);

    // 4. Tính toán góc bắt đầu (theta1) và góc quét (deltaTheta)
    auto getAngle = [](double ux, double uy, double vx, double vy) {
        double dot = ux * vx + uy * vy;
        double len = std::sqrt(ux * ux + uy * uy) * std::sqrt(vx * vx + vy * vy);
        double ang = std::acos((std::max)(-1.0, (std::min)(1.0, dot / len)));
        if (ux * vy - uy * vx < 0) ang = -ang;
        return ang;
        };

    double theta1 = getAngle(1, 0, (x1p - cxp) / rx, (y1p - cyp) / ry);
    double deltaTheta = getAngle((x1p - cxp) / rx, (y1p - cyp) / ry, (-x1p - cxp) / rx, (-y1p - cyp) / ry);

    if (!sweep && deltaTheta > 0) deltaTheta -= 2 * M_PI;
    else if (sweep && deltaTheta < 0) deltaTheta += 2 * M_PI;

    // 5. Chia cung tròn thành các phân đoạn Bezier (tối đa 90 độ/đoạn)
    int segments = (int)std::ceil(std::abs(deltaTheta) / (M_PI / 2.0));
    double delta = deltaTheta / segments;
    double t = (4.0 / 3.0) * std::tan(delta / 4.0);

    Gdiplus::PointF lastPoint = start;

    for (int i = 0; i < segments; ++i) {
        double a0 = theta1 + i * delta;
        double a1 = a0 + delta;

        double cosA0 = std::cos(a0); double sinA0 = std::sin(a0);
        double cosA1 = std::cos(a1); double sinA1 = std::sin(a1);

        // Các điểm trong hệ tọa độ elip chưa xoay
        auto transform = [&](double x, double y) {
            double tx = cosPhi * x - sinPhi * y + (cosPhi * cxp - sinPhi * cyp + (x1 + x2) / 2.0);
            double ty = sinPhi * x + cosPhi * y + (sinPhi * cxp + cosPhi * cyp + (y1 + y2) / 2.0);
            return Gdiplus::PointF((float)tx, (float)ty);
            };

        Gdiplus::PointF p1 = transform(rx * (cosA0 - t * sinA0), ry * (sinA0 + t * cosA0));
        Gdiplus::PointF p2 = transform(rx * (cosA1 + t * sinA1), ry * (sinA1 - t * cosA1));
        Gdiplus::PointF pEnd = (i == segments - 1) ? end : transform(rx * cosA1, ry * sinA1);

        path.AddBezier(lastPoint, p1, p2, pEnd);
        lastPoint = pEnd;
    }
}
Gdiplus::WrapMode SVGRenderer::getGdiWrapMode(const std::string& spreadMethod) const {
    if (spreadMethod == "reflect") return Gdiplus::WrapModeTileFlipXY;
    if (spreadMethod == "repeat") return Gdiplus::WrapModeTile;
    // BẮT BUỘC: Mặc định phải là Clamp (tương ứng với "pad" trong SVG)
    return Gdiplus::WrapModeClamp;
}

//void SVGRenderer::renderLinearGradientFill(Gdiplus::Graphics& g, const SVGLinearGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    if (!grad) return;
//
//    // 1. Lấy danh sách stop màu
//    std::vector<SVGStop> stops = grad->getStopArray();
//    if (stops.empty()) return;
//
//    // 2. Normalize: Nếu stop đầu/cuối chưa chạm mốc 0.0 hoặc 1.0, chèn thêm để màu không bị hở
//    if (stops.front().getOffset() > 0.0f) {
//        stops.insert(stops.begin(), SVGStop(stops.front().getStopColor(), 0.0f, stops.front().getStopOpacity()));
//    }
//    if (stops.back().getOffset() < 1.0f) {
//        stops.push_back(SVGStop(stops.back().getStopColor(), 1.0f, stops.back().getStopOpacity()));
//    }
//
//    // 3. Tính tọa độ p1, p2 dựa trên GradientUnits
//    Gdiplus::PointF p1, p2;
//    if (grad->getGradientUnits() == "objectBoundingBox") {
//        p1 = mapPoint(grad->getX1(), grad->getY1(), bounds);
//        p2 = mapPoint(grad->getX2(), grad->getY2(), bounds);
//    }
//    else {
//        p1 = { grad->getX1(), grad->getY1() };
//        p2 = { grad->getX2(), grad->getY2() };
//    }
//
//    // 4. Chuẩn bị mảng ColorBlend (InterpolationColors)
//    int n = (int)stops.size();
//    std::vector<Gdiplus::REAL> pos(n);
//    std::vector<Gdiplus::Color> col(n);
//    for (int i = 0; i < n; ++i) {
//        pos[i] = stops[i].getOffset();
//        // Bạn nhớ dùng hàm tính cả Opacity: Color(A, R, G, B)
//        col[i] = Gdiplus::Color(BYTE(stops[i].getStopOpacity() * 255), stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b);
//    }
//
//    // 5. Tạo Brush và áp dụng
//    Gdiplus::LinearGradientBrush brush(p1, p2, col.front(), col.back());
//    brush.SetInterpolationColors(col.data(), pos.data(), n);
//    brush.SetWrapMode(toGdiWrapMode(grad->getSpreadMethod()));
//
//    if (grad->getGradientTransform()) {
//        brush.SetTransform(grad->getGradientTransform());
//    }
//
//    // BẮT BUỘC: FillPath thay vì FillRectangle
//    g.FillPath(&brush, &path);
//}

// Hàm bổ trợ: Điều chỉnh độ loang
float applyBias(float x, float bias) {
    if (x <= 0.0f) return 0.0f;
    if (x >= 1.0f) return 1.0f;
    return x / ((1.0f / bias - 2.0f) * (1.0f - x) + 1.0f);
}

// Hàm bổ trợ: Tính độ sáng của màu
float getLuminance(Gdiplus::Color c) {
    return 0.299f * c.GetR() + 0.587f * c.GetG() + 0.114f * c.GetB();
}

//ban 1
void SVGRenderer::renderLinearGradientFill(Gdiplus::Graphics& g, const SVGLinearGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
    using namespace Gdiplus;
    if (!grad) return;

    std::vector<SVGStop> stops = grad->getStopArray();
    if (stops.empty()) return;

    // 1. Tính tọa độ gốc từ SVG
    PointF p1_svg, p2_svg;
    if (grad->getGradientUnits() == "objectBoundingBox") {
        p1_svg = { bounds.X + grad->getX1() * bounds.Width, bounds.Y + grad->getY1() * bounds.Height };
        p2_svg = { bounds.X + grad->getX2() * bounds.Width, bounds.Y + grad->getY2() * bounds.Height };
    }
    else {
        p1_svg = { grad->getX1(), grad->getY1() };
        p2_svg = { grad->getX2(), grad->getY2() };
    }

    // 2. KỸ THUẬT NỚI RỘNG ĐỂ PAD:
    // Ta lấy P1 và P2 thực tế là hai góc xa nhất của Bounding Box theo hướng vector
    // Nhưng để đơn giản và hiệu quả nhất cho Linear, ta nới rộng P1-P2 ra một khoảng rất lớn
    // hoặc khớp với đường chéo của bounds.
    float dx = p2_svg.X - p1_svg.X;
    float dy = p2_svg.Y - p1_svg.Y;
    float len_svg = sqrt(dx * dx + dy * dy);
    if (len_svg < 0.1f) return; // Tránh chia cho 0

    // Tính vector đơn vị
    float ux = dx / len_svg;
    float uy = dy / len_svg;

    // Nới rộng P1 về phía ngược lại và P2 về phía tới (ví dụ nới thêm 1000 đơn vị để bao phủ)
    // Hoặc tính toán chính xác để bao phủ bounds:
    float extension = (std::max)(bounds.Width, bounds.Height) * 2.0f;
    PointF p1_ext = { p1_svg.X - ux * extension, p1_svg.Y - uy * extension };
    PointF p2_ext = { p2_svg.X + ux * extension, p2_svg.Y + uy * extension };

    float len_ext = len_svg + 2.0f * extension;
    float start_ratio = extension / len_ext;
    float svg_ratio = len_svg / len_ext;

    // 3. Mảng màu: Nén dải màu SVG vào đoạn giữa của vector mới
    int n = (int)stops.size();
    std::vector<REAL> pos; std::vector<Color> col;

    // PAD ĐẦU: Màu của Stop đầu tiên kéo dài từ p1_ext đến p1_svg
    pos.push_back(0.0f);
    col.push_back(Color(BYTE(stops.front().getStopOpacity() * 255),
        stops.front().getStopColor().r, stops.front().getStopColor().g, stops.front().getStopColor().b));

    for (int i = 0; i < n; ++i) {
        // Áp dụng Bias nếu cần (như đã làm với Radial)
        float offset = stops[i].getOffset();
        float adjustedOffset = offset;

        // Nếu muốn vàng tươi hơn cho ID 'l', có thể dùng applyBias ở đây
        if (grad->getGradientID() == "l") adjustedOffset = applyBias(offset, 0.45f);

        pos.push_back(start_ratio + adjustedOffset * svg_ratio);
        col.push_back(Color(BYTE(stops[i].getStopOpacity() * 255),
            stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b));
    }

    // PAD CUỐI: Màu của Stop cuối cùng kéo dài từ p2_svg đến p2_ext
    pos.push_back(1.0f);
    col.push_back(col.back());

    // Sửa lỗi trùng offset
    for (size_t i = 1; i < pos.size(); ++i) if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;

    // 4. Tạo Brush và vẽ
    LinearGradientBrush brush(p1_ext, p2_ext, col.front(), col.back());
    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());

    // Dùng WrapModeClamp (Pad) - cực kỳ an toàn vì vector đã nới rộng
    brush.SetWrapMode(WrapModeClamp);
    brush.SetGammaCorrection(FALSE);

    if (grad->getGradientTransform()) {
        brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderPrepend);
    }

    g.FillPath(&brush, &path);
}

//// ban chinh sau ban cu 
//void SVGRenderer::renderLinearGradientFill(Gdiplus::Graphics& g, const SVGLinearGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    using namespace Gdiplus;
//    if (!grad) return;
//
//    std::vector<SVGStop> stops = grad->getStopArray();
//    std::cout << "\n>>>>>> LINEAR INTENSITY DEBUG: " << grad->getGradientID() << " <<<<<<" << std::endl;
//    if (stops.empty()) return;
//
//    // 1. Tính toán tọa độ (Đã chuẩn theo log P1:402, P2:102)
//    PointF p1 = { bounds.X + grad->getX1() * bounds.Width, bounds.Y + grad->getY1() * bounds.Height };
//    PointF p2 = { bounds.X + grad->getX2() * bounds.Width, bounds.Y + grad->getY2() * bounds.Height };
//
//    // 2. Xử lý màu sắc - ÉP ĐẬM BÊN PHẢI (P1)
//    std::vector<REAL> pos;
//    std::vector<Color> col;
//
//    for (int i = 0; i < (int)stops.size(); ++i) {
//        float offset = stops[i].getOffset();
//        int alpha = (int)(stops[i].getStopOpacity() * 255);
//
//        // KỸ THUẬT ĐẶC TRỊ CHO ID 'l':
//        if (grad->getGradientID() == "l" && i == 0) {
//            // Tăng alpha gốc của stop 1 lên mức tối đa nếu cần
//            alpha = (std::max)(alpha, 230);
//
//            // Chèn stop hiện tại
//            pos.push_back(offset);
//            col.push_back(Color(alpha, stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b));
//
//            // CHÈN THÊM STOP ẢO: Giữ nguyên màu này tới 35% quãng đường (0.35)
//            // Việc này giúp bên phải (điểm bắt đầu) đậm và dày màu hơn
//            pos.push_back(0.35f);
//            col.push_back(Color(alpha, stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b));
//
//            std::cout << "[STRETCH] Holding color intensity from 0.0 to 0.35" << std::endl;
//            continue;
//        }
//
//        pos.push_back(offset);
//        col.push_back(Color(alpha, stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b));
//    }
//
//    // 3. Sắp xếp và Padding
//    for (size_t i = 1; i < pos.size(); ++i) {
//        if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;
//    }
//    if (pos.back() < 1.0f) { pos.push_back(1.0f); col.push_back(col.back()); }
//
//    // 4. Khởi tạo Brush
//    LinearGradientBrush brush(p1, p2, col.front(), col.back());
//    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());
//    brush.SetGammaCorrection(FALSE);
//    brush.SetWrapMode(WrapModeTileFlipXY);
//
//    if (grad->getGradientTransform()) {
//        brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderPrepend);
//    }
//
//    g.FillPath(&brush, &path);
//    std::cout << "[RESULT] Linear render finished with strengthened right side." << std::endl;
//    std::cout << "==========================================\n" << std::endl;
//}

//// ban nay ben trai hoi nhat lam lo lop duoi khien phan duoi dam mau
//void SVGRenderer::renderLinearGradientFill(Gdiplus::Graphics& g, const SVGLinearGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    if (!grad) return;
//
//    std::vector<SVGStop> stops = grad->getStopArray();
//    if (stops.empty()) return;
//
//    // 1. Tính toán p1, p2 chính xác theo BoundingBox
//    Gdiplus::PointF p1, p2;
//    if (grad->getGradientUnits() == "objectBoundingBox") {
//        p1.X = bounds.X + grad->getX1() * bounds.Width;
//        p1.Y = bounds.Y + grad->getY1() * bounds.Height;
//        p2.X = bounds.X + grad->getX2() * bounds.Width;
//        p2.Y = bounds.Y + grad->getY2() * bounds.Height;
//    }
//    else {
//        p1 = { grad->getX1(), grad->getY1() };
//        p2 = { grad->getX2(), grad->getY2() };
//    }
//
//    // DEBUG: Kiểm tra tọa độ vẽ
//    std::cout << "[DEBUG LINEAR] ID: " << grad->getGradientID()
//        << " P1(" << p1.X << "," << p1.Y << ") P2(" << p2.X << "," << p2.Y << ")" << std::endl;
//
//    // 2. Chuẩn bị mảng màu
//    int n = (int)stops.size();
//    std::vector<Gdiplus::REAL> pos(n);
//    std::vector<Gdiplus::Color> col(n);
//    for (int i = 0; i < n; ++i) {
//        pos[i] = stops[i].getOffset();
//        col[i] = Gdiplus::Color(BYTE(stops[i].getStopOpacity() * 255),
//            stops[i].getStopColor().r,
//            stops[i].getStopColor().g,
//            stops[i].getStopColor().b);
//    }
//
//    // 3. Xử lý "Pad" màu để không bị thừa nét hoặc khuyết đỉnh
//    // Đảm bảo dải màu bao phủ từ 0.0 đến 1.0
//    if (pos.front() > 0.0f) {
//        pos.insert(pos.begin(), 0.0f);
//        col.insert(col.begin(), col.at(1));
//    }
//    if (pos.back() < 1.0f) {
//        pos.push_back(1.0f);
//        col.push_back(col.back());
//    }
//
//    // 4. Tạo Brush
//    Gdiplus::LinearGradientBrush brush(p1, p2, col.front(), col.back());
//
//    // THIẾT LẬP QUAN TRỌNG: GDI+ cần SetInterpolationColors 
//    // sau khi khởi tạo để ghi đè dải màu mặc định
//    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());
//
//    // SVG spreadMethod="pad" tương đương với WrapModeClamp trong GDI+
//    // Tuy nhiên, LinearGradientBrush thường dùng WrapModeTile hoặc TileFlip.
//    // Để "Pad" chuẩn, ta sử dụng WrapModeTileFlipXY hoặc Tile.
//    brush.SetWrapMode(Gdiplus::WrapModeTileFlipXY);
//
//    // 5. Áp dụng Transform
//    if (grad->getGradientTransform()) {
//        brush.MultiplyTransform(grad->getGradientTransform(), Gdiplus::MatrixOrderPrepend);
//    }
//
//    // Vẽ
//    g.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
//    g.FillPath(&brush, &path);
//}

////chay dươc roi nhung mau chua dung lam, ban nay mau hoi nhat - ban 2
//void SVGRenderer::renderLinearGradientFill(Gdiplus::Graphics& g, const SVGLinearGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    using namespace Gdiplus;
//    if (!grad) return;
//
//    std::vector<SVGStop> stops = grad->getStopArray();
//
//    std::cout << "\n>>>>>> LINEAR COORDINATE DEBUG: " << grad->getGradientID() << " <<<<<<" << std::endl;
//    if (stops.empty()) return;
//
//    // 1. Lấy thông số gốc từ SVG
//    float x1_raw = grad->getX1();
//    float y1_raw = grad->getY1();
//    float x2_raw = grad->getX2();
//    float y2_raw = grad->getY2();
//
//    std::cout << "[SVG RAW] x1=" << x1_raw * 100 << "%, y1=" << y1_raw * 100
//        << "%, x2=" << x2_raw * 100 << "%, y2=" << y2_raw * 100 << "%" << std::endl;
//
//    // 2. Tính toán tọa độ thực tế (Pixel)
//    PointF p1, p2;
//    if (grad->getGradientUnits() == "objectBoundingBox") {
//        p1 = { bounds.X + x1_raw * bounds.Width, bounds.Y + y1_raw * bounds.Height };
//        p2 = { bounds.X + x2_raw * bounds.Width, bounds.Y + y2_raw * bounds.Height };
//        std::cout << "[MAP] Units: objectBoundingBox" << std::endl;
//    }
//    else {
//        p1 = { x1_raw, y1_raw };
//        p2 = { x2_raw, y2_raw };
//        std::cout << "[MAP] Units: userSpaceOnUse" << std::endl;
//    }
//
//    std::cout << "[PIXEL] P1:(" << p1.X << ", " << p1.Y << ") -> P2:(" << p2.X << ", " << p2.Y << ")" << std::endl;
//    std::cout << "[BOUNDS] X=" << bounds.X << ", Y=" << bounds.Y << ", W=" << bounds.Width << ", H=" << bounds.Height << std::endl;
//
//    // 3. Chuẩn bị mảng màu
//    int n = (int)stops.size();
//    std::vector<REAL> pos(n);
//    std::vector<Color> col(n);
//    for (int i = 0; i < n; ++i) {
//        pos[i] = stops[i].getOffset();
//        float opacity = stops[i].getStopOpacity();
//
//        // Cố định Alpha cho ID 'l' để tránh quá nhạt ở phía bên trái
//        int alpha = (int)(opacity * 255);
//        if (grad->getGradientID() == "l" && i == 0) alpha = (std::max)(alpha, 200);
//
//        col[i] = Color(alpha, stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b);
//    }
//
//    // 4. Khởi tạo Brush
//    LinearGradientBrush brush(p1, p2, col.front(), col.back());
//    brush.SetInterpolationColors(col.data(), pos.data(), n);
//
//    // Rất quan trọng để không bị sọc trắng khi vật thể lớn hơn vector p1-p2
//    brush.SetWrapMode(WrapModeTileFlipXY);
//
//    // 5. Xử lý Transform (Nếu có)
//    if (grad->getGradientTransform()) {
//        REAL m[6];
//        grad->getGradientTransform()->GetElements(m);
//        std::cout << "[MATRIX] " << m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3] << ", " << m[4] << ", " << m[5] << std::endl;
//        brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderPrepend);
//    }
//
//    g.SetCompositingMode(CompositingModeSourceOver);
//    Status s = g.FillPath(&brush, &path);
//    std::cout << "[RESULT] Fill Status: " << s << std::endl;
//    std::cout << "==========================================\n" << std::endl;
//}

// Hàm hỗ trợ tạo Radial Gradient Brush
//void SVGRenderer::renderRadialGradientFill(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    using namespace Gdiplus;
//    std::vector<SVGStop> stops = grad->getStopArray();
//    if (stops.empty()) return;
//
//    // 1. Tính toán cx, cy, r
//    float cx, cy, r;
//    if (grad->getGradientUnits() == "userSpaceOnUse") {
//        cx = grad->getCX(); cy = grad->getCY(); r = grad->getR();
//    }
//    else {
//        cx = bounds.X + grad->getCX() * bounds.Width;
//        cy = bounds.Y + grad->getCY() * bounds.Height;
//        r = grad->getR() * (std::max)(bounds.Width, bounds.Height);
//    }
//    if (r <= 0.5f) return; // Bảo vệ bán kính tối thiểu
//
//    // 2. Chuẩn hóa stops (Giữ nguyên logic của bạn nhưng thêm kiểm tra)
//    if (stops.front().getOffset() > 0.0f)
//        stops.insert(stops.begin(), SVGStop(stops.front().getStopColor(), 0.0f, stops.front().getStopOpacity()));
//    if (stops.back().getOffset() < 1.0f)
//        stops.push_back(SVGStop(stops.back().getStopColor(), 1.0f, stops.back().getStopOpacity()));
//
//    // 3. Tạo hình elip bao phủ vùng gradient
//    GraphicsPath ellipsePath;
//    ellipsePath.AddEllipse(cx - r, cy - r, r * 2, r * 2);
//
//    // 4. Khởi tạo Brush
//    PathGradientBrush brush(&ellipsePath);
//
//    // 5. CẤU HÌNH QUAN TRỌNG: Đặt tâm dải màu
//    // Điều này ngăn crash khi cx, cy nằm ngoài ellipsePath
//    brush.SetCenterPoint(PointF(cx, cy));
//
//    int n = (int)stops.size();
//    std::vector<REAL> pos(n);
//    std::vector<Color> col(n);
//
//    // Đảo ngược mảng (SVG: 0=tâm -> GDI+: 1=tâm)
//    for (int i = 0; i < n; ++i) {
//        int idx = n - 1 - i;
//        pos[i] = 1.0f - stops[idx].getOffset();
//        col[i] = Color(BYTE(stops[idx].getStopOpacity() * 255), stops[idx].getStopColor().r, stops[idx].getStopColor().g, stops[idx].getStopColor().b);
//    }
//
//    // ĐẢM BẢO MẢNG POS TĂNG DẦN TUYỆT ĐỐI (Sửa lỗi crash do offset trùng nhau)
//    for (int i = 1; i < n; ++i) {
//        if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;
//    }
//
//    brush.SetInterpolationColors(col.data(), pos.data(), n);
//    brush.SetWrapMode(WrapModeClamp); // SVG pad tương đương Clamp
//
//    if (grad->getGradientTransform()) {
//        brush.SetTransform(grad->getGradientTransform());
//    }
//
//    g.FillPath(&brush, &path);
//}
//void SVGRenderer::renderRadialGradientFill(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    using namespace Gdiplus;
//
//    // Lấy danh sách stops (Bản thân hoặc từ cha thông qua resolveReference)
//    std::vector<SVGStop> stops = grad->getStopArray();
//    if (stops.empty()) {
//        std::cout << "[DEBUG] No stops found for Gradient: " << grad->getGradientID() << std::endl;
//        return;
//    }
//
//    // 1. Tính toán tọa độ và đơn vị (userSpaceOnUse vs objectBoundingBox)
//    float cx, cy, r;
//    std::string units = grad->getGradientUnits();
//
//    if (units == "userSpaceOnUse") {
//        cx = grad->getCX();
//        cy = grad->getCY();
//        r = grad->getR();
//    }
//    else {
//        cx = bounds.X + grad->getCX() * bounds.Width;
//        cy = bounds.Y + grad->getCY() * bounds.Height;
//        r = grad->getR() * (std::max)(bounds.Width, bounds.Height);
//    }
//    if (r <= 0.5f) r = 0.5f;
//
//    // DÒNG DEBUG: Kiểm tra thông số nạp vào GDI+
//    std::cout << "[DEBUG] Rendering Radial ID: " << grad->getGradientID()
//        << " | Units: " << units
//        << " | Center: (" << cx << "," << cy << ")"
//        << " | Radius: " << r << std::endl;
//
//    // 2. Chuẩn bị mảng màu và vị trí (Đảo ngược cho GDI+)
//    int n = (int)stops.size();
//    std::vector<REAL> pos;
//    std::vector<Color> col;
//    pos.reserve(n + 2);
//    col.reserve(n + 2);
//
//    for (int i = n - 1; i >= 0; --i) {
//        pos.push_back(1.0f - stops[i].getOffset());
//        col.push_back(Color(BYTE(stops[i].getStopOpacity() * 255),
//            stops[i].getStopColor().r,
//            stops[i].getStopColor().g,
//            stops[i].getStopColor().b));
//    }
//
//    // 3. CHỐNG CRASH: Đảm bảo dải màu bao phủ 0.0 -> 1.0 và luôn tăng dần
//    if (pos.front() > 0.0f) {
//        pos.insert(pos.begin(), 0.0f);
//        col.insert(col.begin(), col.front());
//    }
//    if (pos.back() < 1.0f) {
//        pos.push_back(1.0f);
//        col.push_back(col.back());
//    }
//    for (size_t i = 1; i < pos.size(); ++i) {
//        if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;
//    }
//
//    // 4. Khởi tạo Brush
//    GraphicsPath ellipsePath;
//    ellipsePath.AddEllipse(cx - r, cy - r, r * 2, r * 2);
//    PathGradientBrush brush(&ellipsePath);
//
//    brush.SetCenterPoint(PointF(cx, cy));
//    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());
//    brush.SetWrapMode(WrapModeClamp);
//
//    // 5. Áp dụng Ma trận Transform (MatrixOrderAppend cho Instagram)
//    if (grad->getGradientTransform()) {
//        if (grad->getGradientTransform()->GetLastStatus() == Ok) {
//            brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderAppend);
//            std::cout << "[DEBUG] Matrix applied for ID: " << grad->getGradientID() << std::endl;
//        }
//        else {
//            std::cout << "[ERROR] Invalid Matrix on ID: " << grad->getGradientID() << std::endl;
//        }
//    }
//
//    // 6. Thực hiện vẽ
//    Status status = g.FillPath(&brush, &path);
//    if (status == Ok) {
//        std::cout << "[DEBUG] FillPath Success: " << grad->getGradientID() << std::endl;
//    }
//    else {
//        std::cout << "[ERROR] GDI+ FillPath Error Code: " << status << std::endl;
//    }
//}

//ban 1 - ban nay co ban oke nhg phan mau vang dang bi cam qua, ngon duoi pha duoi dang chua hong, phan tai cao dang bi sang qua phia ben trai
void SVGRenderer::renderRadialUserSpace(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
    using namespace Gdiplus;
    std::vector<SVGStop> stops = grad->getStopArray();
    if (stops.empty()) return;

    float cx = grad->getCX(), cy = grad->getCY(), r = grad->getR();
    float fx = grad->getFX(), fy = grad->getFY();

    // Kỹ thuật nới rộng để không bị khuyết mảng trắng
    float r_render = r * 1.5f;
    float ratio = r / r_render;

    std::vector<REAL> pos; std::vector<Color> col;
    for (int i = (int)stops.size() - 1; i >= 0; --i) {
        pos.push_back(1.0f - (stops[i].getOffset() * ratio));
        col.push_back(Color(BYTE(stops[i].getStopOpacity() * 255), stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b));
    }
    // Giả lập Pad
    if (pos.front() > 0.0f) { pos.insert(pos.begin(), 0.0f); col.insert(col.begin(), col.front()); }
    for (size_t i = 1; i < pos.size(); ++i) if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;

    GraphicsPath ellipsePath;
    ellipsePath.AddEllipse(cx - r_render, cy - r_render, r_render * 2, r_render * 2);
    PathGradientBrush brush(&ellipsePath);

    brush.SetCenterPoint(PointF(fx, fy));
    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());
    brush.SetWrapMode(WrapModeClamp);

    if (grad->getGradientTransform()) {
        brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderAppend);
    }

    g.FillPath(&brush, &path);
}

//ban nay phan hinh tron mau dung nhung dang to thieu, co the chinh sua , ban nay dung Flipxy nhung khong to duoc het
//void SVGRenderer::renderRadialObjectBoundingBox(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    using namespace Gdiplus;
//    std::vector<SVGStop> stops = grad->getStopArray();
//    if (stops.empty()) return;
//
//    // 1. Tọa độ tương đối (Giống hệt bản Firefox của bạn)
//    float cx = bounds.X + grad->getCX() * bounds.Width;
//    float cy = bounds.Y + grad->getCY() * bounds.Height;
//    float r = grad->getR() * (std::max)(bounds.Width, bounds.Height);
//    float fx = bounds.X + grad->getFX() * bounds.Width;
//    float fy = bounds.Y + grad->getFY() * bounds.Height;
//    if (r <= 0.5f) r = 0.5f;
//
//    // 2. Mảng màu (Đảo ngược)
//    int n = (int)stops.size();
//    std::vector<REAL> pos; std::vector<Color> col;
//    for (int i = n - 1; i >= 0; --i) {
//        float alpha = stops[i].getStopOpacity();
//        if (alpha < 0.01f) alpha = 0.02f; // Giữ sắc tím loang theo bản cũ
//        pos.push_back(1.0f - stops[i].getOffset());
//        col.push_back(Color(BYTE(alpha * 255), stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b));
//    }
//    // Chốt dải màu 0.0 - 1.0
//    if (pos.front() > 0.0f) { pos.insert(pos.begin(), 0.0f); col.insert(col.begin(), col.front()); }
//    if (pos.back() < 1.0f) { pos.push_back(1.0f); col.push_back(col.back()); }
//    for (size_t i = 1; i < pos.size(); ++i) if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;
//
//    // 3. Khởi tạo Brush tại tâm (-r, -r) - COPY CHUẨN LOGIC
//    GraphicsPath brushPath;
//    brushPath.AddEllipse(-r, -r, r * 2, r * 2);
//    PathGradientBrush brush(&brushPath);
//
//    brush.SetCenterPoint(PointF(fx - cx, fy - cy));
//    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());
//    brush.SetWrapMode(WrapModeTileFlipXY); // Giữ TileFlipXY như bản bạn gửi
//
//    // 4. Ma trận kết hợp (Translate + Prepend SVG Transform)
//    Matrix gradMatrix;
//    gradMatrix.Translate(cx, cy);
//    if (grad->getGradientTransform()) {
//        gradMatrix.Multiply(grad->getGradientTransform(), MatrixOrderPrepend);
//    }
//
//    brush.SetTransform(&gradMatrix);
//    g.FillPath(&brush, &path);
//}
//
////ban moi - da dung phan vien tron nhung ch test case h - than cao con cam qua
//void SVGRenderer::renderRadialObjectBoundingBox(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    using namespace Gdiplus;
//    std::vector<SVGStop> stops = grad->getStopArray();
//    if (stops.empty()) return;
//
//    // 1. Tọa độ và Bán kính thực tế của SVG
//    float cx = bounds.X + grad->getCX() * bounds.Width;
//    float cy = bounds.Y + grad->getCY() * bounds.Height;
//    float fx = bounds.X + grad->getFX() * bounds.Width;
//    float fy = bounds.Y + grad->getFY() * bounds.Height;
//
//    // Bán kính chuẩn của SVG (giữ tỷ lệ loang chuẩn)
//    float r_svg = grad->getR() * (std::max)(bounds.Width, bounds.Height);
//
//    // Bán kính cần thiết để bao phủ toàn bộ vật thể (để Pad màu)
//    float d1 = pow(cx - bounds.X, 2) + pow(cy - bounds.Y, 2);
//    float d2 = pow(cx - (bounds.X + bounds.Width), 2) + pow(cy - bounds.Y, 2);
//    float d3 = pow(cx - bounds.X, 2) + pow(cy - (bounds.Y + bounds.Height), 2);
//    float d4 = pow(cx - (bounds.X + bounds.Width), 2) + pow(cy - (bounds.Y + bounds.Height), 2);
//    float r_cover = sqrt((std::max)({ d1, d2, d3, d4 }));
//
//    // r_final phải đủ lớn để bao phủ, nhưng dải màu bên trong phải giữ theo r_svg
//    float r = (std::max)(r_svg, r_cover);
//    float ratio = r_svg / r; // Tỷ lệ để nén dải màu SVG vào trong r_final
//
//    // 2. Mảng màu: Nén dải màu SVG và tạo vùng Pad
//    std::vector<REAL> pos; std::vector<Color> col;
//
//    // Stop cuối cùng (Màu tím hồng biên) sẽ được kéo dài từ r_svg đến r_final
//    // Điều này giả lập hoàn hảo spreadMethod="pad"
//    for (int i = (int)stops.size() - 1; i >= 0; --i) {
//        float offset = stops[i].getOffset();
//        // Nén offset theo tỷ lệ ratio để màu tím hồng xuất hiện đúng vị trí r_svg
//        pos.push_back(1.0f - (offset * ratio));
//        col.push_back(Color(BYTE(stops[i].getStopOpacity() * 255),
//            stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b));
//    }
//
//    // CHỐT PAD: Điểm 0.0 (biên ngoài cùng của r_final) phải là màu của Stop cuối cùng
//    if (pos.front() > 0.0f) {
//        pos.insert(pos.begin(), 0.0f);
//        col.insert(col.begin(), col.at(1)); // Lấy màu tím hồng của biên r_svg phủ ra r_final
//    }
//
//    // Đảm bảo dải màu 0.0 -> 1.0 tăng dần
//    if (pos.back() < 1.0f) { pos.push_back(1.0f); col.push_back(col.back()); }
//    for (size_t i = 1; i < pos.size(); ++i) if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;
//
//    // 3. Khởi tạo Brush bao phủ
//    GraphicsPath brushPath;
//    brushPath.AddEllipse(cx - r, cy - r, r * 2, r * 2);
//    PathGradientBrush brush(&brushPath);
//
//    brush.SetCenterPoint(PointF(fx, fy));
//    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());
//
//    // Dùng Clamp để an toàn, nhưng nhờ dải màu đã được "Pad" thủ công nên sẽ không bị khuyết
//    brush.SetWrapMode(WrapModeClamp);
//
//    if (grad->getGradientTransform()) {
//        brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderPrepend);
//    }
//
//    std::cout << "[PAD DEBUG] SVG Radius: " << r_svg << " | Cover Radius: " << r
//        << " | Color Ratio: " << ratio << std::endl;
//
//    g.FillPath(&brush, &path);
//}

/**
 * Hàm điều chỉnh độ loang (Bias Function)
 * @param x: giá trị offset gốc (0.0 - 1.0)
 * @param bias: < 0.5 để tăng vùng màu biên (Hồng), > 0.5 để tăng vùng màu tâm (Vàng)
 */
//float applyBias(float x, float bias) {
//    if (x <= 0.0f) return 0.0f;
//    if (x >= 1.0f) return 1.0f;
//    // Công thức Schlick's Bias giúp thay đổi độ dốc của dải màu
//    return x / ((1.0f / bias - 2.0f) * (1.0f - x) + 1.0f);
//}


////ban nay oke nhung phan dang le vang dang cam qua va phan vien ngoc dang thieu xanh
//void SVGRenderer::renderRadialObjectBoundingBox(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    using namespace Gdiplus;
//    std::vector<SVGStop> stops = grad->getStopArray();
//    if (stops.empty()) return;
//
//    // 1. Tọa độ (Giữ nguyên logic chuẩn đã xử lý được số âm)
//    float cx = bounds.X + grad->getCX() * bounds.Width;
//    float cy = bounds.Y + grad->getCY() * bounds.Height;
//    float fx = bounds.X + grad->getFX() * bounds.Width;
//    float fy = bounds.Y + grad->getFY() * bounds.Height;
//    float r_svg = grad->getR() * (std::max)(bounds.Width, bounds.Height);
//
//    float d1 = pow(cx - bounds.X, 2) + pow(cy - bounds.Y, 2);
//    float d2 = pow(cx - (bounds.X + bounds.Width), 2) + pow(cy - bounds.Y, 2);
//    float d3 = pow(cx - bounds.X, 2) + pow(cy - (bounds.Y + bounds.Height), 2);
//    float d4 = pow(cx - (bounds.X + bounds.Width), 2) + pow(cy - (bounds.Y + bounds.Height), 2);
//    float r_cover = sqrt((std::max)({ d1, d2, d3, d4 }));
//
//    float r = (std::max)(r_svg, r_cover);
//    float ratio = r_svg / r;
//
//    // 2. Mảng màu: Điều chỉnh Bias để "nhả" bớt màu hồng
//    std::vector<REAL> pos; std::vector<Color> col;
//
//    std::cout << "\n>>>>>> FINE-TUNING BIAS: " << grad->getGradientID() << " <<<<<<" << std::endl;
//
//    for (int i = (int)stops.size() - 1; i >= 0; --i) {
//        float offset = stops[i].getOffset();
//
//        // ĐIỀU CHỈNH TẠI ĐÂY: 
//        // Thay vì 0.35f (quá hồng), ta dùng 0.42f để sắc vàng/cam lan ra rộng hơn.
//        float adjustedOffset = applyBias(offset, 0.4f);
//
//        float gdiPos = 1.0f - (adjustedOffset * ratio);
//        pos.push_back(gdiPos);
//        col.push_back(Color(BYTE(stops[i].getStopOpacity() * 255),
//            stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b));
//    }
//
//    // 3. Logic xử lý PAD
//    if (pos.front() > 0.0f) { pos.insert(pos.begin(), 0.0f); col.insert(col.begin(), col.at(1)); }
//    if (pos.back() < 1.0f) { pos.push_back(1.0f); col.push_back(col.back()); }
//    for (size_t i = 1; i < pos.size(); ++i) if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;
//
//    // 4. Khởi tạo Brush
//    GraphicsPath brushPath;
//    brushPath.AddEllipse(cx - r, cy - r, r * 2, r * 2);
//    PathGradientBrush brush(&brushPath);
//
//    brush.SetCenterPoint(PointF(fx, fy));
//    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());
//
//    // TIẾP TỤC TẮT GammaCorrection để màu vàng không bị xỉn
//    brush.SetGammaCorrection(FALSE);
//    brush.SetWrapMode(WrapModeClamp);
//
//    if (grad->getGradientTransform()) {
//        brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderPrepend);
//    }
//
//    g.FillPath(&brush, &path);
//    std::cout << "[RESULT] Bias 0.42 applied. Yellow areas expanded." << std::endl;
//}

void SVGRenderer::renderRadialObjectBoundingBox(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
    using namespace Gdiplus;
    std::vector<SVGStop> stops = grad->getStopArray();
    if (stops.empty()) return;

    // 1. Tọa độ và Bán kính chuẩn
    float cx = bounds.X + grad->getCX() * bounds.Width;
    float cy = bounds.Y + grad->getCY() * bounds.Height;
    float fx = bounds.X + grad->getFX() * bounds.Width;
    float fy = bounds.Y + grad->getFY() * bounds.Height;
    float r_svg = grad->getR() * (std::max)(bounds.Width, bounds.Height);

    float d1 = pow(cx - bounds.X, 2) + pow(cy - bounds.Y, 2);
    float d2 = pow(cx - (bounds.X + bounds.Width), 2) + pow(cy - bounds.Y, 2);
    float d3 = pow(cx - bounds.X, 2) + pow(cy - (bounds.Y + bounds.Height), 2);
    float d4 = pow(cx - (bounds.X + bounds.Width), 2) + pow(cy - (bounds.Y + bounds.Height), 2);
    float r_cover = sqrt((std::max)({ d1, d2, d3, d4 }));

    float r = (std::max)(r_svg, r_cover);
    float ratio = r_svg / r;

    // 2. Mảng màu: LOGIC MỞ RỘNG VÙNG TRẮNG
    std::vector<REAL> pos; std::vector<Color> col;

    for (int i = (int)stops.size() - 1; i >= 0; --i) {
        float offset = stops[i].getOffset();
        Color currentStopCol(BYTE(stops[i].getStopOpacity() * 255), stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b);

        float lum = getLuminance(currentStopCol);
        float currentBias = 0.4f; // Mặc định

        // NẾU LÀ MÀU TRẮNG/SÁNG (như vùng tâm viên ngọc):
        if (lum > 230) {
            // Dùng bias thấp (0.2 - 0.25) để ép màu trắng loang rộng ra biên
            currentBias = 0.22f;
        }
        else if (lum > 180) {
            currentBias = 0.32f;
        }

        float adjustedOffset = applyBias(offset, currentBias);
        pos.push_back(1.0f - (adjustedOffset * ratio));
        col.push_back(currentStopCol);
    }

    // 3. Logic Pad (Giữ nguyên để chuẩn viên ngọc)
    if (pos.front() > 0.0f) { pos.insert(pos.begin(), 0.0f); col.insert(col.begin(), col.at(1)); }
    if (pos.back() < 1.0f) { pos.push_back(1.0f); col.push_back(col.back()); }
    for (size_t i = 1; i < pos.size(); ++i) if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;

    // 4. Brush
    GraphicsPath brushPath;
    brushPath.AddEllipse(cx - r, cy - r, r * 2, r * 2);
    PathGradientBrush brush(&brushPath);
    brush.SetCenterPoint(PointF(fx, fy));
    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());
    brush.SetGammaCorrection(FALSE);
    brush.SetWrapMode(WrapModeClamp);

    if (grad->getGradientTransform()) brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderPrepend);

    g.FillPath(&brush, &path);
}

////ban nay gan dung nhung nguoc mau co the thu lai
//void SVGRenderer::renderRadialObjectBoundingBox(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    using namespace Gdiplus;
//    std::vector<SVGStop> stops = grad->getStopArray();
//
//    std::cout << "\n========== DEBUG RADIAL COVER: " << grad->getGradientID() << " ==========" << std::endl;
//    if (stops.empty()) return;
//
//    // 1. Tính tâm chính xác
//    float cx = bounds.X + grad->getCX() * bounds.Width;
//    float cy = bounds.Y + grad->getCY() * bounds.Height;
//    float fx = bounds.X + grad->getFX() * bounds.Width;
//    float fy = bounds.Y + grad->getFY() * bounds.Height;
//
//    // 2. Kỹ thuật COVER: Tính bán kính đến góc xa nhất của vật thể
//    float dx1 = pow(cx - bounds.X, 2);
//    float dx2 = pow(cx - (bounds.X + bounds.Width), 2);
//    float dy1 = pow(cy - bounds.Y, 2);
//    float dy2 = pow(cy - (bounds.Y + bounds.Height), 2);
//
//    float r_needed = sqrt((std::max)(dx1, dx2) + (std::max)(dy1, dy2));
//    float diagonalSVG = grad->getR() * sqrt(pow(bounds.Width, 2) + pow(bounds.Height, 2)) / sqrt(2.0f);
//    float r = (std::max)(r_needed, diagonalSVG);
//
//    std::cout << "[GEOM] Center: (" << cx << "," << cy << ") | Focus: (" << fx << "," << fy << ")" << std::endl;
//    std::cout << "[GEOM] r_needed (to cover corners): " << r_needed << std::endl;
//    std::cout << "[GEOM] diagonalSVG: " << diagonalSVG << " | Final r: " << r << std::endl;
//
//    // 3. Mảng màu
//    std::vector<REAL> pos; std::vector<Color> col;
//    for (int i = 0; i < (int)stops.size(); ++i) {
//        pos.push_back(stops[i].getOffset());
//        col.push_back(Color(static_cast<BYTE>(stops[i].getStopOpacity() * 255),
//            stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b));
//    }
//
//    if (pos.front() > 0.0f) {
//        pos.insert(pos.begin(), 0.0f);
//        col.insert(col.begin(), col.at(1));
//    }
//    if (pos.back() < 1.0f) {
//        pos.push_back(1.0f);
//        col.push_back(col.back());
//    }
//
//    std::cout << "[COLORS] Color Interpolation Stops:" << std::endl;
//    for (size_t i = 1; i < pos.size(); ++i) {
//        if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;
//        std::cout << "  - Stop " << i << ": pos=" << pos[i] << " Alpha=" << (int)col[i].GetA() << std::endl;
//    }
//
//    // 4. Khởi tạo Brush
//    GraphicsPath brushPath;
//    brushPath.AddEllipse(cx - r, cy - r, r * 2, r * 2);
//    PathGradientBrush brush(&brushPath);
//
//    brush.SetCenterPoint(PointF(fx, fy));
//    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());
//
//    brush.SetGammaCorrection(TRUE);
//    brush.SetFocusScales(0.05f, 0.05f);
//    brush.SetWrapMode(WrapModeClamp);
//
//    if (grad->getGradientTransform()) {
//        REAL m[6];
//        grad->getGradientTransform()->GetElements(m);
//        std::cout << "[MATRIX] Applying Transform: [" << m[0] << "," << m[1] << "," << m[2] << "," << m[3] << "," << m[4] << "," << m[5] << "]" << std::endl;
//        brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderPrepend);
//    }
//
//    g.SetCompositingMode(CompositingModeSourceOver);
//    Status s = g.FillPath(&brush, &path);
//    std::cout << "[RESULT] Fill status: " << s << " (0 is OK)" << std::endl;
//    std::cout << "==========================================\n" << std::endl;
//}

////gan dung nhưng phan tren chua tim lam - co le ty le scale bi sai
//void SVGRenderer::renderRadialObjectBoundingBox(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    using namespace Gdiplus;
//    std::vector<SVGStop> stops = grad->getStopArray();
//
//    std::cout << "\n========== DEEP COVER DEBUG: " << grad->getGradientID() << " ==========" << std::endl;
//    if (stops.empty()) return;
//
//    // 1. Tính tâm chính xác
//    float cx = bounds.X + grad->getCX() * bounds.Width;
//    float cy = bounds.Y + grad->getCY() * bounds.Height;
//    float fx = bounds.X + grad->getFX() * bounds.Width;
//    float fy = bounds.Y + grad->getFY() * bounds.Height;
//
//    // 2. Kỹ thuật COVER: Tính toán khoảng cách tới 4 góc
//    float distSq1 = pow(cx - bounds.X, 2) + pow(cy - bounds.Y, 2); // Top-Left
//    float distSq2 = pow(cx - (bounds.X + bounds.Width), 2) + pow(cy - bounds.Y, 2); // Top-Right
//    float distSq3 = pow(cx - bounds.X, 2) + pow(cy - (bounds.Y + bounds.Height), 2); // Bottom-Left
//    float distSq4 = pow(cx - (bounds.X + bounds.Width), 2) + pow(cy - (bounds.Y + bounds.Height), 2); // Bottom-Right
//
//    float r_needed = sqrt((std::max)({ distSq1, distSq2, distSq3, distSq4 }));
//    float diagonalSVG = grad->getR() * sqrt(pow(bounds.Width, 2) + pow(bounds.Height, 2)) / sqrt(2.0f);
//    float r = (std::max)(r_needed, diagonalSVG);
//
//    std::cout << "[GEOM] Center: (" << cx << "," << cy << ")" << std::endl;
//    std::cout << "[GEOM] Bounding Box: Y_min=" << bounds.Y << " to Y_max=" << (bounds.Y + bounds.Height) << std::endl;
//    std::cout << "[GEOM] Radius Needed (to avoid clipping): " << r_needed << " | SVG Radius: " << diagonalSVG << std::endl;
//    std::cout << "[GEOM] Final Radius used: " << r << std::endl;
//
//    // 3. Mảng màu: Đảo ngược chuẩn
//    int n = (int)stops.size();
//    std::vector<REAL> pos; std::vector<Color> col;
//    for (int i = 0; i < n; ++i) {
//        pos.push_back(1.0f - stops[i].getOffset());
//        col.push_back(Color(static_cast<BYTE>(stops[i].getStopOpacity() * 255),
//            stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b));
//    }
//    std::reverse(pos.begin(), pos.end());
//    std::reverse(col.begin(), col.end());
//
//    if (pos.front() > 0.0f) {
//        pos.insert(pos.begin(), 0.0f);
//        col.insert(col.begin(), col.at(1));
//    }
//    if (pos.back() < 1.0f) {
//        pos.push_back(1.0f);
//        col.push_back(col.back());
//    }
//
//    std::cout << "[COLORS] Final Gradient Map (0.0=Edge, 1.0=Center):" << std::endl;
//    for (size_t i = 0; i < pos.size(); ++i) {
//        if (i > 0 && pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;
//        std::cout << "  - Stop " << i << ": pos=" << pos[i] << " RGB(" << (int)col[i].GetR() << "," << (int)col[i].GetG() << "," << (int)col[i].GetB() << ")" << std::endl;
//    }
//
//    // 4. Khởi tạo Brush
//    GraphicsPath brushPath;
//    brushPath.AddEllipse(cx - r, cy - r, r * 2, r * 2);
//    PathGradientBrush brush(&brushPath);
//
//    brush.SetCenterPoint(PointF(fx, fy));
//    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());
//
//    brush.SetGammaCorrection(TRUE);
//    brush.SetFocusScales(0.05f, 0.05f);
//    brush.SetWrapMode(WrapModeClamp);
//
//    if (grad->getGradientTransform()) {
//        brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderPrepend);
//    }
//
//    g.SetCompositingMode(CompositingModeSourceOver);
//    Status s = g.FillPath(&brush, &path);
//    std::cout << "[RESULT] Fill Status: " << s << std::endl;
//    std::cout << "==========================================\n" << std::endl;
//}




////ban ve tuong doi firefox
//void SVGRenderer::renderRadialGradientFill(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    using namespace Gdiplus;
//    std::vector<SVGStop> stops = grad->getStopArray();
//
//    std::cout << "\n========== START RADIAL DEBUG: " << grad->getGradientID() << " ==========" << std::endl;
//    if (stops.empty()) {
//        std::cout << "[ERROR] No stops found. Check xlink:href resolution!" << std::endl;
//        return;
//    }
//
//    // 1. Tọa độ (Xử lý userSpaceOnUse)
//    float cx, cy, r, fx, fy;
//    std::string units = grad->getGradientUnits();
//
//    if (units == "userSpaceOnUse") {
//        cx = grad->getCX(); cy = grad->getCY(); r = grad->getR();
//        fx = grad->getFX(); fy = grad->getFY();
//    }
//    else {
//        cx = bounds.X + grad->getCX() * bounds.Width;
//        cy = bounds.Y + grad->getCY() * bounds.Height;
//        r = grad->getR() * (std::max)(bounds.Width, bounds.Height);
//        fx = bounds.X + grad->getFX() * bounds.Width;
//        fy = bounds.Y + grad->getFY() * bounds.Height;
//    }
//    if (r <= 0.5f) r = 0.5f;
//
//    std::cout << "[GEOMETRY] Center: (" << cx << ", " << cy << ")" << std::endl;
//    std::cout << "[GEOMETRY] Focus:  (" << fx << ", " << fy << ")" << std::endl;
//    std::cout << "[GEOMETRY] Radius: " << r << std::endl;
//
//    // 2. Mảng màu (Đảo ngược)
//    int n = (int)stops.size();
//    std::vector<REAL> pos; std::vector<Color> col;
//    for (int i = n - 1; i >= 0; --i) {
//        float alpha = stops[i].getStopOpacity();
//        if (alpha < 0.01f) alpha = 0.02f; // Giữ sắc tím loang
//        pos.push_back(1.0f - stops[i].getOffset());
//        col.push_back(Color(BYTE(alpha * 255), stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b));
//    }
//    if (pos.front() > 0.0f) { pos.insert(pos.begin(), 0.0f); col.insert(col.begin(), col.front()); }
//    if (pos.back() < 1.0f) { pos.push_back(1.0f); col.push_back(col.back()); }
//    for (size_t i = 1; i < pos.size(); ++i) if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;
//
//    // 3. Khởi tạo Brush tại tâm 0,0
//    GraphicsPath brushPath;
//    brushPath.AddEllipse(-r, -r, r * 2, r * 2);
//    PathGradientBrush brush(&brushPath);
//
//    // Focus point tương đối
//    brush.SetCenterPoint(PointF(fx - cx, fy - cy));
//    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());
//    brush.SetWrapMode(WrapModeTileFlipXY);
//
//    // 4. Ma trận kết hợp
//    Matrix gradMatrix;
//    gradMatrix.Translate(cx, cy);
//
//    const Matrix* svgTransform = grad->getGradientTransform();
//    if (svgTransform) {
//        REAL elements[6];
//        svgTransform->GetElements(elements);
//        std::cout << "[MATRIX] SVG Transform: matrix(" << elements[0] << ", " << elements[1] << ", "
//            << elements[2] << ", " << elements[3] << ", " << elements[4] << ", " << elements[5] << ")" << std::endl;
//
//        gradMatrix.Multiply(svgTransform, MatrixOrderPrepend);
//    }
//
//    brush.SetTransform(&gradMatrix);
//
//    // 5. Thực hiện vẽ
//    Status s = g.FillPath(&brush, &path);
//    std::cout << "[RESULT] Fill status: " << s << " (0 is OK)" << std::endl;
//    std::cout << "========== END RADIAL DEBUG ==========\n" << std::endl;
//}

// ban ve duoc insta
//void SVGRenderer::renderRadialGradientFill(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
//    using namespace Gdiplus;
//    std::vector<SVGStop> stops = grad->getStopArray();
//
//    // Debug tối giản để theo dõi
//    std::cout << "Rendering Gradient ID: " << grad->getGradientID() << std::endl;
//    if (stops.empty()) return;
//
//    // 1. Lấy tọa độ gốc (Giữ nguyên logic bạn đang dùng)
//    float cx, cy, r, fx, fy;
//    if (grad->getGradientUnits() == "userSpaceOnUse") {
//        cx = grad->getCX(); cy = grad->getCY(); r = grad->getR();
//        fx = grad->getFX(); fy = grad->getFY();
//    }
//    else {
//        cx = bounds.X + grad->getCX() * bounds.Width;
//        cy = bounds.Y + grad->getCY() * bounds.Height;
//        r = grad->getR() * (std::max)(bounds.Width, bounds.Height);
//        fx = bounds.X + grad->getFX() * bounds.Width;
//        fy = bounds.Y + grad->getFY() * bounds.Height;
//    }
//    if (r <= 0.5f) r = 0.5f;
//
//    // 2. Chuẩn bị dải màu (Đảo ngược)
//    int n = (int)stops.size();
//    std::vector<REAL> pos; std::vector<Color> col;
//    for (int i = n - 1; i >= 0; --i) {
//        float alpha = stops[i].getStopOpacity();
//        if (alpha < 0.01f) alpha = 0.01f; // Giữ sắc độ
//        pos.push_back(1.0f - stops[i].getOffset());
//        col.push_back(Color(BYTE(alpha * 255), stops[i].getStopColor().r, stops[i].getStopColor().g, stops[i].getStopColor().b));
//    }
//    // Đảm bảo mảng pos luôn hợp lệ
//    for (size_t i = 1; i < pos.size(); ++i) if (pos[i] <= pos[i - 1]) pos[i] = pos[i - 1] + 0.001f;
//
//    // 3. Kỹ thuật "Nới lỏng" bán kính (An toàn cho mọi testcase)
//    // Thay vì dùng r=65, ta dùng r_render lớn hơn một chút để chống khuyết mảng
//    float r_render = r * 1.5f;
//    float ratio = r / r_render;
//
//    GraphicsPath ellipsePath;
//    ellipsePath.AddEllipse(cx - r_render, cy - r_render, r_render * 2, r_render * 2);
//    PathGradientBrush brush(&ellipsePath);
//
//    // Nén dải màu theo tỷ lệ 1.5x
//    for (auto& p : pos) {
//        p = 1.0f - ((1.0f - p) * ratio);
//    }
//
//    // Giả lập "Pad": Thêm stop cuối cùng kéo dài ra biên r_render
//    if (pos.front() > 0.0f) {
//        pos.insert(pos.begin(), 0.0f);
//        col.insert(col.begin(), col.front());
//    }
//
//    brush.SetCenterPoint(PointF(fx, fy));
//    brush.SetInterpolationColors(col.data(), pos.data(), (int)pos.size());
//    brush.SetWrapMode(WrapModeClamp);
//
//    // 4. Áp dụng Ma trận (Giữ nguyên MatrixOrderAppend như bạn thấy ổn)
//    if (grad->getGradientTransform()) {
//        brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderAppend);
//    }
//
//    // 5. Vẽ và kiểm tra trạng thái
//    Status status = g.FillPath(&brush, &path);
//    if (status != Ok) {
//        std::cout << "[ERROR] GDI+ FillPath failed with code: " << status << std::endl;
//    }
//}

void SVGRenderer::renderRadialGradientFill(Gdiplus::Graphics& g, const SVGRadialGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds) const {
    std::string units = grad->getGradientUnits();

    if (units == "userSpaceOnUse") {
        // Gọi logic dành cho Instagram/Tọa độ tuyệt đối
        renderRadialUserSpace(g, grad, path, bounds);
    }
    else {
        // Gọi logic dành cho Firefox/Tọa độ tương đối
        renderRadialObjectBoundingBox(g, grad, path, bounds);
    }
}

// Hàm tổng quát renderGradientFill mới
void SVGRenderer::renderGradientFill(Gdiplus::Graphics& g, const SVGGradient* grad,
    const Gdiplus::GraphicsPath& path, // Thêm tham số này
    const Gdiplus::RectF& bounds,
    const SVGDocumentContext& context) {
    if (!grad) return;

    grad->resolveReference(context);

    if (auto* linear = dynamic_cast<const SVGLinearGradient*>(grad)) {
        // Truyền thêm tham số path vào đây
        renderLinearGradientFill(g, linear, path, bounds);
    }
    else if (auto* radial = dynamic_cast<const SVGRadialGradient*>(grad)) {
        // Truyền thêm tham số path vào đây
        renderRadialGradientFill(g, radial, path, bounds);
    }
}


