#ifndef CUSTOMCOLOR_H
#define CUSTOMCOLOR_H

#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include <algorithm> 
#include <stdexcept> 
#include <cctype>  
#include <unordered_map>
#include <cmath>
#include <climits>
#include <string>

namespace Gdiplus {
	class Graphics;
	class Color;
	class RectF;
	typedef unsigned char BYTE;
}
namespace tinyxml2 {
	class XMLDocument;
	class XMLElement;
	class XMLAttribute;
	class XMLNode;
}

struct CustomColor {
	int r, g, b;
	CustomColor();
	CustomColor(int, int, int);

	static int clampCustomColorValue(int);
	static CustomColor fromStringToCustomColor(const std::string&);
	std::string fromCustomColorToString() const;

	operator Gdiplus::Color() const;

	Gdiplus::ARGB getARGB(float opacity) const;

};
#endif