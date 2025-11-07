// Rectangle.h

#pragma once
#include <windows.h>     // phải có trước
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
        float strokeOpa = 1.0f): x(x), y(y), width(w), height(h), fill_color(fill), stroke_color(stroke),stroke_width(strokeW),fill_opacity(fillOpa), stroke_opacity(strokeOpa){}

    void Render(Graphics& graphics) const;
};

