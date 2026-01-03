#ifndef SVGTRANSFORM_H
#define SVGTRANSFORM_H

#include <vector>
#include <string>

enum class TransformType { Translate, Rotate, Scale, Matrix };

struct TransformOp {
    TransformType type;
    float a, b, c, d, e, f;
};

class SVGTransform {
public:
    SVGTransform() = default;
    std::vector<TransformOp> ops;

    void clear();
    bool isEmpty() const;
    void addTranslate(float, float);
    void addRotate(float);
    void addScale(float, float);
    void addMatrix(float, float, float, float, float, float);
};
#endif
