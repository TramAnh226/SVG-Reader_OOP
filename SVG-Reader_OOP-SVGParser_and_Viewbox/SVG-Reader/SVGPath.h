//#pragma once
////#include <windows.h>
////#include "gdiplus.h"
//#include <string>
//#include <vector>
////#undef Point
//#include <cctype>
//#include <sstream>
//#include "SVGElement.h"
////#include "SVGParser.h"
////#include "SVGRenderer.h"
////#include "SVGElement.h"
//
//class SVGParser;
//class SVGRenderer;
//
////using namespace Gdiplus;
////using namespace std;
//
//struct PathCommand {
//	char command; // e.g., 'M', 'L', 'C', etc.
//	std::vector<float> parameters; // parameters associated with the command
//
//};
//
//class SVGPath : public SVGElement {
//private:
//	std::string pathData;
//	std::vector<PathCommand> commands;
//
//public:
//	//Gdiplus::Matrix* transform; // transformation matrix
//
//	SVGPath();
//	SVGPath(const std::string&);
//	SVGPath(const SVGPath&);
//	SVGPath& operator=(const SVGPath&);
//	~SVGPath();
//	SVGElement* clone() const;
//	std::string getPathData() const;
//	void setPathData(const std::string&);
//	void parsePathData();
//	void parse(SVGParser&, tinyxml2::XMLElement*) override;
//
//	//new
//	void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;
//	void setTransform(const std::string&) override;
//	//void setTransform(const std::string& svgTransformString)
//};


/*
// SVGPath.h
#pragma once
#include <windows.h> 
#include <gdiplus.h>
#include "SVGElement.h"
#include <vector>

class SVGParser;
class SVGRenderer;

enum class PathCommandType {
    MoveTo,
    LineTo,
    HLineTo,
    VLineTo,
    ClosePath,
    CubicBezier,
    SmoothCubicBezier
};

struct PathCommand {
    PathCommandType type;
    vector<float> params;
};

class SVGPath : public SVGElement {
//private:
public:

    vector<PathCommand> commands;

//public:
    SVGPath();
    SVGElement* clone() const override;

    //virtual void parse(SVGParser&, tinyxml2::XMLElement*);
    //virtual void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const = 0;
    void parse(SVGParser&, tinyxml2::XMLElement*) override;
    void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;

    const vector<PathCommand>& getCommands() const { return commands; }
    void addCommands(PathCommand pc) {
        commands.push_back(pc);
    }
};
*/
//#pragma once
////#include <windows.h>
////#include "gdiplus.h"
//#include <string>
//#include <vector>
////#undef Point
//#include <cctype>
//#include <sstream>
//#include "SVGElement.h"
////#include "SVGParser.h"
////#include "SVGRenderer.h"
////#include "SVGElement.h"
//
//class SVGParser;
//class SVGRenderer;
//
////using namespace Gdiplus;
////using namespace std;
//
//struct PathCommand {
//	char command; // e.g., 'M', 'L', 'C', etc.
//	std::vector<float> parameters; // parameters associated with the command
//
//};
//
//class SVGPath : public SVGElement {
//private:
//	std::string pathData;
//	std::vector<PathCommand> commands;
//
//public:
//	//Gdiplus::Matrix* transform; // transformation matrix
//
//	SVGPath();
//	SVGPath(const std::string&);
//	SVGPath(const SVGPath&);
//	SVGPath& operator=(const SVGPath&);
//	~SVGPath();
//	SVGElement* clone() const;
//	std::string getPathData() const;
//	void setPathData(const std::string&);
//	void parsePathData();
//	void parse(SVGParser&, tinyxml2::XMLElement*) override;
//
//	//new
//	void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;
//	void setTransform(const std::string&) override;
//	//void setTransform(const std::string& svgTransformString)
//};



// SVGPath.h
#pragma once
#include <windows.h> 
#include <gdiplus.h>
#include "SVGElement.h"
#include <vector>

class SVGParser;
class SVGRenderer;

enum class PathCommandType {
    MoveTo,
    LineTo,
    HLineTo,
    VLineTo,
    ClosePath,
    CubicBezier,
    SmoothCubicBezier
};

struct PathCommand {
    PathCommandType type;
    vector<float> params;
};

class SVGPath : public SVGElement {
private:
    vector<PathCommand> commands;

public:
    SVGPath();
    SVGElement* clone() const override;

    //virtual void parse(SVGParser&, tinyxml2::XMLElement*);
    //virtual void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const = 0;
    void parse(SVGParser&, tinyxml2::XMLElement*) override;
    void render(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&) const override;

    const vector<PathCommand>& getCommands() const { return commands; }
};