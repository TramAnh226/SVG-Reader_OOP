//#pragma once
////#include "SVGGroup.h"
////#include <gdiplus.h>
//#include "SVGDocumentContext.h"
////#include "Library.h"
//
//class SVGGroup;
//class SVGParser;
//class SVGRenderer;
//class SVGFactoryPattern;
//
//namespace Gdiplus {
//    class Graphics;
//    //class Color;
//    //typedef unsigned char BYTE;
//    //typedef unsigned char ARGB;
//    //class ARGB;
//}
//class SVGDocument {
//private:
////public: 
//    std::string fileName;
//    SVGGroup* rootGroup;
//
//public:
//    SVGDocumentContext context;
//
//    SVGDocument(std::string);
//    ~SVGDocument();
//
//    SVGGroup* getRootGroup();
//    void parseSVGImage(SVGParser&, SVGFactoryPattern&);
//    void renderSVGImage(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&);
//
//    void resolveReferences();
//};
#pragma once
//#include "SVGGroup.h"
//#include <gdiplus.h>
#include "SVGDocumentContext.h"
//#include "Library.h"

class SVGGroup;
class SVGParser;
class SVGRenderer;
class SVGFactoryPattern;

namespace Gdiplus {
    class Graphics;
    //class Color;
    //typedef unsigned char BYTE;
    //typedef unsigned char ARGB;
    //class ARGB;
}
class SVGDocument {
private:
    //public: 
    std::string fileName;
    SVGGroup* rootGroup;

public:
    SVGDocumentContext context;

    SVGDocument(std::string);
    ~SVGDocument();

    SVGGroup* getRootGroup();
    void parseSVGImage(SVGParser&, SVGFactoryPattern&);
    void renderSVGImage(SVGRenderer&, Gdiplus::Graphics&, const SVGDocumentContext&);

    void resolveReferences();
};
