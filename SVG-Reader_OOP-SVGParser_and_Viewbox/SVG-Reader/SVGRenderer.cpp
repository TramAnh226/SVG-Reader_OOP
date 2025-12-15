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
            renderGradientFill(g, grad, bounds, context);
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
            renderGradientFill(g, grad, bounds, context);
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
            renderGradientFill(g, grad, bounds, context);
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
        wFontFamilyName = L"Arial";
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
void SVGRenderer::renderGradientPathFill(Gdiplus::Graphics& g, const SVGGradient* grad, const Gdiplus::GraphicsPath& path, const Gdiplus::RectF& bounds, const SVGDocumentContext& context) const {
    using namespace Gdiplus;
    if (!grad) return;

    const SVGGradient* resolvedGrad = grad;
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

    // Lấy commands từ đối tượng Path
    const std::vector<PathCommand>& commands = pathElement->getCommands(); // Giả sử bạn có hàm getter này

    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
    Gdiplus::Matrix oldMatrix;
    g.GetTransform(&oldMatrix);
    const Gdiplus::Matrix* matrix = pathElement->getTransformMatrix(); // Thừa kế từ SVGElement
    if (matrix) {
        g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    }

    // 2. XÂY DỰNG GRAPHICS PATH TỪ CÁC COMMANDS
    Gdiplus::GraphicsPath path;
    Gdiplus::PointF currentPoint(0.0f, 0.0f); // Vị trí điểm hiện tại
    Gdiplus::PointF startPoint(0.0f, 0.0f);  // Vị trí điểm đầu tiên của subpath

    // NOTE: Cú pháp parse của bạn (M, L, C, S...) đã chuyển đổi tất cả sang tọa độ tuyệt đối
    // (Relative coordinates đã được tính toán thành Absolute trong hàm parse).

    for (const auto& cmd : commands) {
        // Kiểm tra số lượng tham số an toàn (Dù logic parse đã kiểm tra)
        if (cmd.params.empty() && cmd.type != PathCommandType::ClosePath) continue;

        switch (cmd.type) {
        case PathCommandType::MoveTo: {
            currentPoint = Gdiplus::PointF(cmd.params[0], cmd.params[1]);
            startPoint = currentPoint;
            path.StartFigure(); // Bắt đầu một hình dạng mới
            break;
        }
        case PathCommandType::LineTo: {
            Gdiplus::PointF p(cmd.params[0], cmd.params[1]);
            path.AddLine(currentPoint, p);
            currentPoint = p;
            break;
        }
        case PathCommandType::HLineTo: {
            Gdiplus::PointF p(cmd.params[0], currentPoint.Y); // Y giữ nguyên
            path.AddLine(currentPoint, p);
            currentPoint = p;
            break;
        }
        case PathCommandType::VLineTo: {
            Gdiplus::PointF p(currentPoint.X, cmd.params[0]); // X giữ nguyên
            path.AddLine(currentPoint, p);
            currentPoint = p;
            break;
        }
        case PathCommandType::CubicBezier: {
            // Cần 3 cặp tọa độ (p1, p2, p3). p0 là currentPoint.
            Gdiplus::PointF p1(cmd.params[0], cmd.params[1]);
            Gdiplus::PointF p2(cmd.params[2], cmd.params[3]);
            Gdiplus::PointF p3(cmd.params[4], cmd.params[5]);
            path.AddBezier(currentPoint, p1, p2, p3);
            currentPoint = p3;
            break;
        }
        case PathCommandType::SmoothCubicBezier: {
            // Lệnh S đã tính toán p1, y1 (điểm điều khiển đối xứng)
            // và lưu chúng vào pc.params[0], [1].
            Gdiplus::PointF p1(cmd.params[0], cmd.params[1]); // Điểm điều khiển đối xứng (đã tính trong parse)
            Gdiplus::PointF p2(cmd.params[2], cmd.params[3]);
            Gdiplus::PointF p3(cmd.params[4], cmd.params[5]);
            path.AddBezier(currentPoint, p1, p2, p3);
            currentPoint = p3;
            break;
        }
        case PathCommandType::ClosePath: {
            path.CloseFigure();
            currentPoint = startPoint;
            break;
        }
        default:
            // Bỏ qua các lệnh chưa hỗ trợ (Q, A...)
            break;
        }
    }

    FillRule svgFillRule = pathElement->getSVGStyle().getFillRule();

    // Chuyển FillRule của SVG sang FillMode của GDI+
    if (svgFillRule == FillRule::EvenOdd) {
        path.SetFillMode(Gdiplus::FillModeAlternate);
    }
    else {
        // Mặc định là NonZero (Winding)
        path.SetFillMode(Gdiplus::FillModeWinding);
    }

    // 3. XỬ LÝ FILL VÀ STROKE

    // Lấy Bounds để sử dụng cho Gradient/Brush
    Gdiplus::RectF bounds;
    path.GetBounds(&bounds);

    // 3A. FILL (Gradient HOẶC Màu đơn)
    if (pathElement->getSVGStyle().hasGradientFill()) {
        const SVGGradient* grad = pathElement->getSVGStyle().getGrad();
        if (grad) {
            // Sử dụng hàm hỗ trợ Fill Path
            renderGradientPathFill(g, grad, path, bounds, context);
        }
    }
    else {
        // TRƯỜNG HỢP MÀU ĐƠN
        Gdiplus::Color fillColor = pathElement->getSVGStyle().getGdiFillColor();
        if (fillColor.GetA() > 0) {
            Gdiplus::SolidBrush brush(fillColor);
            g.FillPath(&brush, &path);
        }
    }

    // 3B. STROKE
    Stroke* strokeObj = pathElement->getSVGStyle().getStroke();
    if (strokeObj && strokeObj->strokeWidth > 0.0f) {
        // Giả định getGdiColor() trả về màu đã tính opacity
        Gdiplus::Color finalStrokeColor = strokeObj->getGdiColor();
        if (finalStrokeColor.GetA() > 0) {
            Gdiplus::Pen pen(finalStrokeColor, strokeObj->strokeWidth);

            pen.SetMiterLimit(strokeObj->getStrokeMiterLimit());
            // Có thể thêm logic cho MiterLimit/LineCap/LineJoin nếu cần
            g.DrawPath(&pen, &path);
        }
    }

    // 4. KHÔI PHỤC TRANSFORM
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
    Gdiplus::PointF p1(bounds.X + grad->getX1() * bounds.Width,
        bounds.Y + grad->getY1() * bounds.Height);

    Gdiplus::PointF p2(bounds.X + grad->getX2() * bounds.Width,
        bounds.Y + grad->getY2() * bounds.Height);

    // 2. Lấy màu đầu và màu cuối (KHÔNG DÙNG ColorBlend)

    // Lấy màu đầu
    Gdiplus::Color c1 = stops.front().getStopColor();

    // Lấy màu cuối
    Gdiplus::Color c2 = (stops.size() > 1)
        ? stops.back().getStopColor()
        : c1;

    // 3. Tạo LinearGradientBrush
    Gdiplus::LinearGradientBrush brush(p1, p2, c1, c2);

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
void SVGRenderer::renderGradientFill(Gdiplus::Graphics& g, const SVGGradient* grad, const Gdiplus::RectF& bounds, const SVGDocumentContext& context) {
    if (!grad) return;

    const SVGGradient* resolvedGrad = grad;
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


