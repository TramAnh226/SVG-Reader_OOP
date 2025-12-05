#pragma comment (lib, "Gdiplus.lib")
#include <windows.h>
#undef Point
#include "SVGElement/SVG_H/SVGPath.h"
#include <cctype>
#include <sstream>


SVGPath::SVGPath() { transform = new Matrix();
transform->Reset();
}

SVGPath::SVGPath(const string& d) : pathData(d) {
	parsePathData();
}

SVGPath::SVGPath(const SVGPath& other) {
	this->pathData = other.pathData;
    this->commands = other.commands;
    this->transform = other.transform->Clone();
}

SVGPath& SVGPath::operator=(const SVGPath& other) {
	if (this != &other) {
		pathData = other.pathData;
		commands = other.commands;
        delete this->transform;
        this->transform = other.transform->Clone();
	}
	return *this;
}

SVGPath::~SVGPath() {
    delete transform;
}

SVGElement* SVGPath::clone() const {
	return new SVGPath(*this);
}

string SVGPath::getPathData() const {
	return pathData;
}

void SVGPath::setPathData(const string& data) {
	pathData = data;
	parsePathData();
}


void SVGPath::parse(tinyxml2::XMLElement* node) {
    const char* dAttr = node->Attribute("d");
    if (dAttr) {
        setPathData(dAttr);
    }

    // Parse style
    style.parse(node);

    // Parse transform
    const char* tAttr = node->Attribute("transform");
    if (tAttr) {
        setTransform(tAttr);
    }
}

void SVGPath::setTransform(const std::string& transformStr) {
    if (!transform)
        transform = new Matrix();
    else
        transform->Reset();

    Matrix combined;
    combined.Reset();

    std::string s = transformStr;
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

    size_t pos = 0;
    while (pos < s.size()) {

        size_t nameEnd = s.find('(', pos);
        if (nameEnd == std::string::npos) break;

        std::string name = s.substr(pos, nameEnd - pos);

        size_t close = s.find(')', nameEnd);
        if (close == std::string::npos) break;

        std::string params = s.substr(nameEnd + 1, close - nameEnd - 1);

        std::vector<float> nums;
        {
            std::string tmp;
            for (char c : params) {
                if (c == ',' || c == ' ') {
                    if (!tmp.empty()) {
                        nums.push_back(std::stof(tmp));
                        tmp.clear();
                    }
                }
                else tmp += c;
            }
            if (!tmp.empty()) nums.push_back(std::stof(tmp));
        }

        Matrix m;
        m.Reset();

        if (name == "translate") {
            float tx = nums.size() > 0 ? nums[0] : 0;
            float ty = nums.size() > 1 ? nums[1] : 0;
            m.Translate(tx, ty);
        }
        else if (name == "scale") {
            float sx = nums.size() > 0 ? nums[0] : 1;
            float sy = nums.size() > 1 ? nums[1] : sx;
            m.Scale(sx, sy);
        }
        else if (name == "rotate") {
            float angle = nums.size() > 0 ? nums[0] : 0;
            m.Rotate(angle);
        }

        combined.Multiply(&m, MatrixOrderAppend);
        pos = close + 1;
    }

    REAL elems[6];
    combined.GetElements(elems);

    transform->SetElements(
        elems[0], elems[1],
        elems[2], elems[3],
        elems[4], elems[5]
    );
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