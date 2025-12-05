#pragma comment (lib, "Gdiplus.lib")
#include <windows.h>
#undef Point
#include "SVGElement/SVG_H/SVGPath.h"
#include <gdiplus.h>
#include <cctype>
#include <sstream>

SVGPath::SVGPath() {}

SVGPath::SVGPath(const string& d) : pathData(d) {
	parsePathData();
}

SVGPath::SVGPath(const SVGPath& other)
	: pathData(other.pathData), commands(other.commands) {
}

SVGPath& SVGPath::operator=(const SVGPath& other) {
	if (this != &other) {
		pathData = other.pathData;
		commands = other.commands;
	}
	return *this;
}

SVGPath::~SVGPath() {}

SVGElement* SVGPath::clone() const {
	return new SVGPath(*this);
}

string SVGPath::getPathData() const {
	return pathData;
}

void SVGPath::parse(tinyxml2::XMLElement* node) {
	const char* dAttr = node->Attribute("d");
	if (dAttr) {
		setPathData(dAttr);
	}
}

void SVGPath::setPathData(const string& data) {
	pathData = data;
	parsePathData();
}

void SVGPath::parsePathData() {
	commands.clear();

	char cmd = 0;
	string token = "";
	vector <float> params;

	auto flushParams = [&]() {
		if (cmd != 0) {
			commands.push_back({ cmd, params });
			params.clear();
		}
		};

	for (size_t i = 0; i < pathData.size(); i++) {
		char c = pathData[i];
		if (isalpha(c)) {
			flushParams();
			cmd = c;
		}
		else if (isdigit(c) || c == '-' || c == '+' || c == '.') {
			token += c;
		}
		else if (c == ' ' || c == ',' || c == '\n' || c == '\t') {
			if (!token.empty()) {
				params.push_back(stof(token));
				token.clear();
			}
		}
	}

	if (!token.empty()) {
		params.push_back(stof(token));
	}
	flushParams();
}

void SVGPath::render(SVGRenderer& r, Gdiplus::Graphics& g) const {
	r.renderPath(g, const_cast<SVGPath*>(this), commands);
}