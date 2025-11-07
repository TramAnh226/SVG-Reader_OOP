#include "Shape.h"
#include <string>

//LINE 
Line::Line(float x1, float y1, float x2, float y2,
    Color stroke, float strokeW)
    : x1(x1), y1(y1), x2(x2), y2(y2),
    stroke_color(stroke), stroke_width(strokeW)
{}

void Line::draw(Graphics& graphics) const {
    Pen pen(stroke_color, stroke_width);
    graphics.DrawLine(&pen, x1, y1, x2, y2);
}


//ELLIPSE 
Ellipse::Ellipse(float cx, float cy, float rx, float ry,
    Color fill, Color stroke, float strokeW)
    : cx(cx), cy(cy), rx(rx), ry(ry),
    fill_color(fill), stroke_color(stroke),
    stroke_width(strokeW)
{}

void Ellipse::draw(Graphics& graphics) const {
    SolidBrush brush(fill_color);
    Pen pen(stroke_color, stroke_width);
    graphics.FillEllipse(&brush, cx - rx, cy - ry, 2 * rx, 2 * ry);
    graphics.DrawEllipse(&pen, cx - rx, cy - ry, 2 * rx, 2 * ry);
}


//POLYGON
Polygon::Polygon(PointF* pts, int count,
    Color fill, Color stroke, float strokeW)
    : points(pts), point_count(count),
    fill_color(fill), stroke_color(stroke),
    stroke_width(strokeW)
{}

void Polygon::draw(Graphics& graphics) const {
    SolidBrush brush(fill_color);
    Pen pen(stroke_color, stroke_width);
    graphics.FillPolygon(&brush, points, point_count);
    graphics.DrawPolygon(&pen, points, point_count);
}


//POLYLINE
Polyline::Polyline(PointF* pts, int count,
    Color stroke, float strokeW)
    : points(pts), point_count(count),
    stroke_color(stroke), stroke_width(strokeW)
{}

void Polyline::draw(Graphics& graphics) const {
    Pen pen(stroke_color, stroke_width);
    graphics.DrawLines(&pen, points, point_count);
}


//TEXT
Text::Text(const std::wstring& text, float x, float y,
    Color fill, float fontSize)
    : content(text), x(x), y(y),
    fill_color(fill), font_size(fontSize)
{}

void Text::draw(Graphics& graphics) const {
    SolidBrush brush(fill_color);
    FontFamily fontFamily(L"Arial");
    Font font(&fontFamily, font_size, FontStyleRegular, UnitPixel);

    graphics.DrawString(content.c_str(), -1, &font, PointF(x, y), &brush);
}
