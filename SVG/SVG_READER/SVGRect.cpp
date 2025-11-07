#include "SVGRect.h"

rectangle::rectangle(float x, float y, float w, float h,
                     Color fill, Color stroke,
                     float strokeW, float fillOpa, float strokeOpa)
    : x(x), y(y), width(w), height(h),
      fill_color(fill), stroke_color(stroke),
      stroke_width(strokeW),
      fill_opacity(fillOpa), stroke_opacity(strokeOpa)
{}

void rectangle::Render(Graphics& graphics) const {
    // Lưu trạng thái hiện tại của Graphics
    GraphicsState state = graphics.Save();

    // Brush để tô phần trong
    SolidBrush fillBrush(Color(static_cast<int>(fill_opacity * 255),
                               fill_color.GetR(),
                               fill_color.GetG(),
                               fill_color.GetB()));

    // Pen để vẽ viền
    Pen pen(Color(static_cast<int>(stroke_opacity * 255),
                  stroke_color.GetR(),
                  stroke_color.GetG(),
                  stroke_color.GetB()),
            static_cast<REAL>(stroke_width));

    // Thực hiện vẽ
    graphics.FillRectangle(&fillBrush, x, y, width, height);
    graphics.DrawRectangle(&pen, x, y, width, height);

    // Khôi phục trạng thái Graphics
    graphics.Restore(state);
}
