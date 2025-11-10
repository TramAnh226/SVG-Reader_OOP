#pragma once
#include <string>
class Color {
public:
	int r, g, b;
	Color();
	Color(int, int, int);

	static Color fromString(const std::string&)
};

