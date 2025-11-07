#pragma once
#ifndef STROKE_H
#define STROKE_H
#include "Color"

class Stroke {
private: 
private: 
	Color strokeColr; 
	float strokeWidth;
	float strokeOpacity; 
public: 
	Color strokeColr;
	float strokeWidth;
	float strokeOpacity;

	void parseStroke(const map<string, string>&);
	void renderStroke(SvgRenderer& r);
};
#endif