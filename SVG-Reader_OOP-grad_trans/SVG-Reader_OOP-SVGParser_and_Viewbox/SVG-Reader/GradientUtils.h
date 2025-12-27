#pragma once
#include <vector>
#include <algorithm>
#include <gdiplus.h>
#include "SVGStop.h"
#include "ColorUtils.h"

inline void NormalizeStops(
    const std::vector<SVGStop>& stops,
    std::vector<Gdiplus::REAL>& pos,
    std::vector<Gdiplus::Color>& col,
    float ratio
) {
    for (const auto& s : stops) {
        float t = s.getOffset();

        // SVG radial: offset 0 = center, 1 = edge
        t = t * ratio;
        t = std::clamp(t, 0.0f, 1.0f);

        pos.push_back(t);
        col.emplace_back(
            BYTE(s.getStopOpacity() * 255),
            s.getStopColor().r,
            s.getStopColor().g,
            s.getStopColor().b
        );
    }

    // Pad bắt buộc
    if (pos.front() > 0.0f) {
        pos.insert(pos.begin(), 0.0f);
        col.insert(col.begin(), col.front());
    }
    if (pos.back() < 1.0f) {
        pos.push_back(1.0f);
        col.push_back(col.back());
    }

    // Monotonic
    for (size_t i = 1; i < pos.size(); ++i)
        if (pos[i] <= pos[i - 1])
            pos[i] = pos[i - 1] + 0.001f;
}

inline void ResampleGradientHSV(
    const std::vector<SVGStop>& stops,
    std::vector<Gdiplus::REAL>& pos,
    std::vector<Gdiplus::Color>& col,
    int samples = 64
) {
    pos.clear();
    col.clear();

    for (int i = 0; i < samples; ++i) {
        float t = (float)i / (samples - 1);
        pos.push_back(t);

        const SVGStop* s0 = &stops.front();
        const SVGStop* s1 = &stops.back();

        for (size_t j = 1; j < stops.size(); ++j) {
            if (t <= stops[j].getOffset()) {
                s0 = &stops[j - 1];
                s1 = &stops[j];
                break;
            }
        }

        float localT = (t - s0->getOffset()) /
            std::max(0.0001f, s1->getOffset() - s0->getOffset());

        HSV c0 = RGBtoHSV(
            s0->getStopColor().r,
            s0->getStopColor().g,
            s0->getStopColor().b
        );
        HSV c1 = RGBtoHSV(
            s1->getStopColor().r,
            s1->getStopColor().g,
            s1->getStopColor().b
        );

        HSV mix;
        mix.h = HueLerp(c0.h, c1.h, localT);
        mix.s = mylerp(c0.s, c1.s, localT);
        mix.v = mylerp(c0.v, c1.v, localT);

        float opacity = mylerp(
            s0->getStopOpacity(),
            s1->getStopOpacity(),
            localT
        );

        Gdiplus::Color rgb = HSVtoRGB(mix);

        col.emplace_back(
            BYTE(opacity * 255.0f),
            rgb.GetR(),
            rgb.GetG(),
            rgb.GetB()
        );

    }
}

