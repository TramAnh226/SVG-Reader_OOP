#pragma once
#include <gdiplus.h>
#include <cmath>
#include <algorithm>
#include <string.h>
#include <sstream>

struct Color {
	int r, g, b;
	Color();
	Color(int, int, int);

	static int clampColorValue(int);
	static Color fromStringToColor(const std::string&);
	std::string fromColorToString() const;
};

