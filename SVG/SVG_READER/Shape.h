#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;

// (Base class)
class Shape {
public:
    virtual ~Shape() = default;

    // Hàm ảo thuần túy để các class con ghi đè
    virtual void draw(Graphics& graphics) const = 0;
};


// 
class Line : public Shape {
private:
    float x1, y1, x2, y2;
    Color stroke_color;
    float stroke_width;

public:
    Line(float x1, float y1, float x2, float y2,
        Color stroke = Color(255, 0, 0, 0),
        float strokeW = 1.0f);

    void draw(Graphics& graphics) const override;
};


// 
class Ellipse : public Shape {
private:
    float cx, cy, rx, ry;
    Color fill_color;
    Color stroke_color;
    float stroke_width;

public:
    Ellipse(float cx, float cy, float rx, float ry,
        Color fill = Color(255, 200, 0, 0),
        Color stroke = Color(255, 0, 0, 0),
        float strokeW = 1.0f);

    void draw(Graphics& graphics) const override;
};


// 
class Polygon : public Shape {
private:
    PointF* points;
    int point_count;
    Color fill_color;
    Color stroke_color;
    float stroke_width;

public:
    Polygon(PointF* pts, int count,
        Color fill = Color(255, 0, 255, 0),
        Color stroke = Color(255, 0, 0, 0),
        float strokeW = 1.0f);

    void draw(Graphics& graphics) const override;
};


// 
class Polyline : public Shape {
private:
    PointF* points;
    int point_count;
    Color stroke_color;
    float stroke_width;

public:
    Polyline(PointF* pts, int count,
        Color stroke = Color(255, 0, 0, 0),
        float strokeW = 1.0f);

    void draw(Graphics& graphics) const override;
};


// 
class Text : public Shape {
private:
    std::wstring content;
    float x, y;
    Color fill_color;
    float font_size;

public:
    Text(const std::wstring& text, float x, float y,
        Color fill = Color(255, 0, 0, 0),
        float fontSize = 16.0f);

    void draw(Graphics& graphics) const override;
};
