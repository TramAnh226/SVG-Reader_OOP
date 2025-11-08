#include "Stroke.h"

void Stroke::parse(SVGParser& p) {
    p.parseStroke();
}
void STroke::render(SVGRenderer& r) {
    r.renderStroke();
}