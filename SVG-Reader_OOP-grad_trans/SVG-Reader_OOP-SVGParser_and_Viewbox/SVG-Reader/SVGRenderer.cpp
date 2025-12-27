#ifndef NOMINMAX
#define NOMINMAX
#endif
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
#include "GradientUtils.h"
#include "ColorUtils.h"
#include "SVGLinearGradient.h" // Cần cho dynamic_cast
#include "SVGRadialGradient.h"// Cần cho dynamic_cast
#include "SVGGroup.h"
#include "SVGRenderer.h"


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





void SVGRenderer::renderRectangle(Gdiplus::Graphics& g, const SVGRectangle* rect, const SVGDocumentContext& context) {
    if (!rect) return;
    // 0. ÁP DỤNG MATRIX LÊN CONTEXT
    
    //// lưu matrix hiện tại của Graphics Context
    //Gdiplus::Matrix oldMatrix;
    //g.GetTransform(&oldMatrix);
    //// áp dụng ma trận biến đổi của đối tượng
    //const Gdiplus::Matrix* matrix = rect->getTransformMatrix();
    //if (matrix) {
    //    // nhân ma trận của rect và của context
    //    g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    //}
    
    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; rect->getSVGStyle().getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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

void SVGRenderer::renderCircle(Gdiplus::Graphics& g, const SVGCircle* circle, const SVGDocumentContext& context) {
    if (!circle) return;
    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
    
    //// lưu ma trận hiện tại
    //Gdiplus::Matrix oldMatrix;
    //g.GetTransform(&oldMatrix);
    //// áp dụng ma trận biến đội
    //const Gdiplus::Matrix* matrix = circle->getTransformMatrix();
    //if (matrix) {
    //    g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    //}

    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; circle->getSVGStyle().getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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

void SVGRenderer::renderEllipse(Gdiplus::Graphics& g, const SVGEllipse* ellipse, const SVGDocumentContext& context) {
    if (!ellipse) return;
    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
    
    //// lưu ma trận hiện tại
    //Gdiplus::Matrix oldMatrix;
    //g.GetTransform(&oldMatrix);
    //// áp dụng ma trận biến đội
    //const Gdiplus::Matrix* matrix = ellipse->getTransformMatrix();
    //if (matrix) {
    //    g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    //}

    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; ellipse->getSVGStyle().getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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

void SVGRenderer::renderLine(Gdiplus::Graphics& g, const SVGLine* line, const SVGDocumentContext& context) {
    if (!line) return;
    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
    
    //// lưu ma trận hiện tại
    //Gdiplus::Matrix oldMatrix;
    //g.GetTransform(&oldMatrix);
    //// áp dụng ma trận biến đội
    //const Gdiplus::Matrix* matrix = line->getTransformMatrix();
    //if (matrix) {
    //    g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    //}

    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; line->getSVGStyle().getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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

void SVGRenderer::renderPolygon(Gdiplus::Graphics& g, const SVGPolygon* polygon, const SVGDocumentContext& context) {
    if (!polygon) return;
    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
    
    //// lưu ma trận hiện tại
    //Gdiplus::Matrix oldMatrix;
    //g.GetTransform(&oldMatrix);
    //// áp dụng ma trận biến đội
    //const Gdiplus::Matrix* matrix = polygon->getTransformMatrix();
    //if (matrix) {
    //    g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    //}

    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; polygon->getSVGStyle().getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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



void SVGRenderer::renderPolyline(Gdiplus::Graphics& g, const SVGPolyline* polyline, const SVGDocumentContext& context) {
    if (!polyline) return;

    // 1. ÁP DỤNG MA TRẬN
    
    //Gdiplus::Matrix oldMatrix;
    //g.GetTransform(&oldMatrix);
    //const Gdiplus::Matrix* matrix = polyline->getTransformMatrix();
    //if (matrix) {
    //    g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    //}

    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; polyline->getSVGStyle().getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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

void SVGRenderer::renderText(Gdiplus::Graphics& g, const SVGText* text, const SVGDocumentContext& context) {
    if (!text) return;

    // 1. ÁP DỤNG MA TRẬN
    
    //Gdiplus::Matrix oldMatrix;
    //g.GetTransform(&oldMatrix);
    //const Gdiplus::Matrix* matrix = text->getTransformMatrix();
    //if (matrix) {
    //    g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    //}

    Matrix oldMatrix; g.GetTransform(&oldMatrix);

    Matrix localMatrix;
    text->getSVGStyle().getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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

void SVGRenderer::renderSquare(Gdiplus::Graphics& g, const SVGSquare* square, const SVGDocumentContext& context) {
    if (!square) return;
    // 1. ÁP DỤNG MA TRẬN LÊN CONTEXT
    
    //// lưu ma trận hiện tại
    //Gdiplus::Matrix oldMatrix;
    //g.GetTransform(&oldMatrix);
    //// áp dụng ma trận biến đội
    //const Gdiplus::Matrix* matrix = square->getTransformMatrix();
    //if (matrix) {
    //    g.MultiplyTransform(matrix, Gdiplus::MatrixOrderPrepend);
    //}

    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; square->getSVGStyle().getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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

void SVGRenderer::renderGroup(Gdiplus::Graphics& g, const SVGGroup* rootGroup, const SVGDocumentContext& context) {
    if (!rootGroup) return;

    // 1. LƯU MA TRẬN HIỆN TẠI (Tích lũy từ các Group cha)
    
    //Gdiplus::Matrix oldMatrix;
    //g.GetTransform(&oldMatrix);


    //// 2. ÁP DỤNG MA TRẬN RIÊNG CỦA NHÓM NÀY
    //const Gdiplus::Matrix* groupMatrix = rootGroup->getTransformMatrix();

    //if (groupMatrix) {
    //    // Gdiplus::Graphics::MultiplyTransform() chỉ đọc ma trận được truyền vào.
    //    // Không cần gọi Clone() hay delete.
    //    g.MultiplyTransform(groupMatrix, Gdiplus::MatrixOrderPrepend);
    //}

    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix groupMatrix; rootGroup->getSVGStyle().getTransform().applyToMatrix(groupMatrix);
    g.MultiplyTransform(&groupMatrix);

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


void SVGRenderer::renderPath(Gdiplus::Graphics& g, const SVGPath* pathElement, const SVGDocumentContext& context) const {
    if (!pathElement) return;

    // 1. Chống răng cưa mức độ cao
    g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    g.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);

    // 2. Xử lý ma trận biến đổi (Sửa Prepend thành Append)
    
    //Gdiplus::Matrix oldMatrix;
    //g.GetTransform(&oldMatrix);
    //const Gdiplus::Matrix* pathMatrix = pathElement->getTransformMatrix();
    //if (pathMatrix) {
    //    // Sử dụng Append để đảm bảo ma trận của Path được áp dụng SAU ma trận Group
    //    g.MultiplyTransform(pathMatrix, Gdiplus::MatrixOrderAppend);
    //}

    Matrix oldMatrix; g.GetTransform(&oldMatrix);
    Matrix localMatrix; pathElement->getSVGStyle().getTransform().applyToMatrix(localMatrix);
    g.MultiplyTransform(&localMatrix);

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

    //if (grad->getGradientTransform()) {
    //    brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderPrepend);
    //}

    // Lấy đối tượng transform (giá trị thực, không phải con trỏ)
    const SVGTransform& transform = grad->getGradientTransform();

    // Thay thế logic cũ: if (grad->getGradientTransform())
    if (!transform.isEmpty()) {
        // Tạo một ma trận tạm thời của GDI+
        Gdiplus::Matrix gdiMatrix;

        // Áp dụng các phép toán (Translate, Rotate, Scale) vào ma trận này
        transform.applyToMatrix(gdiMatrix);

        // Nhân vào brush
        brush.MultiplyTransform(&gdiMatrix, Gdiplus::MatrixOrderPrepend);
        //brush.MultiplyTransform(&gdiMatrix);
        //brush.SetTransform(&gdiMatrix);
    }


    g.FillPath(&brush, &path);
}



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

    //if (grad->getGradientTransform()) {
    //    brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderAppend);
    //}

    // Lấy đối tượng transform (giá trị thực, không phải con trỏ)
    const SVGTransform& transform = grad->getGradientTransform();

    // Thay thế logic cũ: if (grad->getGradientTransform())
    if (!transform.isEmpty()) {
        // Tạo một ma trận tạm thời của GDI+
        Gdiplus::Matrix gdiMatrix;

        // Áp dụng các phép toán (Translate, Rotate, Scale) vào ma trận này
        transform.applyToMatrix(gdiMatrix);

        // Nhân vào brush
        //brush.MultiplyTransform(&gdiMatrix);
        brush.MultiplyTransform(&gdiMatrix, Gdiplus::MatrixOrderAppend);
        //brush.SetTransform(&gdiMatrix);
    }

    g.FillPath(&brush, &path);
}
bool isBlueHeavy(const Gdiplus::Color& c) {
    return c.GetB() > c.GetR() + 25 &&
        c.GetB() > c.GetG() + 20;
}

bool isPurpleZone(const Gdiplus::Color& c) {
    return c.GetR() > 120 && c.GetB() > 120;
}

Gdiplus::Color normalizeRadialColor(const Gdiplus::Color& src, float offset) {
    int r = src.GetR();
    int g = src.GetG();
    int b = src.GetB();
    BYTE a = src.GetA();

    // Chỉ can thiệp vùng ngoài tâm
    if (offset > 0.35f) {

        // Giảm xanh gắt (Instagram, Firefox glow...)
        if (isBlueHeavy(src)) {
            r = (std::min)(255, r + int(32 * offset));
            b = (std::max)(0, b - int(36 * offset));
        }

        // Giữ tím không chuyển xanh
        if (isPurpleZone(src)) {
            r = (std::min)(255, r + int(14 * offset));
            g = (std::min)(255, g + int(6 * offset));
        }
    }

    return Gdiplus::Color(a, r, g, b);
}

Gdiplus::Color stabilizeHue(const Gdiplus::Color& src, float offset) {
    int r = src.GetR();
    int g = src.GetG();
    int b = src.GetB();
    int a = src.GetA();

    // Vùng ngoài (nơi IG bị bẻ xanh)
    if (offset > 0.45f) {
        // Nếu xanh chiếm ưu thế
        if (b > r && b > g) {
            float t = (offset - 0.45f) / 0.55f;
            t = std::clamp(t, 0.0f, 1.0f);

            r += int(45 * t);   // kéo về tím
            b -= int(35 * t);   // giảm xanh
        }
    }

    return Gdiplus::Color(a,
        (BYTE)std::clamp(r, 0, 255),
        (BYTE)std::clamp(g, 0, 255),
        (BYTE)std::clamp(b, 0, 255)
    );
}


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

    //if (grad->getGradientTransform()) 
    //    brush.MultiplyTransform(grad->getGradientTransform(), MatrixOrderPrepend);

    // Lấy đối tượng transform (giá trị thực, không phải con trỏ)
    const SVGTransform& transform = grad->getGradientTransform();
    
    // Thay thế logic cũ: if (grad->getGradientTransform())
    if (!transform.isEmpty()) {
        // Tạo một ma trận tạm thời của GDI+
        Gdiplus::Matrix gdiMatrix;

        // Áp dụng các phép toán (Translate, Rotate, Scale) vào ma trận này
        transform.applyToMatrix(gdiMatrix);

        // Nhân vào brush
        //brush.MultiplyTransform(&gdiMatrix);

        brush.MultiplyTransform(&gdiMatrix, Gdiplus::MatrixOrderPrepend);
        //brush.SetTransform(&gdiMatrix);
    }

    g.FillPath(&brush, &path);
}

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