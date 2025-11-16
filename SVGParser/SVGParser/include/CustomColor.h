#pragma once
#include <gdiplus.h>
#include <cmath>
#include <algorithm>
#include <string.h>
#include <sstream>

struct CustomColor {
	int r, g, b;
	CustomColor();
	CustomColor(int, int, int);

	static int clampCustomColorValue(int);
	static CustomColor fromStringToCustomColor(const std::string&);
	std::string fromCustomColorToString() const;
};
