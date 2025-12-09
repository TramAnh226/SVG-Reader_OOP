#include "SVGRadialGradient.h"
#include "tinyxml2.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

// --- Constructors ---

// Giá trị mặc định theo chuẩn SVG: cx, cy, r, fx, fy đều mặc định là 50% (0.5) 
// nếu gradientUnits="objectBoundingBox"
SVGRadialGradient::SVGRadialGradient()
    : SVGGradient(), cx(0.5f), cy(0.5f), r(0.5f), fx(0.5f), fy(0.5f)
{
}

SVGRadialGradient::SVGRadialGradient(const SVGRadialGradient& other)
    : SVGGradient(other), 
    cx(other.cx), cy(other.cy), r(other.r), fx(other.fx), fy(other.fy)
{
}

SVGRadialGradient& SVGRadialGradient::operator=(const SVGRadialGradient& other) {
    if (this != &other) {
        SVGGradient::operator=(other); 
        this->cx = other.cx;
        this->cy = other.cy;
        this->r = other.r;
        this->fx = other.fx;
        this->fy = other.fy;
    }
    return *this;
}

SVGRadialGradient::SVGRadialGradient(float cx, float cy, float r, float fx, float fy)
    : SVGGradient(), cx(cx), cy(cy), r(r), fx(fx), fy(fy)
{
}

SVGGradient* SVGRadialGradient::clone() const {
    return new SVGRadialGradient(*this);
}

float SVGRadialGradient::getCX() const { return this->cx; }
float SVGRadialGradient::getCY() const { return this->cy; }
float SVGRadialGradient::getR() const { return this->r; }
float SVGRadialGradient::getFX() const { return this->fx; }
float SVGRadialGradient::getFY() const { return this->fy; }

void SVGRadialGradient::setCX(float v) { this->cx = v; }
void SVGRadialGradient::setCY(float v) { this->cy = v; }
void SVGRadialGradient::setR(float v) { this->r = v; }
void SVGRadialGradient::setFX(float v) { this->fx = v; }
void SVGRadialGradient::setFY(float v) { this->fy = v; }

void SVGRadialGradient::parse(SVGParser& p, tinyxml2::XMLElement* node) {
    p.parseRadialGradient(this, node);
}