#include <windows.h> 
#include <gdiplus.h>
#include "SVGTransform.h"
#include <sstream>
#include <cctype>
#include <algorithm>

void SVGTransform::clear() { 
	ops.clear(); 
}
bool SVGTransform::isEmpty() const {
	return ops.empty();
}
void SVGTransform::addTranslate(float tx, float ty) {
	ops.push_back({ TransformType::Translate, tx, ty });

}
void SVGTransform::addRotate(float deg) {
	ops.push_back({ TransformType::Rotate, deg, 0.0f });
}
void SVGTransform::addScale(float sx, float sy) {
	ops.push_back({ TransformType::Scale, sx, sy });
}
void SVGTransform::addMatrix(float a, float b, float c, float d, float e, float f) {
	ops.push_back({ TransformType::Matrix, a, b, c, d, e, f });
}