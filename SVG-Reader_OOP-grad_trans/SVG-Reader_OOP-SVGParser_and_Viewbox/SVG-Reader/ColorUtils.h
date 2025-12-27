#pragma once
#include <gdiplus.h>
#include <cmath>

inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

inline float shortestAngle(float a, float b) {
    float d = fmodf(b - a, 360.0f);
    if (d > 180.0f) d -= 360.0f;
    if (d < -180.0f) d += 360.0f;
    return d;
}
struct HSV {
    float h, s, v;
};

inline HSV RGBtoHSV(int r, int g, int b) {
    float fr = r / 255.0f;
    float fg = g / 255.0f;
    float fb = b / 255.0f;

    float mx = std::max({ fr, fg, fb });
    float mn = std::min({ fr, fg, fb });
    float diff = mx - mn;

    HSV out;
    out.v = mx;
    out.s = mx == 0 ? 0 : diff / mx;

    if (diff == 0) out.h = 0;
    else if (mx == fr) out.h = 60 * fmodf((fg - fb) / diff, 6);
    else if (mx == fg) out.h = 60 * ((fb - fr) / diff + 2);
    else out.h = 60 * ((fr - fg) / diff + 4);

    if (out.h < 0) out.h += 360;
    return out;
}

inline Gdiplus::Color HSVtoRGB(const HSV& c) {
    float C = c.v * c.s;
    float X = C * (1 - fabsf(fmodf(c.h / 60.0f, 2) - 1));
    float m = c.v - C;

    float r, g, b;
    if (c.h < 60) { r = C; g = X; b = 0; }
    else if (c.h < 120) { r = X; g = C; b = 0; }
    else if (c.h < 180) { r = 0; g = C; b = X; }
    else if (c.h < 240) { r = 0; g = X; b = C; }
    else if (c.h < 300) { r = X; g = 0; b = C; }
    else { r = C; g = 0; b = X; }

    return Gdiplus::Color(
        255,
        (BYTE)((r + m) * 255),
        (BYTE)((g + m) * 255),
        (BYTE)((b + m) * 255)
    );
}

inline float HueLerp(float h0, float h1, float t) {
    float dh = h1 - h0;

    if (dh > 180.0f) dh -= 360.0f;
    if (dh < -180.0f) dh += 360.0f;

    float h = h0 + dh * t;

    if (h < 0.0f) h += 360.0f;
    if (h >= 360.0f) h -= 360.0f;

    return h;
}


inline float mylerp(float a, float b, float t) {
    return a + (b - a) * t;
}
