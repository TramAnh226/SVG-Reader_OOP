#pragma once
#include <gdiplus.h>
using namespace Gdiplus;

class SVGRenderer {
private:
    Graphics* m_graphics;

public:
    SVGRenderer(Graphics* g);
    ~SVGRenderer(); 

    void DrawCircle(float cx, float cy, float r,
        Color fillColor, Color strokeColor, float strokeWidth);

    void DrawRect(float x, float y, float width, float height,
        Color fillColor, Color strokeColor, float strokeWidth);
};
