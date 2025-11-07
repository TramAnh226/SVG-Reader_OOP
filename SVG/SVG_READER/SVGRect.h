// Rectangle.h
#pragma once
#include <gdiplus.h>
using namespace Gdiplus;

class rectangle {
private:
    float x, y, width, height;
    Color fill_color;
    Color stroke_color;
    float stroke_width;
    float fill_opacity, stroke_opacity;

public:
    rectangle(float x, float y, float w, float h,
              Color fill = Color(255, 200, 0, 0),     // đỏ
              Color stroke = Color(255, 0, 0, 0),     // đen
              float strokeW = 2,
              float fillOpa = 1.0f,
              float strokeOpa = 1.0f)
        : x(x), y(y), width(w), height(h),
          fill_color(fill), stroke_color(stroke),
          stroke_width(strokeW),
          fill_opacity(fillOpa), stroke_opacity(strokeOpa)
    {}

    void Render(Graphics& graphics) const {
        // Lưu trạng thái hiện tại
        GraphicsState save = graphics.Save();

        // Tạo brush để tô
        SolidBrush fillBrush(Color(static_cast<int>(fill_opacity * 255),
                                   fill_color.GetR(),
                                   fill_color.GetG(),
                                   fill_color.GetB()));

        // Tạo bút viền
        Pen pen(Color(static_cast<int>(stroke_opacity * 255),
                      stroke_color.GetR(),
                      stroke_color.GetG(),
                      stroke_color.GetB()),
                static_cast<REAL>(stroke_width));

        // Tô và vẽ viền
        graphics.FillRectangle(&fillBrush, x, y, width, height);
        graphics.DrawRectangle(&pen, x, y, width, height);

        // Khôi phục trạng thái
        graphics.Restore(save);
    }
};
