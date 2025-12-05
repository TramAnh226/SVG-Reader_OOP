#pragma once
#pragma once
#include "SVGElement.h"
#include <string>
#include <vector>
#include "CustomColor.h"
#include "CustomPoint.h"

using namespace std;

struct PathCommand {
	char command; // e.g., 'M', 'L', 'C', etc.
	vector<float> parameters; // parameters associated with the command
};

class SVGPath : public SVGElement {
private:
	string pathData;
	vector<PathCommand> commands;
public:
	SVGPath();
	SVGPath(const string&);
	SVGPath(const SVGPath&);
	SVGPath& operator=(const SVGPath&);
	~SVGPath();
	SVGElement* clone() const;
	string getPathData() const;
	void setPathData(const string&);
	void parsePathData();
	void parse(tinyxml2::XMLElement*);
	void render(SVGRenderer&, Gdiplus::Graphics&) const;

	// void transform(Matrix*) override;
};