#pragma once
#include <gdiplus.h>
#include <cmath>
#include <algorithm>
#include <string.h>
#include <sstream>

class CustomColor {
public: 
	int r, g, b;
	CustomColor();
	CustomColor(int, int, int);

	static int clampCustomColorValue(int);
	static CustomColor fromStringToCustomColor(const std::string&);
	std::string fromCustomColorToString() const;

	operator Gdiplus::Color() const {
		return Gdiplus::Color(255, static_cast<BYTE>(r), static_cast<BYTE>(g), static_cast<BYTE>(b)); 
	}
};

