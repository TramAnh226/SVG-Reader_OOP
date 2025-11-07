#include "SVGRectangle.h"


void rectangle::Render(Graphics& graphics) const {
    GraphicsState state = graphics.Save();

    SolidBrush fillBrush(Color(static_cast<int>(fill_opacity * 255),
        fill_color.GetR(),
        fill_color.GetG(),
        fill_color.GetB()));

    Pen pen(Color(static_cast<int>(stroke_opacity * 255),
        stroke_color.GetR(),
        stroke_color.GetG(),
        stroke_color.GetB()),
        static_cast<REAL>(stroke_width));

    graphics.FillRectangle(&fillBrush, x, y, width, height);
    graphics.DrawRectangle(&pen, x, y, width, height);

    graphics.Restore(state);
}
