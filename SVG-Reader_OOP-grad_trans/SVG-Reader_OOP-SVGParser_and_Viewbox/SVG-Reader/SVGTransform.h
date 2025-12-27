#pragma once
#include <vector>
#include <string>
#include <windows.h> 
#include <gdiplus.h>

using namespace Gdiplus;
using namespace std;

enum class TransformType { Translate, Rotate, Scale };

struct TransformOp {
    TransformType type;
    float a, b;
};

class SVGTransform {
public:
    SVGTransform() = default;
    vector<TransformOp> ops;

    void clear() { ops.clear(); }
    bool isEmpty() const {
        return ops.empty();
    }
    void addTranslate(float tx, float ty) {
        ops.push_back({ TransformType::Translate, tx, ty });

    }
    void addRotate(float deg) {
        ops.push_back({ TransformType::Rotate, deg, 0.0f });
    }
    void addScale(float sx, float sy) {
        if (sy == 0) sy = sx; // SVG tiêu chuẩn: nếu thiếu sy thì sy = sx
        ops.push_back({ TransformType::Scale, sx, sy });
    }

    void applyToMatrix(Gdiplus::Matrix& m) const;

    //static SVGTransform parse(const std::string& str); // parse chuỗi "translate(...) rotate(...) ..."
    //Gdiplus::Matrix* parse(const std::string& str); // parse chuỗi "translate(...) rotate(...) ..."

};

